///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  WSPRbeacon.h - WSPR beacon - related functions.
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
#ifndef WSPRBEACON_H_
#define WSPRBEACON_H_

#include <stdint.h>
#include <string.h>
#include <TxChannel.h>
#include <logutils.h>

typedef struct
{
    uint8_t _u8_tx_slot_skip;           /* 0=1tx0skip, 1=1tx1skip, 2=1tx2skip, ... */
    uint8_t _u8_tx_GPS_mandatory;       /* No tx when no active GPS solution. */
    uint8_t _u8_tx_GPS_past_time;       /* Override _u8_tx_GPS_mandatory if there 
                                           was solution in the past. */
    uint8_t _u8_tx_heating_pause_min;   /* No tx during this interval from start. */

} WSPRbeaconSchedule;

typedef struct
{
    uint8_t _pu8_callsign[12];
    uint8_t _pu8_locator[7];
    uint8_t _u8_txpower;

    uint8_t _pu8_outbuf[256];

    TxChannelContext *_pTX;

    WSPRbeaconSchedule _txSched;

} WSPRbeaconContext;

WSPRbeaconContext *WSPRbeaconInit(const char *pcallsign, const char *pgridsquare, int txpow_dbm,
                                  PioDco *pdco, uint32_t dial_freq_hz, uint32_t shift_freq_hz,
                                  int gpio);
void WSPRbeaconSetDialFreq(WSPRbeaconContext *pctx, uint32_t freq_hz);
int WSPRbeaconCreatePacket(WSPRbeaconContext *pctx);
int WSPRbeaconSendPacket(const WSPRbeaconContext *pctx);

int WSPRbeaconTxScheduler(WSPRbeaconContext *pctx, int verbose);

void WSPRbeaconDumpContext(const WSPRbeaconContext *pctx);

char *WSPRbeaconGetLastQTHLocator(const WSPRbeaconContext *pctx);
uint8_t WSPRbeaconIsGPSsolutionActive(const WSPRbeaconContext *pctx);

#endif
