#include "bitemitter.h"

BitEmitterContext *pBEC = NULL;

void __not_in_flash_func (BitEmitterISR)(void)
{
    if(!pBEC)
    {
        pBEC->_tm_future_call += 500LL;
        goto EXIT;
    }

    pBEC->_pf_modulator(pBEC->_bits_per_sample, 
                        (pBEC->_u8byte_buffer[pBEC->_ix_output] >> pBEC->_ixbit_output)
                        & ((1 << pBEC->_bits_per_sample) - 1));
    
    pBEC->_ixbit_output += pBEC->_bits_per_sample;
    if(8 == pBEC->_ixbit_output)
    {
        ++pBEC->_ix_output;
    }

    pBEC->_tm_future_call += pBEC->_bit_period_us;

EXIT:
    hw_clear_bits(&timer_hw->intr, 1U<<pBEC->_timer_alarm_num);
    timer_hw->alarm[pBEC->_timer_alarm_num] = (uint32_t)pBEC->_tm_future_call;
}

BitEmitterContext *BitEmitterInit(const uint32_t bit_period_us, uint8_t timer_alarm_num, 
                                  uint8_t bits_per_sample, void *pfmodulator)
{
    assert_(pfmodulator);
    assert_(bit_period_us > 10);

    BitEmitterContext *p = calloc(1, sizeof(BitEmitterContext));
    assert_(p);

    p->_bit_period_us = bit_period_us;
    p->_timer_alarm_num = timer_alarm_num;
    p->_bits_per_sample = bits_per_sample;
    p->_pf_modulator = pfmodulator;

    hw_set_bits(&timer_hw->inte, 1U << p->_timer_alarm_num);
    irq_set_exclusive_handler(TIMER_IRQ_0, BitEmitterISR);
    irq_set_priority(TIMER_IRQ_0, 0x00);
    irq_set_enabled(TIMER_IRQ_0, true);

    p->_tm_future_call = timer_hw->timerawl + 500LL;
    timer_hw->alarm[p->_timer_alarm_num] = (uint32_t)p->_tm_future_call;

    return p;
}
