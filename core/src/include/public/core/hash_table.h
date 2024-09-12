// SPDX-License-Identifier: MIT
//
// Copyright 2024 dgz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/// @file hash_table.h Provides the public interface for the hash table
/// implementation.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include "types.h"

struct ht_entry {
	/// @brief The key associated with this entry.
	void *key;

	/// @brief The value associated with the key.
	void *val;

	/// @brief The number of probes required to find this key during lookup.
	uint psl;
};

struct ht_conf {
	/// @brief The function to call to generate a hash value for a key.
	///
	/// @param key The key to generate a hash value from.
	/// @returns The hash value for the key.
	size_t (*hash_cb)(const void *key);

	/// @brief The initial capacity of the hash table.
	/// This must be a power of two.
	size_t initial_capacity;

	/// @brief The maximum load factor, in percent.
	uint load_fact_max;
};

#ifdef __cplusplus
}
#endif // __cplusplus
