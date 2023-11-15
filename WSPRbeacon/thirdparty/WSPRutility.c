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
#include "WSPRutility.h"

static char callsign[12];
static char locator[7];
static int8_t power;

/*
 * wspr_encode(const char * call, const char * loc, const uint8_t dbm, uint8_t * symbols)
 *
 * Takes a callsign, grid locator, and power level and returns a WSPR symbol
 * table for a Type 1, 2, or 3 message.
 *
 * call - Callsign (12 characters maximum).
 * loc - Maidenhead grid locator (6 characters maximum).
 * dbm - Output power in dBm.
 * symbols - Array of channel symbols to transmit returned by the method.
 *  Ensure that you pass a uint8_t array of at least size WSPR_SYMBOL_COUNT to the method.
 *
 */
void wspr_encode(const char * call, const char * loc, const int8_t dbm, uint8_t * symbols)
{
  char call_[13];
  char loc_[7];
  uint8_t dbm_ = dbm;
  strcpy(call_, call);
  strcpy(loc_, loc);

  // Ensure that the message text conforms to standards
  // --------------------------------------------------
  wspr_message_prep(call_, loc_, dbm_);

  // Bit packing
  // -----------
  uint8_t c[11];
  wspr_bit_packing(c);

  // Convolutional Encoding
  // ---------------------
  uint8_t s[WSPR_SYMBOL_COUNT];
  convolve(c, s, 11, WSPR_BIT_COUNT);

  // Interleaving
  // ------------
  wspr_interleave(s);

  // Merge with sync vector
  // ----------------------
  wspr_merge_sync_vector(s, symbols);
}

void wspr_message_prep(char * call, char * loc, int8_t dbm)
{
  // Callsign validation and padding
  // -------------------------------
	
	// Ensure that the only allowed characters are digits, uppercase letters, slash, and angle brackets
	uint8_t i;
  for(i = 0; i < 12; i++)
	{
		if(call[i] != '/' && call[i] != '<' && call[i] != '>')
		{
			call[i] = toupper(call[i]);
			if(!(isdigit(call[i]) || isupper(call[i])))
			{
				call[i] = ' ';
			}
		}
	}
  call[12] = 0;

  strncpy(callsign, call, 12);

	// Grid locator validation
  if(strlen(loc) == 4 || strlen(loc) == 6)
	{
		for(i = 0; i <= 1; i++)
		{
			loc[i] = toupper(loc[i]);
			if((loc[i] < 'A' || loc[i] > 'R'))
			{
				strncpy(loc, "AA00AA", 7);
			}
		}
		for(i = 2; i <= 3; i++)
		{
			if(!(isdigit(loc[i])))
			{
				strncpy(loc, "AA00AA", 7);
			}
		}
	}
	else
	{
		strncpy(loc, "AA00AA", 7);
	}

	if(strlen(loc) == 6)
	{
		for(i = 4; i <= 5; i++)
		{
			loc[i] = toupper(loc[i]);
			if((loc[i] < 'A' || loc[i] > 'X'))
			{
				strncpy(loc, "AA00AA", 7);
			}
		}
	}

  strncpy(locator, loc, 7);

	// Power level validation
	// Only certain increments are allowed
	if(dbm > 60)
	{
		dbm = 60;
	}
  //const uint8_t VALID_DBM_SIZE = 28;
  const int8_t valid_dbm[VALID_DBM_SIZE] =
    {-30, -27, -23, -20, -17, -13, -10, -7, -3, 
     0, 3, 7, 10, 13, 17, 20, 23, 27, 30, 33, 37, 40,
     43, 47, 50, 53, 57, 60};
  for(i = 0; i < VALID_DBM_SIZE; i++)
  {
    if(dbm == valid_dbm[i])
    {
      power = dbm;
    }
  }
  // If we got this far, we have an invalid power level, so we'll round down
  for(i = 1; i < VALID_DBM_SIZE; i++)
  {
    if(dbm < valid_dbm[i] && dbm >= valid_dbm[i - 1])
    {
      power = valid_dbm[i - 1];
    }
  }
}

void wspr_bit_packing(uint8_t * c)
{
  uint32_t n, m;

  // Determine if type 1, 2 or 3 message
	char* slash_avail = strchr(callsign, (int)'/');
	if(callsign[0] == '<')
	{
		// Type 3 message
		char base_call[13];
    memset(base_call, 0, 13);
		uint32_t init_val = 146;
		char* bracket_avail = strchr(callsign, (int)'>');
		int call_len = bracket_avail - callsign - 1;
		strncpy(base_call, callsign + 1, call_len);
		uint32_t hash = nhash_(base_call, &call_len, &init_val);
		hash &= 32767;

		// Convert 6 char grid square to "callsign" format for transmission
		// by putting the first character at the end
		char temp_loc = locator[0];
		locator[0] = locator[1];
		locator[1] = locator[2];
		locator[2] = locator[3];
		locator[3] = locator[4];
		locator[4] = locator[5];
		locator[5] = temp_loc;

		n = wspr_code(locator[0]);
		n = n * 36 + wspr_code(locator[1]);
		n = n * 10 + wspr_code(locator[2]);
		n = n * 27 + (wspr_code(locator[3]) - 10);
		n = n * 27 + (wspr_code(locator[4]) - 10);
		n = n * 27 + (wspr_code(locator[5]) - 10);

		m = (hash * 128) - (power + 1) + 64;
	}
	else if(slash_avail == (void *)0)
	{
		// Type 1 message
		pad_callsign(callsign);
		n = wspr_code(callsign[0]);
		n = n * 36 + wspr_code(callsign[1]);
		n = n * 10 + wspr_code(callsign[2]);
		n = n * 27 + (wspr_code(callsign[3]) - 10);
		n = n * 27 + (wspr_code(callsign[4]) - 10);
		n = n * 27 + (wspr_code(callsign[5]) - 10);
		
		m = ((179 - 10 * (locator[0] - 'A') - (locator[2] - '0')) * 180) + 
			(10 * (locator[1] - 'A')) + (locator[3] - '0');
		m = (m * 128) + power + 64;
	}
	else if(slash_avail)
	{
		// Type 2 message
		int slash_pos = slash_avail - callsign;
    uint8_t i;

		// Determine prefix or suffix
		if(callsign[slash_pos + 2] == ' ' || callsign[slash_pos + 2] == 0)
		{
			// Single character suffix
			char base_call[7];
      memset(base_call, 0, 7);
			strncpy(base_call, callsign, slash_pos);
			for(i = 0; i < 7; i++)
			{
				base_call[i] = toupper(base_call[i]);
				if(!(isdigit(base_call[i]) || isupper(base_call[i])))
				{
					base_call[i] = ' ';
				}
			}
			pad_callsign(base_call);

			n = wspr_code(base_call[0]);
			n = n * 36 + wspr_code(base_call[1]);
			n = n * 10 + wspr_code(base_call[2]);
			n = n * 27 + (wspr_code(base_call[3]) - 10);
			n = n * 27 + (wspr_code(base_call[4]) - 10);
			n = n * 27 + (wspr_code(base_call[5]) - 10);

			char x = callsign[slash_pos + 1];
			if(x >= 48 && x <= 57)
			{
				x -= 48;
			}
			else if(x >= 65 && x <= 90)
			{
				x -= 55;
			}
			else
			{
				x = 38;
			}

			m = 60000 - 32768 + x;

			m = (m * 128) + power + 2 + 64;
		}
		else if(callsign[slash_pos + 3] == ' ' || callsign[slash_pos + 3] == 0)
		{
			// Two-digit numerical suffix
			char base_call[7];
      memset(base_call, 0, 7);
			strncpy(base_call, callsign, slash_pos);
			for(i = 0; i < 6; i++)
			{
				base_call[i] = toupper(base_call[i]);
				if(!(isdigit(base_call[i]) || isupper(base_call[i])))
				{
					base_call[i] = ' ';
				}
			}
			pad_callsign(base_call);

			n = wspr_code(base_call[0]);
			n = n * 36 + wspr_code(base_call[1]);
			n = n * 10 + wspr_code(base_call[2]);
			n = n * 27 + (wspr_code(base_call[3]) - 10);
			n = n * 27 + (wspr_code(base_call[4]) - 10);
			n = n * 27 + (wspr_code(base_call[5]) - 10);

			// TODO: needs validation of digit
			m = 10 * (callsign[slash_pos + 1] - 48) + callsign[slash_pos + 2] - 48;
			m = 60000 + 26 + m;
			m = (m * 128) + power + 2 + 64;
		}
		else
		{
			// Prefix
			char prefix[4];
			char base_call[7];
            memset(prefix, 0, 4);
            memset(base_call, 0, 7);
			strncpy(prefix, callsign, slash_pos);
			strncpy(base_call, callsign + slash_pos + 1, 7);

			if(prefix[2] == ' ' || prefix[2] == 0)
			{
				// Right align prefix
				prefix[3] = 0;
				prefix[2] = prefix[1];
				prefix[1] = prefix[0];
				prefix[0] = ' ';
			}

			for(uint8_t i = 0; i < 6; i++)
			{
				base_call[i] = toupper(base_call[i]);
				if(!(isdigit(base_call[i]) || isupper(base_call[i])))
				{
					base_call[i] = ' ';
				}
			}
			pad_callsign(base_call);

			n = wspr_code(base_call[0]);
			n = n * 36 + wspr_code(base_call[1]);
			n = n * 10 + wspr_code(base_call[2]);
			n = n * 27 + (wspr_code(base_call[3]) - 10);
			n = n * 27 + (wspr_code(base_call[4]) - 10);
			n = n * 27 + (wspr_code(base_call[5]) - 10);

			m = 0;
			for(uint8_t i = 0; i < 3; ++i)
			{
				m = 37 * m + wspr_code(prefix[i]);
			}

			if(m >= 32768)
			{
				m -= 32768;
				m = (m * 128) + power + 2 + 64;
			}
			else
			{
				m = (m * 128) + power + 1 + 64;
			}
		}
	}

  // Callsign is 28 bits, locator/power is 22 bits.
	// A little less work to start with the least-significant bits
	c[3] = (uint8_t)((n & 0x0f) << 4);
	n = n >> 4;
	c[2] = (uint8_t)(n & 0xff);
	n = n >> 8;
	c[1] = (uint8_t)(n & 0xff);
	n = n >> 8;
	c[0] = (uint8_t)(n & 0xff);

	c[6] = (uint8_t)((m & 0x03) << 6);
	m = m >> 2;
	c[5] = (uint8_t)(m & 0xff);
	m = m >> 8;
	c[4] = (uint8_t)(m & 0xff);
	m = m >> 8;
	c[3] |= (uint8_t)(m & 0x0f);
	c[7] = 0;
	c[8] = 0;
	c[9] = 0;
	c[10] = 0;
}

void convolve(uint8_t * c, uint8_t * s, uint8_t message_size, uint8_t bit_size)
{
  uint32_t reg_0 = 0;
  uint32_t reg_1 = 0;
  uint32_t reg_temp = 0;
  uint8_t input_bit, parity_bit;
  uint8_t bit_count = 0;
  uint8_t i, j, k;

  for(i = 0; i < message_size; i++)
  {
    for(j = 0; j < 8; j++)
    {
      // Set input bit according the MSB of current element
      input_bit = (((c[i] << j) & 0x80) == 0x80) ? 1 : 0;

      // Shift both registers and put in the new input bit
      reg_0 = reg_0 << 1;
      reg_1 = reg_1 << 1;
      reg_0 |= (uint32_t)input_bit;
      reg_1 |= (uint32_t)input_bit;

      // AND Register 0 with feedback taps, calculate parity
      reg_temp = reg_0 & 0xf2d05351;
      parity_bit = 0;
      for(k = 0; k < 32; k++)
      {
        parity_bit = parity_bit ^ (reg_temp & 0x01);
        reg_temp = reg_temp >> 1;
      }
      s[bit_count] = parity_bit;
      bit_count++;

      // AND Register 1 with feedback taps, calculate parity
      reg_temp = reg_1 & 0xe4613c47;
      parity_bit = 0;
      for(k = 0; k < 32; k++)
      {
        parity_bit = parity_bit ^ (reg_temp & 0x01);
        reg_temp = reg_temp >> 1;
      }
      s[bit_count] = parity_bit;
      bit_count++;
      if(bit_count >= bit_size)
      {
        break;
      }
    }
  }
}

void wspr_interleave(uint8_t * s)
{
  uint8_t d[WSPR_BIT_COUNT];
	uint8_t rev, index_temp, i, j, k;

	i = 0;

	for(j = 0; j < 255; j++)
	{
		// Bit reverse the index
		index_temp = j;
		rev = 0;

		for(k = 0; k < 8; k++)
		{
			if(index_temp & 0x01)
			{
				rev = rev | (1 << (7 - k));
			}
			index_temp = index_temp >> 1;
		}

		if(rev < WSPR_BIT_COUNT)
		{
			d[rev] = s[i];
			i++;
		}

		if(i >= WSPR_BIT_COUNT)
		{
			break;
		}
	}

  memcpy(s, d, WSPR_BIT_COUNT);
}

void wspr_merge_sync_vector(uint8_t * g, uint8_t * symbols)
{
  uint8_t i;
  const uint8_t sync_vector[WSPR_SYMBOL_COUNT] =
	{1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0,
	 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0,
	 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1,
	 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0,
	 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
	 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1,
	 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0};

	for(i = 0; i < WSPR_SYMBOL_COUNT; i++)
	{
		symbols[i] = sync_vector[i] + (2 * g[i]);
	}
}

uint8_t wspr_code(char c)
{
  // Validate the input then return the proper integer code.
  // Change character to a space if the char is not allowed.

  if(isdigit(c))
	{
		return (uint8_t)(c - 48);
	}
	else if(c == ' ')
	{
		return 36;
	}
	else if(c >= 'A' && c <= 'Z')
	{
		return (uint8_t)(c - 55);
	}
	else
	{
		return 36;
	}
}

void pad_callsign(char * call)
{
	// If only the 2nd character is a digit, then pad with a space.
	// If this happens, then the callsign will be truncated if it is
	// longer than 6 characters.
	if(isdigit(call[1]) && isupper(call[2]))
	{
		// memmove(call + 1, call, 6);
    call[5] = call[4];
    call[4] = call[3];
    call[3] = call[2];
    call[2] = call[1];
    call[1] = call[0];
		call[0] = ' ';
	}

	// Now the 3rd charcter in the callsign must be a digit
	// if(call[2] < '0' || call[2] > '9')
	// {
	// 	// return 1;
	// }
}
