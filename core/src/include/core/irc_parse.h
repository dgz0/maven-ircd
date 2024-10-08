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

#include <stdbool.h>
#include <stddef.h>

// clang-format off

#define IRC_MSG_CMD_LEN_MAX	(16)
#define IRC_MSG_PARAM_LEN_MAX	(256)
#define IRC_MSG_PARAM_NUM_MAX	(8)

// clang-format on

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

struct irc_param {
	char entry[IRC_MSG_PARAM_LEN_MAX + 1];
	size_t entry_len;
};

struct irc_msg {
	char cmd[IRC_MSG_CMD_LEN_MAX + 1];
	struct irc_param params[IRC_MSG_PARAM_NUM_MAX];
	size_t num_params;
	size_t cmd_len;
};

#pragma GCC diagnostic pop

void irc_msg_parse(const char *const str, const size_t str_len,
		   struct irc_msg *msg);
