/*
 * JTEncode.cpp - JT65/JT9/WSPR/FSQ encoder library for Arduino
 *
 * Copyright (C) 2015-2021 Jason Milldrum <milldrum@gmail.com>
 *
 * Based on the algorithms presented in the WSJT software suite.
 * Thanks to Andy Talbot G4JNT for the whitepaper on the WSPR encoding
 * process that helped me to understand all of this.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef WSPR_UTILITY_H_
#define WSPR_UTILITY_H_

#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "nhash.h"

#define WSPR_SYMBOL_COUNT   162
#define WSPR_BIT_COUNT      162
#define VALID_DBM_SIZE      28

void wspr_encode(const char * call, const char * loc, const int8_t dbm, uint8_t * symbols);
void wspr_message_prep(char * call, char * loc, int8_t dbm);
void wspr_bit_packing(uint8_t * c);
void convolve(uint8_t * c, uint8_t * s, uint8_t message_size, uint8_t bit_size);
void wspr_interleave(uint8_t * s);
void wspr_merge_sync_vector(uint8_t * g, uint8_t * symbols);
uint8_t wspr_code(char c);
void pad_callsign(char * call);

#endif
