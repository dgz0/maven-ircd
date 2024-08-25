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

#include <errno.h>
#include <stdlib.h>

#include "compiler.h"
#include "core/conf.h"

// clang-format off

#define LISTENER_PORT_BASE      (10)
#define LISTENER_PORT_MAX       (65535)

// clang-format on

[[nodiscard]] bool
irc_conf_listener_add(struct irc_conf *const conf,
		      const struct irc_conf_listener *const listener,
		      enum irc_conf_status_code *const code)
{
	if (unlikely(conf->listeners.num_entries >=
		     IRC_CONF_LISTENER_NUM_MAX)) {
		*code = IRC_CONF_TOO_MANY_LISTENERS;
		return false;
	}

	const long port = strtol(listener->port, NULL, LISTENER_PORT_BASE);

	if (unlikely((port > LISTENER_PORT_MAX) || (errno == ERANGE))) {
		*code = IRC_CONF_INVALID_PORT_RANGE;
		return false;
	}

	conf->listeners.entries[conf->listeners.num_entries++] = *listener;

	*code = IRC_CONF_STATUS_OK;
	return true;
}
