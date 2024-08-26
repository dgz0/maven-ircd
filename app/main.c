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

#include <stdio.h>

#include "core/conf.h"
#include "core/ctx.h"
#include "core/log.h"

static void listeners_add(struct irc_ctx *const ctx)
{
	static const struct irc_conf_listener local_listener = {
		.host = "localhost", .port = "6667"
	};

	enum irc_conf_status_code code;

	irc_conf_listener_add(&ctx->conf, &local_listener, &code);
}

static void log_msg(void *udata, const uint level, char *const str)
{
	(void)udata;
	(void)level;

	printf("log msg: %s\n", str);
}

static void ctx_setup(struct irc_ctx *const ctx)
{
	ctx->log.cb = &log_msg;
	ctx->log.lvl = IRC_LOG_LVL_TRACE;
	ctx->log.udata = ctx;

	irc_init(ctx);
	listeners_add(ctx);
}

int main(void)
{
	struct irc_ctx ctx = {};
	ctx_setup(&ctx);

	irc_io_loop(&ctx);
}
