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

#include "core/conf.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// clang-format off

#define LISTENER_PORT_NUM_MAX   (65535)

// clang-format on

enum irc_conf_retval
irc_conf_listener_add(struct irc_conf *const conf,
		      struct irc_conf_listener *const listener)
{
	if (conf->listeners.num_entries >= IRC_CONF_LISTENER_NUM_MAX) {
		return IRC_CONF_LISTENERS_TOO_MANY;
	}

	const long port = strtol(listener->port, NULL, 10);

	if ((port > LISTENER_PORT_NUM_MAX) || (errno == ERANGE)) {
		return IRC_CONF_ERR_INVALID_PORT;
	}

	conf->listeners.entries[conf->listeners.num_entries++] = *listener;
	return IRC_CONF_NO_ERROR;
}
