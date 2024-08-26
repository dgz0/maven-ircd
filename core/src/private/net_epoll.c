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

#include <errno.h>
#include <string.h>
#include <sys/epoll.h>

#include "compiler.h"
#include "log.h"
#include "net_platform.h"
#include "net.h"

#define MAX_EVENTS (32)

static struct epoll_event ev[MAX_EVENTS];
static int epfd = 0;

static void process_fd(struct irc_net *const net, const int idx,
		       const int listen_sock)
{
	if (ev[idx].data.fd == listen_sock) {
		// New connection from client.
		net_accept(net, listen_sock);
	} else if (ev[idx].events & EPOLLIN) {
		net_read(net, ev[idx].data.fd);
	}
}

static bool fd_add(struct irc_net *const net, const int fd, const u32 flags)
{
	struct epoll_event ev_data;

	ev_data.events = flags;
	ev_data.data.fd = fd;

	if (unlikely(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev_data) < 0)) {
		LOG_ERR(net->log, "epoll_ctl() failed: %s", strerror(errno));
		return false;
	}
	LOG_TRACE(net->log, "epoll_ctl() success");
	return true;
}

bool net_platform_init(struct irc_net *const net)
{
	epfd = epoll_create1(0);

	if (unlikely(epfd < 0)) {
		LOG_ERR(net->log, "net: epoll_create1() failed: %s",
			strerror(errno));
		return false;
	}
	LOG_TRACE(net->log, "net: epoll_create1() success");
	return true;
}

bool net_platform_client_add(struct irc_net *const net, const int fd)
{
	return fd_add(net, fd, EPOLLIN | EPOLLET);
}

bool net_platform_listener_add(struct irc_net *const net, const int fd)
{
	return fd_add(net, fd, EPOLLIN | EPOLLOUT | EPOLLET);
}

void net_platform_poll(struct irc_net *const net)
{
	const int num_fds = epoll_wait(epfd, ev, MAX_EVENTS, -1);

	if (unlikely(num_fds < 0)) {
		// error
	}

	for (int fd = 0; fd < num_fds; ++fd) {
		for (size_t i = 0; i < net->listeners.num_entries; ++i) {
			process_fd(net, fd, net->listeners.entries[i]);
		}
	}
}
