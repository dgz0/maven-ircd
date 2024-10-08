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
#include <fcntl.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "core/compiler.h"
#include "core/event.h"
#include "core/log.h"
#include "core/net.h"

void irc_net_read(struct irc_net *const net, const int fd)
{
	char buf[512];

	ssize_t cnt = read(fd, buf, sizeof(buf));

	if (cnt < 0) {
		// error
	}

	struct irc_event_net_data_recv ev = { .fd = fd,
					      .data = buf,
					      .size = (size_t)cnt };

	irc_event_pub(net->event, IRC_EVENT_TYPE_NET_DATA_RECV, &ev);
}

/// @brief Sets the given socket to be non-blocking.
///
/// @param fd The file descriptor corresponding to a socket.
static void set_sock_nonblock(const int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);

	if (flags < 0) {
		// error
	}
	flags |= O_NONBLOCK;
	flags = fcntl(fd, F_SETFL, flags);

	if (flags < 0) {
		// error
	}
}

static void listener_setup(struct irc_net *const net)
{
	for (size_t i = 0; i < net->conf->listeners.num_entries; ++i) {
		const char *host = net->conf->listeners.entries[i].host;
		const char *port = net->conf->listeners.entries[i].port;

		irc_net_listen(net, host, port);
	}
}

bool irc_net_listen(struct irc_net *const net, const char *const host,
		    const char *const port)
{
	if (IRC_UNLIKELY(net->listeners.num_entries >=
			 IRC_CONF_LISTENER_NUM_MAX)) {
		IRC_LOG_ERR(net->log,
			    "platform_net_listen(): too many listeners");
		return false;
	}

	struct addrinfo hints = {};

	// Allow IPv4 or IPv6 addresses.
	hints.ai_family = AF_UNSPEC;

	// Provides sequenced, reliable, two-way, connection-based byte streams.
	// An out-of-band data transmission mechanism may be supported.
	//
	// In short: the socket will be used for TCP connections.
	hints.ai_socktype = SOCK_STREAM;

	hints.ai_flags = AI_PASSIVE;

	struct addrinfo *res = NULL;
	struct addrinfo *rp = NULL;

	if (IRC_UNLIKELY(getaddrinfo(host, port, &hints, &res) != 0)) {
		IRC_LOG_ERR(net->log,
			    "platform_net_listen(): getaddrinfo() failed");

		return false;
	}

	int fd = 0;

	for (rp = res; rp; rp = rp->ai_next) {
		fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if (IRC_UNLIKELY(fd < 0)) {
			// Whatever this is, we can't make a socket for it.
			// Don't panic yet; just try the next entry.
			continue;
		}

		set_sock_nonblock(fd);

		// Allow reuse of local addresses supplied in a bind(2) call.
		// For AF_INET sockets this means that a socket may bind, except
		// when there is an active listening socket bound to the
		// address. When the listening socket is bound to INADDR_ANY
		// with a specific port then it is not possible to bind to this
		// port for any local address.
		if (IRC_UNLIKELY(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
					    &(uint){ 1 }, sizeof(uint)) < 0)) {
			return false;
		}

		if (IRC_LIKELY(bind(fd, rp->ai_addr, rp->ai_addrlen) == 0)) {
			break;
		}

		if (IRC_UNLIKELY(!close(fd))) {
			return false;
		}
	}

	if (IRC_UNLIKELY(listen(fd, SOMAXCONN) < 0)) {
		return false;
	}

	if (!irc_net_platform_listener_add(net, fd)) {
		return false;
	}

	net->listeners.entries[net->listeners.num_entries++] = fd;

	IRC_LOG_INFO(net->log,
		     "listening for incoming client connections on %s:%s", host,
		     port);

	return true;
}

void irc_net_accept(struct irc_net *const net, const int fd)
{
	struct sockaddr user_in;
	socklen_t socklen;

	const int user_sock = accept(fd, &user_in, &socklen);

	if (IRC_UNLIKELY(user_sock < 0)) {
		IRC_LOG_ERR(net->log, "unable to accept connection: %s",
			    strerror(errno));
		return;
	}
	set_sock_nonblock(fd);
	irc_net_platform_client_add(net, user_sock);

	struct irc_event_net_client_conn ev = { .fd = user_sock };
	irc_event_pub(net->event, IRC_EVENT_TYPE_NET_CLIENT_CONN, &ev);
}

void irc_net_init(struct irc_net *const net)
{
	irc_net_platform_init(net);
	listener_setup(net);
}
