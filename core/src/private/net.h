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

/// @file net.h Defines the interface for network handling.

#pragma once

#include "core/net.h"

/// @brief Initializes the network instance.
/// @param net The network instance to initialize.
void net_init(struct irc_net *net);

/// @brief Add a listener for incoming client connections.
/// @param net The network instance to associate the new listener with.
/// @param host The host associated with the listener.
/// @param port The port on the host to listen for incoming connections.
/// @returns `false` if an error was encountered, or `true` otherwise.
bool net_listen(struct irc_net *net, const char *const host,
		const char *const port);

/// @brief Accept an incoming connection on an active listener.
///
/// The resulting socket will be made non-blocking.
///
/// @param net The network instance to associate the new listener with.
/// @param fd The file descriptor associated with the incoming connection.
void net_accept(struct irc_net *net, int fd);

/// @brief Reads incoming data from a file descriptor.
///
/// @param net The network instance associated with the file descriptor.
/// @param fd The file descriptor to read data from.
void net_read(struct irc_net *net, int fd);
