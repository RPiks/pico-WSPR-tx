///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  TxChannel.c - Produces a time-accurate `bit` stream.
//                Invokes a `modulator` function. 
//  DESCRIPTION
//      Receives data asynchronously. Calls low level modulator function
//      synchronously according to params.
//
//  HOWTOSTART
//      -
//
//  PLATFORM
//      Raspberry Pi pico.
//
//  REVISION HISTORY
//      -
//
//  PROJECT PAGE
//      https://github.com/RPiks/pico-WSPR-tx
//
//  LICENCE
//      MIT License (http://www.opensource.org/licenses/mit-license.php)
//
//  Copyright (c) 2023 by Roman Piksaykin
//  
//  Permission is hereby granted, free of charge,to any person obtaining a copy
//  of this software and associated documentation files (the Software), to deal
//  in the Software without restriction,including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY,WHETHER IN AN ACTION OF CONTRACT,TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#include "TxChannel.h"

static TxChannelContext *spTX = NULL;
static void __not_in_flash_func (TxChannelISR)(void)
{
    PioDco *pDCO = spTX->_p_oscillator;

    uint8_t byte;
    const int n2send = TxChannelPop(spTX, &byte);
    if(n2send)
    {
        const int32_t i32_compensation_millis = 
            PioDCOGetFreqShiftMilliHertz(spTX->_p_oscillator, 
                                         (uint64_t)(spTX->_u32_dialfreqhz * 1000LL));

        PioDCOSetFreq(pDCO, spTX->_u32_dialfreqhz, 
                      (uint32_t)byte * WSPR_FREQ_STEP_MILHZ - 2 * i32_compensation_millis);
    }

    spTX->_tm_future_call += spTX->_bit_period_us;

EXIT:
    hw_clear_bits(&timer_hw->intr, 1U<<spTX->_timer_alarm_num);
    timer_hw->alarm[spTX->_timer_alarm_num] = (uint32_t)spTX->_tm_future_call;

    /* LED debug signal */
    static int tick = 0;
    gpio_put(PICO_DEFAULT_LED_PIN, ++tick & 1);
}

/// @brief Initializes a TxChannel context. Starts ISR.
/// @param bit_period_us Period of data bits, BPS speed = 1e6/bit_period_us.
/// @param timer_alarm_num Pico-specific hardware timer resource id.
/// @param pDCO Ptr to oscillator.
/// @return the Context.
TxChannelContext *TxChannelInit(const uint32_t bit_period_us, uint8_t timer_alarm_num, 
                                PioDco *pDCO)
{
    assert_(pDCO);
    assert_(bit_period_us > 10);

    TxChannelContext *p = calloc(1, sizeof(TxChannelContext));
    assert_(p);

    p->_bit_period_us = bit_period_us;
    p->_timer_alarm_num = timer_alarm_num;
    p->_p_oscillator = pDCO;

    spTX = p;

    hw_set_bits(&timer_hw->inte, 1U << p->_timer_alarm_num);
    irq_set_exclusive_handler(TIMER_IRQ_0, TxChannelISR);
    irq_set_priority(TIMER_IRQ_0, 0x00);
    irq_set_enabled(TIMER_IRQ_0, true);

    p->_tm_future_call = timer_hw->timerawl + 20000LL;
    timer_hw->alarm[p->_timer_alarm_num] = (uint32_t)p->_tm_future_call;

    return p;
}

/// @brief Gets a count of bytes to send.
/// @param pctx Context.
/// @return A count of bytes.
uint8_t TxChannelPending(TxChannelContext *pctx)
{
    return 256L + (int)pctx->_ix_input - (int)pctx->_ix_output;
}

/// @brief Push a number of bytes to the output FIFO.
/// @param pctx Context.
/// @param psrc Ptr to buffer to send.
/// @param n A count of bytes to send.
/// @return A count of bytes has been sent (might be lower than n).
int TxChannelPush(TxChannelContext *pctx, uint8_t *psrc, int n)
{
    uint8_t *pdst = pctx->_pbyte_buffer;
    while(n-- && pctx->_ix_input != pctx->_ix_output)
    {
        pdst[pctx->_ix_input++] = *psrc++;
    }

    return n;
}

/// @brief Retrieves a next byte from FIFO.
/// @param pctx Context.
/// @param pdst Ptr to write a byte.
/// @return 1 if a byte has been retrived, or 0.
int TxChannelPop(TxChannelContext *pctx, uint8_t *pdst)
{
    if(pctx->_ix_input != pctx->_ix_output)
    {
        *pdst = pctx->_pbyte_buffer[pctx->_ix_output++];

        return 1;
    }

    return 0;
}

/// @brief Clears FIFO completely. Sets write&read indexes to 0.
/// @param pctx Context.
void TxChannelClear(TxChannelContext *pctx)
{
    pctx->_ix_input = pctx->_ix_output = 0;
}
