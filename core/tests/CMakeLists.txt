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

function(declare_test TEST_NAME TEST_SRC)
	add_executable(${TEST_NAME} ${TEST_SRC})

	target_link_libraries(
		${TEST_NAME}
		maven-ircd-build-settings-c
		cmocka::cmocka
		core)
endfunction()

include(FetchContent)

FetchContent_Declare(
	cmocka
	GIT_REPOSITORY https://gitlab.com/cmocka/cmocka.git
	GIT_TAG cmocka-1.1.7
)

set(UNIT_TESTING OFF CACHE BOOL "" FORCE)
set(WITH_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(cmocka)

declare_test(test_core_conf core_test_conf.c)
declare_test(test_core_irc_parse core_test_irc_parse.c)
