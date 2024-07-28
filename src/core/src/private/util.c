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

#define IPV4_NUM_DOTS		(3)
#define IPV4_LEN_MIN		(7)
#define IPV4_LEN_MAX		(15)

// clang-format on

bool is_ipv4(const char *const str, const size_t len)
{
	if ((len < IPV4_LEN_MIN) || (len > IPV4_LEN_MAX)) {
		return false;
	}

	// How many dots have we seen?
	uint num_dots = 0;

	// How many digits have we seen in the group currently being processed
	// so far?
	uint num_digits_seen = 0;

	// The first seen digit of the group currently being processed.
	char first_digit = '\0';

	// The last digit we processed (NOT the last digit of the group).
	char last_digit = '\0';

	for (size_t i = 0; i < len; ++i) {
		if (str[i] == '.') {
			if (!num_digits_seen) {
				return false;
			}

			num_dots++;

			if (num_dots > IPV4_NUM_DOTS) {
				return false;
			}

			first_digit = '\0';
			last_digit = '\0';

			num_digits_seen = 0;
		} else if (isdigit(str[i])) {
			switch (num_digits_seen) {
			case 0:
				// The digit we're currently processing is the
				// first digit in the group. Since we know the
				// range of an IP address bit field is 0 - 255,
				// we can inspect the first digit and bail out
				// if it isn't between 0 to 2.
				if (str[i] > '2') {
					return false;
				}

				first_digit = str[i];
				break;

			case 1:
				if (last_digit == '0') {
					// We have another digit, but the first
					// one we saw was zero; leading zeros
					// are not allowed, but would be okay if
					// we didn't have another digit. This is
					// why we don't check for zero in the
					// previous case.
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
				if ((first_digit == '2') &&
				    (last_digit == '5') && (str[i] > '5')) {
					// "192.256.12.12"
					//      ^ ^ (6 > 2), not valid
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
			// "192.168.A.B" <- invalid
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
