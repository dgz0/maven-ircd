# Copyright 2024 dgz
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the “Software”), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

set(SRCS_PRIVATE
	private/conf.c
	private/ctx.c
	private/log.c
	private/net.c
	private/platform_net_unix.c)

set(HDRS_PRIVATE
	private/log.h
	private/net.h
	private/platform_net.h)

set(HDRS_PUBLIC
	public/core/conf.h
	public/core/ctx.h
	public/core/net.h
	public/core/types.h)

add_library(core STATIC ${SRCS_PRIVATE} ${HDRS_PRIVATE} ${HDRS_PUBLIC})

target_include_directories(core PUBLIC public)
target_link_libraries(core PRIVATE maven-ircd-build-settings-c)
