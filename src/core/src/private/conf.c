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

#include "util.h"
#include "core/conf.h"

// clang-format off

#define LISTENER_PORT_NUM_MAX   (65535)

// clang-format on

bool irc_conf_listener_add(struct irc_conf *const conf,
			   struct irc_conf_listener *const listener,
			   const size_t host_str_len)
{
	if (conf->listeners.num_entries >= IRC_CONF_LISTENER_NUM_MAX) {
		return false;
	}

	if (!is_ipv4(listener->host, host_str_len)) {
		return false;
	}

	if (!str_all_chars(listener->host, host_str_len)) {
		return false;
	}

	if (listener->port > LISTENER_PORT_NUM_MAX) {
		return false;
	}

	conf->listeners.entries[conf->listeners.num_entries++] = *listener;
	return true;
}
