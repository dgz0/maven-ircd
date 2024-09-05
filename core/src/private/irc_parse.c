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
#include "irc_parse.h"

static void parse_cmd(struct irc_parser *const ctx, const char c,
		      struct irc_msg *const msg)
{
	if (c == ' ') {
		msg->num_args++;
		ctx->curr_pos = 0;
		ctx->cmd_seen = true;
	} else {
		msg->cmd[ctx->curr_pos++] = c;
	}
}

static bool parse_arg(struct irc_parser *const ctx, const char c,
		      struct irc_msg *const msg)
{
	switch (c) {
	case ' ':
		msg->num_args++;
		ctx->curr_pos = 0;

		return false;

	case ':':
		return true;

	default:
		msg->args[msg->num_args - 1][ctx->curr_pos++] = c;
		return false;
	}
}

void parse_msg(struct irc_parser *const ctx, const char *const str,
	       const size_t str_len, struct irc_msg *const msg)
{
#define CRLF_LEN (2)

	const size_t len = str_len - CRLF_LEN;

	for (size_t pos = 0; pos < len; ++pos) {
		if (!ctx->cmd_seen) {
			parse_cmd(ctx, str[pos], msg);
		} else if (parse_arg(ctx, str[pos], msg)) {
			memcpy(&msg->args[msg->num_args - 1][ctx->curr_pos],
			       &str[pos + 1], str_len - pos - CRLF_LEN);
			return;
		}
	}

#undef CRLF_LEN
}
