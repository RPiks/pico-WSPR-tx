
///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  core1.c - Project part which runs on the secondary core.
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
#include <stdint.h>

#include "pico-hf-oscillator/lib/assert.h"
#include <piodco.h>
#include "defines.h"

#include <WSPRbeacon.h>

extern WSPRbeaconContext *pWSPR;

/// @brief The code of dedicated core' program running HF oscillator.
void Core1Entry()
{
    assert_(pWSPR);

    const uint32_t clkhz = PLL_SYS_MHZ * MHz;
    const uint32_t freq_hz = pWSPR->_pTX->_u32_dialfreqhz;

    PioDco *p = pWSPR->_pTX->_p_oscillator;
    assert_(p);

    /* Initialize DCO */
    assert_(0 == PioDCOInit(p, pWSPR->_pTX->_i_tx_gpio, clkhz));

    /* Run DCO. */
    //PioDCOStart(p);

    /* Set initial freq. */
    assert_(0 == PioDCOSetFreq(p, freq_hz, 0U));

    /* Run the main DCO algorithm. It spins forever. */
    PioDCOWorker(p);
}
