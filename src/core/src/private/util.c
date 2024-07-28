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

#define IPV4_NUM_PARTS_MAX	(3)
#define IPV4_NUM_DIGITS_MIN	(4)
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
	uint total_digits = 0;

	char last_digit = '\0';

	bool valid = true;

	for (size_t i = 0; i < len; ++i) {
		if (str[i] == '.') {
			parts++;

			if (num_digits_seen == 1) {
				valid = true;
			}
			num_digits_seen = 0;
		} else if (isdigit(str[i])) {
			total_digits++;

			switch (num_digits_seen) {
			case 0:
				if (str[i] > '2') {
					valid = false;
				}
				break;

			case 1:
				if (((last_digit == '2') && (str[i] > '5')) ||
				    (last_digit == '0')) {
					return false;
				}
				break;

			case 2:
				if ((last_digit == '5') && (str[i] > '5')) {
					return false;
				}
				break;

			default:
				return false;
			}
			last_digit = str[i];
			num_digits_seen++;
		} else {
			return false;
		}
	}
	return valid && (parts == IPV4_NUM_PARTS_MAX) &&
	       (total_digits >= IPV4_NUM_DIGITS_MIN);
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
