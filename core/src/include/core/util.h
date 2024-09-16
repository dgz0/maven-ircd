// SPDX-License-Identifier: MIT
//
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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif // cplusplus

#include <stddef.h>

/// @brief Swaps two variables.
///
/// @param x The first variable to swap.
/// @param y The second variable to swap.
#define IRC_SWAP(x, y)                  \
	({                              \
		typeof(x) x_ = (x);     \
		typeof(y) y_ = (y);     \
                                        \
		typeof(x) temp_ = (x_); \
		x_ = y_;                \
		y_ = temp_;             \
	})

/// @brief Checks to see if the given integer is a power of two.
///
/// 0 is not considered a power of two.
///
/// @param x The integer to check.
#define IRC_IS_POW2(x) ((x) && !((x) & ((x) - 1)))

void *irc_malloc(size_t size);
void *irc_calloc(size_t nmemb, size_t size);

#ifdef __cplusplus
}
#endif // cplusplus
