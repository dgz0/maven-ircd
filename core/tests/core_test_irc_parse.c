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
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include "cmocka.h"
#include "core_private/irc_parse.h"

#define PARSE_CALL(x, m) irc_msg_parse((x), sizeof((x)) - 1, m)

static void accepts_normal(void **state)
{
	(void)state;

	struct irc_msg msg = {};

	PARSE_CALL("CMD PARAM1 PARAM2 PARAM3 :THIS IS AN ENTIRE MESSAGE\r\n",
		   &msg);

	assert_int_equal(msg.cmd_len, 3);
	assert_int_equal(msg.num_params, 4);

	assert_int_equal(msg.params[0].entry_len, 6);
	assert_int_equal(msg.params[1].entry_len, 6);
	assert_int_equal(msg.params[2].entry_len, 6);
	assert_int_equal(msg.params[3].entry_len, 25);

	assert_string_equal(msg.cmd, "CMD");
	assert_string_equal(msg.params[0].entry, "PARAM1");
	assert_string_equal(msg.params[1].entry, "PARAM2");
	assert_string_equal(msg.params[2].entry, "PARAM3");
	assert_string_equal(msg.params[3].entry, "THIS IS AN ENTIRE MESSAGE");
}

static void accepts_only_cmd(void **state)
{
	(void)state;

	struct irc_msg msg = {};

	PARSE_CALL("CMD\r\n", &msg);

	assert_int_equal(msg.cmd_len, 3);
	assert_int_equal(msg.num_params, 0);

	assert_string_equal(msg.cmd, "CMD");
}

static void handles_final_message_with_cmd(void **state)
{
	(void)state;

	struct irc_msg msg = {};

	PARSE_CALL("CMD :MESSAGE WITH SPACE\r\n", &msg);

	assert_int_equal(msg.cmd_len, 3);
	assert_int_equal(msg.num_params, 1);
	assert_int_equal(msg.params[0].entry_len, 18);

	assert_string_equal(msg.cmd, "CMD");
	assert_string_equal(msg.params[0].entry, "MESSAGE WITH SPACE");
}

int main(void)
{
	static const struct CMUnitTest tests[] = {
		[0] = cmocka_unit_test(accepts_normal),
		[1] = cmocka_unit_test(accepts_only_cmd),
		[2] = cmocka_unit_test(handles_final_message_with_cmd)
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
