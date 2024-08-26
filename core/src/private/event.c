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

#include "core/event.h"

static void sub_add_to_ev(struct irc_event_data *const ev_data, irc_event_cb cb)
{
	if (ev_data->num_subs >= IRC_EVENT_SUB_NUM_MAX) {
		// error
	}
	ev_data->sub_cb[ev_data->num_subs++] = cb;
}

static void ev_dispatch(struct irc_event_data *const ev_data, void *const ctx,
			void *const ptr)
{
	if (!ev_data->num_subs) {
		return;
	}

	for (size_t i = 0; i < ev_data->num_subs; ++i) {
		ev_data->sub_cb[i](ctx, ptr);
	}
}

void irc_event_pub(struct irc_event *const ev, const enum irc_event_type type,
		   void *const ptr)
{
	if (!ev->num_events) {
		return;
	}

	for (size_t i = 0; i < ev->num_events; ++i) {
		struct irc_event_data *data = &ev->event_list[i];

		if (data->type == type) {
			ev_dispatch(&ev->event_list[i], ev->ctx, ptr);
		}
	}
}

void irc_event_sub(struct irc_event *const ev, const enum irc_event_type type,
		   irc_event_cb cb)
{
	for (size_t i = 0; i < ev->num_events; ++i) {
		if (ev->event_list[i].type == type) {
			sub_add_to_ev(&ev->event_list[i], cb);
			return;
		}
	}

	if (ev->num_events >= IRC_EVENT_NUM_MAX) {
		// error
	}

	ev->event_list[ev->num_events].type = type;

	ev->event_list[ev->num_events].sub_cb[0] = cb;
	ev->event_list[ev->num_events].num_subs++;

	ev->num_events++;
}
