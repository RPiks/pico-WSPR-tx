///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  defines.h - Project macros.
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
#ifndef DEFINESWSPR_H
#define DEFINESWSPR_H

//#define DEBUG

#ifdef DEBUG
#define DEBUGPRINTF(x) StampPrintf(x);
#else
#define DEBUGPRINTF(x) { }
#endif

#define FALSE 0                                     /* Something is false. */
#define TRUE 1                                       /* Something is true. */
#define BAD 0                                         /* Something is bad. */
#define GOOD 1                                       /* Something is good. */
#define INVALID 0                                 /* Something is invalid. */
#define VALID 1                                     /* Something is valid. */
#define NO 0                                          /* The answer is no. */
#define YES 1                                        /* The answer is yes. */
#define OFF 0                                       /* Turn something off. */
#define ON 1                                         /* Turn something on. */
#define ZERO 0                                 /* Something in zero state. */

#define RAM __not_in_flash_func         /* Place time-critical func in RAM */
#define RAM_A __not_in_flash("A")        /* Place time-critical var in RAM */

     /* A macro for arithmetic right shifts, with casting of the argument. */
#define iSAR(arg, rcount) (((int32_t)(arg)) >> (rcount))

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

  /* A macro of multiplication guarantees of doing so using 1 ASM command. */
#define iMUL32ASM(a,b) __mul_instruction((int32_t)(a), (int32_t)(b))

#define SECOND 1                                                  /* Time. */
#define MINUTE (60 * SECOND)
#define HOUR (60 * MINUTE)

#define kHz 1000UL                                                /* Freq. */
#define MHz 1000000UL
#define PLL_SYS_MHZ 270UL
                                                             /* WSPR defs. */
#define WSPR_FREQ_STEP_MILHZ    2930UL     /* FSK freq.bin (*2 this time). */
#define WSPR_MAX_GPS_DISCONNECT_TM  \
        (6 * HOUR)                      /* How long is active without GPS. */

#define GPS_PPS_PIN 2                                /* GPS time mark PIN. */
#define RFOUT_PIN 6                                      /* RF output PIN. */

#endif
