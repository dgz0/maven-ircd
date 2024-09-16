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

#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#include "core/compiler.h"
#include "core/conf.h"
#include "core/log.h"

// clang-format off

#define LISTENER_PORT_MIN	(0)
#define LISTENER_PORT_MAX       (65535)

// clang-format on

static bool to_int(const char *const str, int *const res)
{
	for (size_t i = 0; str[i] != '\0'; ++i) {
		if (!isdigit(str[i])) {
			return false;
		}
		*res *= 10;
		*res += str[i] - '0';
	}
	return true;
}

IRC_NODISCARD bool
irc_conf_listener_add(struct irc_conf *const conf,
		      const struct irc_conf_listener *const listener,
		      enum irc_conf_status_code *const code)
{
	if (IRC_UNLIKELY(conf->listeners.num_entries >=
			 IRC_CONF_LISTENER_NUM_MAX)) {
		IRC_LOG_ERR(conf->log,
			    "unable to add \"%s:%s\" as a listener - too many "
			    "listeners (max %d)",
			    listener->host, listener->port,
			    IRC_CONF_LISTENER_NUM_MAX);

		*code = IRC_CONF_TOO_MANY_LISTENERS;
		return false;
	}

	int port = 0;
	const bool port_conv_good = to_int(listener->port, &port);

	if (IRC_UNLIKELY(((port < LISTENER_PORT_MIN) ||
			  (port > LISTENER_PORT_MAX)) ||
			 !port_conv_good)) {
		IRC_LOG_ERR(
			conf->log,
			"unable to add \"%s:%s\" as a listener - port not "
			"valid, valid values are integers between %d and %d",
			listener->host, listener->port, LISTENER_PORT_MIN,
			LISTENER_PORT_MAX);

		*code = IRC_CONF_INVALID_PORT_RANGE;
		return false;
	}

	conf->listeners.entries[conf->listeners.num_entries++] = *listener;

	IRC_LOG_INFO(conf->log, "added \"%s:%s\" as a listener", listener->host,
		     listener->port);

	*code = IRC_CONF_STATUS_OK;
	return true;
}
