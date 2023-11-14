///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  BitEmitter.h - Produces a time-accurate bit stream.
//
//  DESCRIPTION
//  Receives data asynchronously. Calls low level bit tx funcs synchronously
//  in time according to params.
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
#ifndef BITEMITTER_H_
#define BITEMITTER_H_

#include <stdint.h>
#include <stdlib.h>
#include "hardware/clocks.h"
#include "pico/stdlib.h"
#include "../pico-hf-oscillator/lib/assert.h"

typedef struct
{
    uint64_t _tm_future_call;
    uint32_t _bit_period_us;
    uint8_t _bits_per_sample;

    uint8_t _timer_alarm_num;

    uint8_t _u8byte_buffer[256];
    uint8_t _ix_input, _ix_output;
    uint8_t _ixbit_output;

    int (*_pf_modulator)(uint8_t bits_per_sample, uint8_t sample_val);

} BitEmitterContext;

BitEmitterContext *BitEmitterInit(const uint32_t bit_period_us, uint8_t timer_alarm_num, 
                                  uint8_t bits_per_sample, void *pfmodulator);
void __not_in_flash_func (BitEmitterISR)(void);

#endif
