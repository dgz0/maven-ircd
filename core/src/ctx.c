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

#include <assert.h>

#include "core/ctx.h"
#include "core/hash_table.h"
#include "core/irc_parse.h"
#include "core/log.h"
#include "core/net.h"
#include "core/user.h"
#include "core/util.h"

static void net_client_recv(void *const ctx, void *const ev_data)
{
	struct irc_ctx *m_ctx = (struct irc_ctx *)ctx;
	struct irc_event_net_data_recv *ev =
		(struct irc_event_net_data_recv *)ev_data;

	struct irc_msg msg = {};
	irc_msg_parse(ev->data, ev->size, &msg);

	struct irc_user *user =
		irc_ht_get(&m_ctx->users, (void *)(uintptr_t)ev->fd);

	assert(user != NULL);
}

static void net_client_conn(void *const ctx, void *const ev_data)
{
	struct irc_ctx *m_ctx = (struct irc_ctx *)ctx;

	struct irc_event_net_client_conn *ev =
		(struct irc_event_net_client_conn *)ev_data;

	struct irc_user *user = irc_calloc(1, sizeof(struct irc_user));
	user->fd = ev->fd;

	irc_ht_add(&m_ctx->users, (void *)(uintptr_t)ev->fd, user);

	IRC_LOG_INFO(&m_ctx->log, "client connected");
}

static void setup_ctx_ptrs(struct irc_ctx *const ctx)
{
	ctx->event.ctx = ctx;

	ctx->net.conf = &ctx->conf;
	ctx->net.log = &ctx->log;
	ctx->net.event = &ctx->event;

	ctx->conf.log = &ctx->log;
}

static void users_table_init(struct irc_ht *const ht)
{
	assert(ht != NULL);

	static const struct irc_ht_conf cfg = {
		// clang-format off

		.initial_capacity	= 4096,
		.load_fact_max		= 75

		// clang-format on
	};
	irc_ht_init(ht, &cfg);
}

static void init_tables(struct irc_ctx *const ctx)
{
	assert(ctx != NULL);

	users_table_init(&ctx->users);
}

static void hook_events(struct irc_ctx *const ctx)
{
	irc_event_sub(&ctx->event, IRC_EVENT_TYPE_NET_CLIENT_CONN,
		      &net_client_conn);

	irc_event_sub(&ctx->event, IRC_EVENT_TYPE_NET_DATA_RECV,
		      &net_client_recv);
}

void irc_init(struct irc_ctx *const ctx)
{
	setup_ctx_ptrs(ctx);
	init_tables(ctx);
	hook_events(ctx);

	IRC_LOG_INFO(&ctx->log, "initialized");
}

IRC_NORETURN void irc_io_loop(struct irc_ctx *const ctx)
{
	irc_net_init(&ctx->net);

	for (;;) {
		irc_net_platform_poll(&ctx->net);
	}
}
