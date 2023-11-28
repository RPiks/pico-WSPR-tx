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

#include "pico/multicore.h"
#include "pico-hf-oscillator/lib/assert.h"
#include "pico-hf-oscillator/defines.h"
#include "defines.h"
#include <piodco.h>
#include <WSPRbeacon.h>
#include "debug/logutils.h"

void InitPicoHW(void);
void Core1Entry(void);

WSPRbeaconContext *pWSPR;

int main()
{
    StampPrintf("\n");
    sleep_ms(5000);
    StampPrintf("R2BDY Pico-WSPR-tx start.");

    InitPicoHW();

    StampPrintf("WSPR beacon init...");
    
    PioDco DCO;
    GPStimeContext *pGPS = GPStimeInit(0, 9600, GPS_PPS_PIN);
    assert_(pGPS);
    DCO._pGPStime = pGPS;

    WSPRbeaconContext *pWB = WSPRbeaconInit(
        "R2BDY",        /* the Callsign. */
        "KO85",         /* the QTH locator. */
        10,             /* Tx power, dbm. */
        &DCO,           /* the PioDCO object. */
        7040000UL,      /* the dial frequency. */
        55UL,           /* the carrier freq. shift relative to dial freq. */
        6               /* RF output GPIO pin. */
        );
    assert_(pWB);
    pWSPR = pWB;

    pWB->_txSched._u8_tx_GPS_mandatory = YES;   /* Send WSPR signals only when GPS solution is active. */
    pWB->_txSched._u8_tx_GPS_past_time = NO;    /* No relying on GPS sp;ution in the past. */
    pWB->_txSched._u8_tx_slot_skip = 1;         /* 1 slot tx, 1 slot idle, etc. */
    pWB->_txSched._u8_tx_heating_pause_min = 1; /* Give 1 minute pre-heating ere first transmition. */

    StampPrintf("PioDco ADDR: %p", pWSPR->_pTX->_p_oscillator);

    StampPrintf("RF oscillator start...");
    sleep_ms(500);
    multicore_launch_core1(Core1Entry);

    for(;;)
    {
        WSPRbeaconTxScheduler(pWB, YES);

        StampPrintf(".");

        sleep_ms(1000);
    }
/*
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
*/
}
