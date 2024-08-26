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

#include "core/ctx.h"
#include "core/event.h"
#include "log.h"
#include "net.h"
#include "net_platform.h"

// clang-format off

#define IRC_CMD_MAX_ARGS	(4)
#define IRC_CMD_ARGS_LEN_MAX	(510)

// clang-format on

static int parse_msg(const char *const str, const size_t str_len,
		     char res[IRC_CMD_MAX_ARGS][IRC_CMD_ARGS_LEN_MAX])
{
#define IRC_STRIP_CRLF_LEN (3)

	size_t curr_pos = 0;
	int num_args = 0;

	for (size_t i = 0; i < str_len; ++i) {
		switch (str[i]) {
		case '\r':
		case '\n':
			return num_args;

		case ' ':
			num_args++;
			curr_pos = 0;

			break;

		case ':':
			memcpy(&res[num_args][curr_pos], &str[i + 1],
			       str_len - i - IRC_STRIP_CRLF_LEN);
			return ++num_args;

		default:
			res[num_args][curr_pos++] = str[i];
			break;
		}
	}
	return num_args;

#undef IRC_STRIP_CRLF_POS
}

static void net_client_recv(void *const ctx, void *const ev_data)
{
	struct irc_ctx *m_ctx = (struct irc_ctx *)ctx;
	struct irc_event_net_data_recv *ev =
		(struct irc_event_net_data_recv *)ev_data;

	// Parse string here, pass to command handlers.
	char args[IRC_CMD_MAX_ARGS][IRC_CMD_ARGS_LEN_MAX] = {};
	parse_msg(ev->data, ev->size, args);
}

static void net_client_conn(void *const ctx, void *const ev_data)
{
	struct irc_ctx *m_ctx = (struct irc_ctx *)ctx;

	struct irc_event_net_client_conn *ev =
		(struct irc_event_net_client_conn *)ev_data;

	LOG_INFO(&m_ctx->log, "client connected");
}

void irc_init(struct irc_ctx *const ctx)
{
	ctx->event.ctx = ctx;

	ctx->net.conf = &ctx->conf;
	ctx->net.log = &ctx->log;
	ctx->net.event = &ctx->event;

	ctx->conf.log = &ctx->log;

	LOG_INFO(&ctx->log, "initialized");
}

void irc_io_loop(struct irc_ctx *const ctx)
{
	irc_event_sub(&ctx->event, IRC_EVENT_TYPE_NET_CLIENT_CONN,
		      &net_client_conn);

	irc_event_sub(&ctx->event, IRC_EVENT_TYPE_NET_DATA_RECV,
		      &net_client_recv);

	net_init(&ctx->net);

	for (;;) {
		net_platform_poll(&ctx->net);
	}
}
