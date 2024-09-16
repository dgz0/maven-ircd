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
#endif // __cplusplus

#include "conf.h"
#include "event.h"
#include "log.h"

struct irc_net {
	struct {
		int entries[IRC_CONF_LISTENER_NUM_MAX];
		size_t num_entries;
	} listeners;

	struct irc_conf *conf;
	struct irc_log *log;
	struct irc_event *event;
};

/// @brief Initializes the network module.
/// @param net The network instance to initialize.
void irc_net_init(struct irc_net *net);

/// @brief Initializes the platform specific multiplexer.
/// @param net The network instance associated with the multiplexer.
/// @returns `false` if an error was encountered, or `true` otherwise.
bool irc_net_platform_init(struct irc_net *net);

/// @brief Adds a listener for incoming client connections to the multiplexer.
/// @param net The network instance associated with the multiplexer.
/// @param fd The file descriptor associated with the listener.
/// @returns `false` if an error was encountered, or `true` otherwise.
bool irc_net_platform_listener_add(struct irc_net *net, int fd);

/// @brief Adds a client connection to the multiplexer.
/// @param net The network instance associated with the multiplexer.
/// @param fd The file descriptor associated with the client connection.
/// @returns `false` if an error was encountered, or `true` otherwise.
bool irc_net_platform_client_add(struct irc_net *net, int fd);

/// @brief Invoke the multiplexer to poll for changes in file descriptors of
/// interest.
///
/// @param net The network instance associated with the multiplexer.
void irc_net_platform_poll(struct irc_net *net);

void irc_net_accept(struct irc_net *net, const int fd);

void irc_net_read(struct irc_net *const net, const int fd);

bool irc_net_listen(struct irc_net *const net, const char *host,
		    const char *port);

#ifdef __cplusplus
}
#endif // __cplusplus
