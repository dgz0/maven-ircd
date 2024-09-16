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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include "types.h"

#define IRC_SIPHASH_SECRET_KEY_LEN (16)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

struct irc_ht_entry {
	/// @brief The key associated with this entry.
	void *key;

	/// @brief The value associated with the key.
	void *val;

	/// @brief The number of probes required to find this key during lookup.
	uint psl;
};

struct irc_ht_conf {
	/// @brief The initial capacity of the hash table.
	/// This must be a power of two.
	size_t initial_capacity;

	/// @brief The maximum load factor, in percent.
	uint load_fact_max;
};

#pragma GCC diagnostic pop

struct irc_ht {
	/// @brief The configuration parameters of the hash table.
	struct irc_ht_conf conf;

	/// @brief The list of entries within the hash table.
	struct irc_ht_entry *entries;

	/// @brief The maximum number of entries in the hash table allowed.
	size_t capacity;

	/// @brief The number of entries present in the hash table.
	size_t num_entries;

	u8 secret_key[IRC_SIPHASH_SECRET_KEY_LEN];
};

void irc_ht_init(struct irc_ht *ht, const struct irc_ht_conf *conf);

void irc_ht_add(struct irc_ht *ht, void *key, void *val);

void *irc_ht_get(struct irc_ht *ht, void *key);

#ifdef __cplusplus
}
#endif // __cplusplus
