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

// clang-format off

#define IRC_EVENT_NUM_MAX       (256)
#define IRC_EVENT_SUB_NUM_MAX   (32)

// clang-format on

typedef void (*irc_event_cb)(void *const, void *const);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

struct irc_event_net_data_recv {
	const char *data;
	size_t size;
	int fd;
};

#pragma GCC diagnostic pop

struct irc_event_net_client_conn {
	int fd;
};

enum irc_event_type {
	// clang-format off

        /// @brief Data has been received over the network.
        IRC_EVENT_TYPE_NET_DATA_RECV    = 0,

	/// @brief A connection to the server has been established.
        IRC_EVENT_TYPE_NET_CLIENT_CONN  = 1,

	// clang-format on
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

struct irc_event_data {
	irc_event_cb sub_cb[IRC_EVENT_SUB_NUM_MAX];
	size_t num_subs;
};

#pragma GCC diagnostic pop

struct irc_event {
	struct irc_event_data event_list[IRC_EVENT_NUM_MAX];
	void *ctx;
};

void irc_event_pub(struct irc_event *ev, enum irc_event_type type, void *ptr);
void irc_event_sub(struct irc_event *ev, enum irc_event_type type,
		   irc_event_cb cb);

#ifdef __cplusplus
}
#endif // __cplusplus
