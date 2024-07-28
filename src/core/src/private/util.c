// Copyright 2024 dgz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <ctype.h>
#include "util.h"
#include "core/types.h"

// clang-format off

#define IPV4_NUM_PARTS		(3)
#define IPV4_LEN_MIN		(7)
#define IPV4_LEN_MAX		(15)

// clang-format on

bool is_ipv4(const char *const str, const size_t len)
{
	if ((len < IPV4_LEN_MIN) || (len > IPV4_LEN_MAX)) {
		return false;
	}

	uint parts = 0;
	uint num_digits_seen = 0;

	char last_digit = '\0';

	for (size_t i = 0; i < len; ++i) {
		if (str[i] == '.') {
			if (!num_digits_seen) {
				return false;
			}

			parts++;

			if (parts > IPV4_NUM_PARTS) {
				return false;
			}
			num_digits_seen = 0;
		} else if (isdigit(str[i])) {
			switch (num_digits_seen) {
			case 0:
				// We haven't seen any other digits yet, but
				// since we know this is a digit ranged from
				// 0-9, we don't care.
				break;

			case 1:
				if (last_digit == '0') {
					// We have another digit, but the first
					// one we saw was zero; leading zeros
					// are not allowed.
					//
					// "192.168.01.100"
					//          ^ error because next
					//            character is 1
					return false;
				}

				if ((last_digit == '2') && (str[i] > '5')) {
					// Each field of an IP address is ranged
					// from 000 to 255.
					//
					// "192.267.0.0"
					//       ^ (6 > 5), not valid
					return false;
				}
				break;

			case 2:
				if ((last_digit == '5') && (str[i] > '5')) {
					// Each field of an IP address is ranged
					// from 000 to 255.
					//
					// "192.257.0.0"
					//        ^ (7 > 5), not valid
					return false;
				}
				break;

			default:
				// Too many digits seen; there can only be 3
				// digits at most in a field.
				return false;
			}

			last_digit = str[i];
			num_digits_seen++;
		} else {
			// Not a number or a dot.
			//
			// "192.168.A.B"
			//          ^ ^ invalid characters
			return false;
		}
	}
	return true;
}

bool str_all_chars(const char *const str, const size_t str_len)
{
	for (size_t i = 0; i < str_len; ++i) {
		if (!isalpha(str[i])) {
			return false;
		}
	}
	return true;
}
