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

/// @file conf.h Defines the configuration structure of an IRC server context.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>
#include <stddef.h>

#include "log.h"

// clang-format off

/// @brief The maximum length of a listener's host.
///
/// * The maximum IPv4 length is 15 characters.
/// * The maximum IPv6 length is 45 characters.
/// * The maximum domain name length is 253 characters.
#define IRC_CONF_LISTENER_HOST_LEN_MAX  (253)

/// @brief The maximum length of a port number.
#define IRC_CONF_LISTENER_PORT_LEN_MAX  (5)

/// @brief The maximum number of listeners allowed.
#define IRC_CONF_LISTENER_NUM_MAX       (16)

// clang-format on

enum irc_conf_status_code {
	// clang-format off

	/// @brief No errors were encountered in the processing of a
	/// configuration setter.
	IRC_CONF_STATUS_OK		= 0,

	/// @brief The desired listener specified a malformed port value.
	IRC_CONF_INVALID_PORT_RANGE	= 1,

	/// @brief The configuration has exceeded the number of listeners
	/// allowed.
	IRC_CONF_TOO_MANY_LISTENERS	= 2,

	// clang-format on
};

/// @brief Defines the attributes of a listener.
struct irc_conf_listener {
	/// @brief The hostname associated with the listener. This can be an
	/// IPv4 or IPv6 address, or a domain name.
	char host[IRC_CONF_LISTENER_HOST_LEN_MAX + 1];

	/// @brief The port number associated with the listener.
	char port[IRC_CONF_LISTENER_PORT_LEN_MAX + 1];
};

/// @brief Defines the full configuration scheme of an IRC server context.
struct irc_conf {
	/// @brief Holds the listener entries.
	struct {
		/// @brief The list of listener entries.
		struct irc_conf_listener entries[IRC_CONF_LISTENER_NUM_MAX];

		/// @brief The number of valid listeners in the @ref entries
		/// array.
		size_t num_entries;
	} listeners;

	/// @brief The IRC context's @ref irc_log instance.
	struct irc_log *log;
};

/// @brief Adds a listener for incoming client connections.
///
/// @param conf The configuration instance to associate the listener entry with.
///
/// @param listener The structure containing the user desired listener
/// parameters.
///
/// @param code The detailed return code. If this function returns `true`, this
/// will be set to @ref IRC_CONF_STATUS_OK. Otherwise, `code` will indicate an
/// error condition.
///
/// @returns `true` if no errors were encountered, or `false` otherwise.
bool irc_conf_listener_add(struct irc_conf *conf,
		      const struct irc_conf_listener *listener,
		      enum irc_conf_status_code *code);

#ifdef __cplusplus
}
#endif // __cplusplus
