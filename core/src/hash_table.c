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

/// @file hash_table.c Defines the implementation of the hash table.
///
/// * This implementation uses the the robin hood hashing technique, which is
///   based on open addressing. No linked lists or additional pointers are used,
///   improving cache performance and lowering memory requirements.
///
/// * Linear probing is used due to the innate resistance to clustering the
///   robin hood technique offers, which further improves cache performance.
///
/// * The initial capacity must always be a power of two. When or if the hash
///   table needs to resize, the hash table will be resized by the next power of
///   two to reduce collisions.
///
///   Insertion example: assume that the hash table was initialized with an
///   initial capacity of 2,048 elements, and the resize threshold is set to
///   ~75%. If the number of entries in the hash table is 1,535 (~75% of 2,048),
///   then on the next insertion, the hash table will be resized to a capacity
///   of 4,096 (2,048 * 2) and the hash table will contain 1,536 elements.
///
///   Removals do not shrink the hash table size; this might be something to
///   look at in the future.
///
/// * The underlying data associated with the keys and values is unknown to us.
///   This is particularly important when the hash table is being destroyed; you
///   are responsible for freeing any memory associated with the keys or values
///   of each entry before destroying the hash table, if necessary.
///
/// * SipHash 2-4 is used as the hash function. The secret key is generated when
///   the hash table is initialized using a CSPRNG.

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/hash_table.h"
#include "core/types.h"
#include "core/util.h"

#include "siphash.h"

#define SIPHASH_OUT_LEN (8)

static void secret_key_gen(u8 *const buf)
{
	assert(buf != NULL);

#ifdef IRC_HAVE_ARC4RANDOM_BUF
	arc4random_buf(buf, IRC_SIPHASH_SECRET_KEY_LEN);
#endif
}

static size_t hash_key(struct irc_ht *const ht, void *key)
{
	u8 res[SIPHASH_OUT_LEN] = {};

	siphash(key, 0, ht->secret_key, res, SIPHASH_OUT_LEN);

	size_t val = 0;
	memcpy(&val, res, SIPHASH_OUT_LEN);

	return val;
}

void irc_ht_init(struct irc_ht *const ht, const struct irc_ht_conf *const conf)
{
	assert(ht != NULL);
	assert(conf != NULL);
	assert(IRC_IS_POW2(conf->initial_capacity));

	ht->entries =
		irc_calloc(conf->initial_capacity, sizeof(struct irc_ht_entry));

	ht->capacity = conf->initial_capacity;
	ht->conf = *conf;

	ht->num_entries = 0;

	secret_key_gen(ht->secret_key);
}

void irc_ht_add(struct irc_ht *const ht, void *const key, void *const val)
{
	const size_t hash = hash_key(ht, key) & (ht->capacity - 1);

	if (!ht->entries[hash].key) {
		ht->entries[hash].key = key;
		ht->entries[hash].val = val;
		ht->entries[hash].psl = 0;
	} else {
		// Handle collision here.
		;
	}
	ht->num_entries++;
}

void *irc_ht_get(struct irc_ht *const ht, void *const key)
{
	const size_t hash = hash_key(ht, key) & (ht->capacity - 1);

	if (ht->entries[hash].key) {
		return ht->entries[hash].val;
	}
	return NULL;
}
