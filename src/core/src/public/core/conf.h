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

/// @file conf.h Defines the configuration structure of an IRC server instance.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>
#include <stddef.h>
#include "types.h"

// clang-format off

#define IRC_CONF_LISTENER_NUM_MAX	(10)
#define IRC_CONF_LISTENER_HOST_LEN_MAX	(32)

#define IRC_CONF_LISTENER_PORT_LEN_MAX	(10)

// clang-format on

enum irc_conf_retval {
	/// @brief The function completed successfully with no errors.
	IRC_CONF_NO_ERROR,

	/// @brief The maximum number of listeners has been reached.
	IRC_CONF_LISTENERS_TOO_MANY,

	/// @brief The IP address specified is not properly formatted.
	IRC_CONF_ERR_INVALID_IP_ADDRESS,

	/// @brief The specified port is invalid.
	IRC_CONF_ERR_INVALID_PORT
};

struct irc_conf_listener {
	char host[IRC_CONF_LISTENER_HOST_LEN_MAX + 1];
	char port[IRC_CONF_LISTENER_PORT_LEN_MAX + 1];
};

struct irc_conf {
	struct {
		struct irc_conf_listener entries[IRC_CONF_LISTENER_NUM_MAX];
		size_t num_entries;
	} listeners;
};

enum irc_conf_retval irc_conf_listener_add(struct irc_conf *conf,
					   struct irc_conf_listener *listener);

#ifdef __cplusplus
}
#endif // __cplusplus
