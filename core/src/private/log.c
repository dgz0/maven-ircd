// SPDX-License-Identifier: MIT
//
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

#define LOG_MSG_MAX (512)

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "log.h"

ATTRIB_FMT(printf, 3, 4)
void log_dispatch(struct irc_log *const log, const uint lvl,
		  const char *const msg, ...)
{
#define LVL_DEF(str) { str, sizeof(str) }

	// clang-format off
	static const struct {
		const char *const str;
		const size_t len;
	} lvl_data[] = {
		[IRC_LOG_LVL_INFO]      = LVL_DEF("[info] "),
		[IRC_LOG_LVL_WARN]      = LVL_DEF("[warn] "),
		[IRC_LOG_LVL_ERR]       = LVL_DEF("[error] "),
		[IRC_LOG_LVL_DBG]       = LVL_DEF("[debug] "),
                [IRC_LOG_LVL_TRACE]     = LVL_DEF("[trace] ")
	};
	// clang-format on

#undef LVL_DEF

	char str[LOG_MSG_MAX];
	memcpy(str, lvl_data[lvl].str, lvl_data[lvl].len);

	va_list args;
	va_start(args, msg);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
	vsprintf(&str[lvl_data[lvl].len - 1], msg, args);
#pragma GCC diagnostic pop

	va_end(args);

	log->cb(log->udata, lvl, str);
}
