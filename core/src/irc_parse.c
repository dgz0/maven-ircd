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

#include <string.h>
#include "core_private/irc_parse.h"

static void process_cmd(const char *const str, ptrdiff_t len,
			struct irc_msg *const msg)
{
	if (len >= IRC_MSG_CMD_LEN_MAX) {
		len = IRC_MSG_CMD_LEN_MAX;
	}
	memcpy(msg->cmd, str, (size_t)len);
	msg->cmd_len = (size_t)len;
}

static void process_param(const char *const str, ptrdiff_t len,
			  struct irc_msg *const msg)
{
	if (msg->num_params >= IRC_MSG_PARAM_NUM_MAX) {
		return;
	}

	if (len >= IRC_MSG_PARAM_LEN_MAX) {
		len = IRC_MSG_PARAM_LEN_MAX;
	}
	msg->params[msg->num_params].entry_len = (size_t)len;
	memcpy(&msg->params[msg->num_params++], str, (size_t)len);
}

static void process_word(const char *const str, const ptrdiff_t word_len,
			 struct irc_msg *const msg)
{
	if (!msg->cmd_len) {
		process_cmd(str, word_len, msg);
	} else {
		process_param(str, word_len, msg);
	}
}

void irc_msg_parse(const char *const str, const size_t str_len,
		   struct irc_msg *const msg)
{
	const size_t len = str_len - (sizeof("\r\n") - 1);

	for (size_t pos = 0; pos < len;) {
		const size_t off = len - pos;

		if ((str[pos] == ':') && (str[pos + 1] != '\r')) {
			process_param(&str[pos + 1], (ptrdiff_t)off - 1, msg);
			break;
		}

		// Since we're starting from the left, it's very likely we'll
		// encounter a space first.
		const char *token = memchr(&str[pos], ' ', off);

		if (token) {
			const ptrdiff_t word_len = token - &str[pos];
			process_word(&str[pos], word_len, msg);

			pos += ((size_t)word_len + 1);
		} else {
			// No whitespace left. This can mean a few things:
			//
			// a) Only a command was sent.
			// b) We're at the last parameter that is not prefixed
			//    with a ":".
			process_word(&str[pos], (ptrdiff_t)off, msg);
			break;
		}
	}
}
