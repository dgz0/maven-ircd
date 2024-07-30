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

#include "core/net.h"
#include "log.h"

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "platform_net.h"

static int set_socket_option(const int fd, const int lvl, const int opt)
{
	uint optval = 1;
	return setsockopt(fd, lvl, opt, (const void *)&optval, sizeof(uint));
}

bool platform_net_listen(struct irc_net *const net, const char *const host,
			 const char *const port)
{
	if (net->listeners.num_entries >= MAVEN_IRCD_LISTENERS_NUM_MAX) {
		LOG_ERR(net->log, "platform_net_listen(): too many listeners");
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

	if (getaddrinfo(host, port, &hints, &res) != 0) {
		LOG_ERR(net->log,
			"platform_net_listen(): getaddrinfo() failed");

		return false;
	}

	int fd = 0;

	for (rp = res; rp; rp = rp->ai_next) {
		fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if (!fd) {
			// Whatever this is, we can't make a socket for it.
			// Don't panic yet; just try the next entry.
			continue;
		}

		// Allow reuse of local addresses supplied in a bind(2) call.
		// For AF_INET sockets this means that a socket may bind, except
		// when there is an active listening socket bound to the
		// address. When the listening socket is bound to INADDR_ANY
		// with a specific port then it is not possible to bind to this
		// port for any local address.
		if (!set_socket_option(fd, SOL_SOCKET, SO_REUSEADDR)) {
			return false;
		}

		if (bind(fd, rp->ai_addr, rp->ai_addrlen)) {
			break;
		}

		if (!close(fd)) {
			return false;
		}
	}

	if (listen(fd, SOMAXCONN) < 0) {
		return false;
	}

	net->listeners.entries[net->listeners.num_entries++] =
		(void *)(uintptr_t)fd;
	return true;
}
