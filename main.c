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
//  External GPS receiver is optional and serves a purpose of holding
//  WSPR time window order and accurate frequancy drift compensation.
//
//  HOWTOSTART
//      ./build.sh; cp ./build/*.uf2 /media/Pico_Board/
//
//  PLATFORM
//      Raspberry Pi pico.
//
//  REVISION HISTORY
//      Rev 0.1   18 Nov 2023
//      Rev 0.5   02 Dec 2023
//
//  PROJECT PAGE
//      https://github.com/RPiks/pico-WSPR-tx
//
//  SUBMODULE PAGE
//      https://github.com/RPiks/pico-hf-oscillator
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
#include <defines.h>
#include <piodco.h>
#include <WSPRbeacon.h>
#include <logutils.h>
#include <protos.h>

WSPRbeaconContext *pWSPR;

int main()
{
    StampPrintf("\n");
    sleep_ms(5000);
    StampPrintf("R2BDY Pico-WSPR-tx start.");

    InitPicoHW();

    PioDco DCO = {0};

    StampPrintf("WSPR beacon init...");

    WSPRbeaconContext *pWB = WSPRbeaconInit(
        "R2BDY",        /* the Callsign. */
        "KO85",         /* the QTH locator. */
        16,             /* Tx power, dbm. */
        &DCO,           /* the PioDCO object. */
        7040000UL,      /* the dial frequency. */
        55UL,           /* the carrier freq. shift relative to dial freq. */
        RFOUT_PIN       /* RF output GPIO pin. */
        );
    assert_(pWB);
    pWSPR = pWB;
    
    pWB->_txSched._u8_tx_GPS_mandatory = YES;   /* Send WSPR signals only when GPS solution is active. */
    pWB->_txSched._u8_tx_GPS_past_time = YES;   /* ?relying on GPS solution in the past. */
    pWB->_txSched._u8_tx_slot_skip = 2;         /* 1 slot tx, 1 slot idle, etc. */
    pWB->_txSched._u8_tx_heating_pause_min = 1; /* Give 1 minute pre-heating ere first transmition. */

    multicore_launch_core1(Core1Entry);
    StampPrintf("RF oscillator started.");

    DCO._pGPStime = GPStimeInit(0, 9600, GPS_PPS_PIN);
    assert_(DCO._pGPStime);

    int tick = 0;
    for(;;)
    {
        WSPRbeaconTxScheduler(pWB, YES);
        
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(100);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);

#ifdef DEBUG
        if(0 == ++tick % 60)
            WSPRbeaconDumpContext(pWB);
#endif
        sleep_ms(900);
    }
}
