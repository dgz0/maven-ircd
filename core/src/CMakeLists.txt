# SPDX-License-Identifier: MIT
#
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

set(SRCS
	conf.c
	ctx.c
	event.c
	irc_parse.c
	log.c
	net_epoll.c
	net.c)

set(HDRS_PRIVATE
	include/private/core_private/compiler.h
	include/private/core_private/irc_parse.h
	include/private/core_private/log.h
	include/private/core_private/net_platform.h
	include/private/core_private/net.h)

set(HDRS_PUBLIC
	include/public/core/conf.h
	include/public/core/ctx.h
	include/public/core/event.h
	include/public/core/log.h
	include/public/core/net.h
	include/public/core/types.h)

# Build the core as a static library.
#
# NOTE: The core should never be built as a shared library; please don't do
# this.
add_library(core STATIC ${SRCS} ${HDRS_PRIVATE} ${HDRS_PUBLIC})

# Expose the public header files to targets that link to us.
target_include_directories(core PUBLIC include/public)

# Expose the private header files to targets that link to us if unit testing is
# enabled.
if (MAVEN_IRCD_BUILD_UNIT_TESTS)
	target_include_directories(core PUBLIC include/private)
else()
	target_include_directories(core PRIVATE include/private)
endif()

# Make sure the core is compiled with the project wide C build settings.
target_link_libraries(core PRIVATE maven-ircd-build-settings-c)
