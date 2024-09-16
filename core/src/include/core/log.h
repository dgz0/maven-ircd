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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "compiler.h"
#include "types.h"

enum irc_log_lvl {
	// clang-format off

	IRC_LOG_LVL_INFO	= 1,
	IRC_LOG_LVL_WARN	= 2,
	IRC_LOG_LVL_ERR		= 3,
	IRC_LOG_LVL_DBG		= 4,
	IRC_LOG_LVL_TRACE	= 5,
	IRC_LOG_LVL_FATAL	= 6

	// clang-format on
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

struct irc_log {
	void *udata;
	void (*cb)(void *udata, const uint lvl, char *str);
	uint lvl;
};

#pragma GCC diagnostic pop

void irc_log_dispatch(struct irc_log *log, uint lvl, const char *msg, ...)
	IRC_ATTRIB_FMT(printf, 3, 4);

#define IRC_LOG_MSG(logger, level, args...)                      \
	({                                                       \
		struct irc_log *log = (logger);                  \
                                                                 \
		if ((log) && (log->lvl >= (level)) && log->cb) { \
			irc_log_dispatch(log, (level), args);    \
		}                                                \
	})

// clang-format off

#define IRC_LOG_INFO(log, args...) \
	(IRC_LOG_MSG((log), IRC_LOG_LVL_INFO, args))

#define IRC_LOG_WARN(log, args...) \
	(IRC_LOG_MSG((log), IRC_LOG_LVL_WARN, args))

#define IRC_LOG_ERR(log, args...) \
	(IRC_LOG_MSG((log), IRC_LOG_LVL_ERR, args))

#define IRC_LOG_FATAL(log, args...) \
	(IRC_LOG_MSG((log), IRC_LOG_LVL_FATAL, args))

#define IRC_LOG_DBG(log, args...) \
	(IRC_LOG_MSG((log), IRC_LOG_LVL_DBG, args))

#define IRC_LOG_TRACE(log, args...) \
	(IRC_LOG_MSG((log), IRC_LOG_LVL_TRACE, args))

// clang-format on

#ifdef __cplusplus
}
#endif // __cplusplus
