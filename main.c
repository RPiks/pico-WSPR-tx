///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  main.c - The project entry point.
// 
//  DESCRIPTION
//      The pico-WSPR-tx project provides WSPR beacon function using only
//  Pi Pico board. *NO* additional hardware such as freq.synth required.
//
//  HOWTOSTART
//  .
//
//  PLATFORM
//      Raspberry Pi pico.
//
//  REVISION HISTORY
// 
//      Rev 0.1   18 Nov 2023
//  Initial release.
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "defines.h"

#include "pico/multicore.h"
#include "pico-hf-oscillator/lib/assert.h"
#include "pico-hf-oscillator/defines.h"
#include <piodco.h>

#include <WSPRbeacon.h>

#include "debug/logutils.h"

PioDco DCO;

int FSK4mod(uint32_t frq_step_millihz, uint8_t shift_index);
void InitPicoHW(void);
void Core1Entry(void);

int main()
{
    DEBUGPRINTF("\n");
    sleep_ms(1000);
    DEBUGPRINTF("Pico-WSPR-tx start.");

    InitPicoHW();

    DEBUGPRINTF("WSPR beacon init...");
    WSPRbeaconContext *pWB = WSPRbeaconInit("R2BDY", "KO85", 6, &DCO);
    WSPRbeaconSetDialFreq(pWB, WSPR_DIAL_FREQ_HZ + WSPR_SHIFT_FREQ_HZ);
    DEBUGPRINTF("OK");
    
    DEBUGPRINTF("Create packet...");
    WSPRbeaconCreatePacket(pWB);
    DEBUGPRINTF("OK");
    
    sleep_ms(100);
    DEBUGPRINTF("Start oscillator on Core #1...");
    multicore_launch_core1(Core1Entry);
    DEBUGPRINTF("OK");

    DEBUGPRINTF("Sending WSPR packet...");
    WSPRbeaconSendPacket(pWB);

    for(;;)
    {
        DEBUGPRINTF("tick.");
        sleep_ms(1000);
    }
}

/// @brief Frequency shift keying modulator wrapper.
/// @param frq_step_millihz Shift step, milliHertz.
/// @param shift_index Shift index, [0..3] for WSPR.
/// @return 0 if OK.
int FSK4mod(uint32_t frq_step_millihz, uint8_t shift_index)
{
    PioDCOSetFreq(&DCO, WSPR_DIAL_FREQ_HZ + WSPR_SHIFT_FREQ_HZ, 
                  frq_step_millihz * (uint32_t)shift_index);
    return 0;
}
