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

/// @file core_test_irc_parse.c Provides unit tests for the IRC message parser.

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

static void only_colon(void **state)
{
	(void)state;

	struct irc_msg msg = {};
	PARSE_CALL(":\r\n", &msg);

	assert_int_equal(msg.cmd_len, 1);
	assert_int_equal(msg.num_params, 0);

	assert_string_equal(msg.cmd, ":");
}

static void drops_extra_param(void **state)
{
	(void)state;

	struct irc_msg msg = {};
	PARSE_CALL(
		"CMD PARAM1 PARAM2 PARAM3 PARAM4 PARAM5 PARAM6 PARAM7 PARAM8 "
		":DROPPED\r\n",
		&msg);

	assert_int_equal(msg.cmd_len, 3);
	assert_int_equal(msg.num_params, 8);

	assert_int_equal(msg.params[0].entry_len, 6);
	assert_int_equal(msg.params[1].entry_len, 6);
	assert_int_equal(msg.params[2].entry_len, 6);
	assert_int_equal(msg.params[3].entry_len, 6);
	assert_int_equal(msg.params[4].entry_len, 6);
	assert_int_equal(msg.params[5].entry_len, 6);
	assert_int_equal(msg.params[6].entry_len, 6);
	assert_int_equal(msg.params[7].entry_len, 6);

	assert_string_equal(msg.cmd, "CMD");
	assert_string_equal(msg.params[0].entry, "PARAM1");
	assert_string_equal(msg.params[1].entry, "PARAM2");
	assert_string_equal(msg.params[2].entry, "PARAM3");
	assert_string_equal(msg.params[3].entry, "PARAM4");
	assert_string_equal(msg.params[4].entry, "PARAM5");
	assert_string_equal(msg.params[5].entry, "PARAM6");
	assert_string_equal(msg.params[6].entry, "PARAM7");
	assert_string_equal(msg.params[7].entry, "PARAM8");
}

static void truncate_cmd(void **state)
{
	(void)state;

	struct irc_msg msg = {};

	PARSE_CALL("THISCOMMANDWILLEXCEEDTHEMAXANDBETRUNCATED PARAM1 PARAM2 "
		   "PARAM3 PARAM4 PARAM5 PARAM6 PARAM7 PARAM8 :DROPPED\r\n",
		   &msg);

	assert_int_equal(msg.cmd_len, IRC_MSG_CMD_LEN_MAX);
	assert_int_equal(msg.num_params, 8);

	assert_int_equal(msg.params[0].entry_len, 6);
	assert_int_equal(msg.params[1].entry_len, 6);
	assert_int_equal(msg.params[2].entry_len, 6);
	assert_int_equal(msg.params[3].entry_len, 6);
	assert_int_equal(msg.params[4].entry_len, 6);
	assert_int_equal(msg.params[5].entry_len, 6);
	assert_int_equal(msg.params[6].entry_len, 6);
	assert_int_equal(msg.params[7].entry_len, 6);

	assert_string_equal(msg.cmd, "THISCOMMANDWILLE");
	assert_string_equal(msg.params[0].entry, "PARAM1");
	assert_string_equal(msg.params[1].entry, "PARAM2");
	assert_string_equal(msg.params[2].entry, "PARAM3");
	assert_string_equal(msg.params[3].entry, "PARAM4");
	assert_string_equal(msg.params[4].entry, "PARAM5");
	assert_string_equal(msg.params[5].entry, "PARAM6");
	assert_string_equal(msg.params[6].entry, "PARAM7");
	assert_string_equal(msg.params[7].entry, "PARAM8");
}

static void truncate_param(void **state)
{
	struct irc_msg msg = {};

	PARSE_CALL(
		"CMD PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1 PARAM2 PARAM3 PARAM4 PARAM5 PARAM6 PARAM7 "
		"PARAM8 :DROPPED\r\n",
		&msg);

	assert_int_equal(msg.cmd_len, 3);
	assert_int_equal(msg.num_params, 8);

	assert_int_equal(msg.params[0].entry_len, IRC_MSG_PARAM_LEN_MAX);
	assert_int_equal(msg.params[1].entry_len, 6);
	assert_int_equal(msg.params[2].entry_len, 6);
	assert_int_equal(msg.params[3].entry_len, 6);
	assert_int_equal(msg.params[4].entry_len, 6);
	assert_int_equal(msg.params[5].entry_len, 6);
	assert_int_equal(msg.params[6].entry_len, 6);
	assert_int_equal(msg.params[7].entry_len, 6);

	assert_string_equal(msg.cmd, "CMD");

	assert_string_equal(
		msg.params[0].entry,
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARA");

	assert_string_equal(msg.params[1].entry, "PARAM2");
	assert_string_equal(msg.params[2].entry, "PARAM3");
	assert_string_equal(msg.params[3].entry, "PARAM4");
	assert_string_equal(msg.params[4].entry, "PARAM5");
	assert_string_equal(msg.params[5].entry, "PARAM6");
	assert_string_equal(msg.params[6].entry, "PARAM7");
	assert_string_equal(msg.params[7].entry, "PARAM8");
}

static void truncate_params(void **state)
{
	struct irc_msg msg = {};

	PARSE_CALL(
		"CMD PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1 PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM"
		"2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2P"
		"ARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PAR"
		"AM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM"
		"2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2P"
		"ARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PAR"
		"AM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM"
		"2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2P"
		"ARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PAR"
		"AM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM"
		"2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2P"
		"ARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PAR"
		"AM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM"
		"2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2P"
		"ARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PAR"
		"AM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM"
		"2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2P"
		"ARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PAR"
		"AM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM"
		"2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2P"
		"ARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PAR"
		"AM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM"
		"2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2P"
		"ARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PAR"
		"AM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM"
		"2PARAM2PARAM2PARAM2PARAM2 PARAM3 PARAM4 PARAM5 PARAM6 PARAM7 "
		"PARAM8 :DROPPED\r\n",
		&msg);

	assert_int_equal(msg.cmd_len, 3);
	assert_int_equal(msg.num_params, 8);

	assert_int_equal(msg.params[0].entry_len, IRC_MSG_PARAM_LEN_MAX);
	assert_int_equal(msg.params[1].entry_len, IRC_MSG_PARAM_LEN_MAX);
	assert_int_equal(msg.params[2].entry_len, 6);
	assert_int_equal(msg.params[3].entry_len, 6);
	assert_int_equal(msg.params[4].entry_len, 6);
	assert_int_equal(msg.params[5].entry_len, 6);
	assert_int_equal(msg.params[6].entry_len, 6);
	assert_int_equal(msg.params[7].entry_len, 6);

	assert_string_equal(msg.cmd, "CMD");

	assert_string_equal(
		msg.params[0].entry,
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARA"
		"M1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1"
		"PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PARAM1PA"
		"RAM1PARA");

	assert_string_equal(
		msg.params[1].entry,
		"PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PA"
		"RAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARA"
		"M2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2"
		"PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PARAM2PA"
		"RAM2PARA");

	assert_string_equal(msg.params[2].entry, "PARAM3");
	assert_string_equal(msg.params[3].entry, "PARAM4");
	assert_string_equal(msg.params[4].entry, "PARAM5");
	assert_string_equal(msg.params[5].entry, "PARAM6");
	assert_string_equal(msg.params[6].entry, "PARAM7");
	assert_string_equal(msg.params[7].entry, "PARAM8");
}

static void truncate_msg(void **state)
{
	(void)state;

	struct irc_msg msg = {};

	PARSE_CALL(
		"CMD PARAM1 PARAM2 PARAM3 PARAM4 PARAM5 PARAM6 PARAM7 "
		":MSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGM"
		"SGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSG"
		"MSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMS"
		"GMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGM"
		"SGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSG"
		"MSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMS"
		"GMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGM"
		"SGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSG"
		"MSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMS"
		"GMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGM"
		"SGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSG"
		"MSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMS"
		"GMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGM"
		"SGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSG\r\n",
		&msg);

	assert_int_equal(msg.cmd_len, 3);
	assert_int_equal(msg.num_params, 8);

	assert_int_equal(msg.params[0].entry_len, 6);
	assert_int_equal(msg.params[1].entry_len, 6);
	assert_int_equal(msg.params[2].entry_len, 6);
	assert_int_equal(msg.params[3].entry_len, 6);
	assert_int_equal(msg.params[4].entry_len, 6);
	assert_int_equal(msg.params[5].entry_len, 6);
	assert_int_equal(msg.params[6].entry_len, 6);
	assert_int_equal(msg.params[7].entry_len, IRC_MSG_PARAM_LEN_MAX);

	assert_string_equal(msg.cmd, "CMD");
	assert_string_equal(msg.params[0].entry, "PARAM1");
	assert_string_equal(msg.params[1].entry, "PARAM2");
	assert_string_equal(msg.params[2].entry, "PARAM3");
	assert_string_equal(msg.params[3].entry, "PARAM4");
	assert_string_equal(msg.params[4].entry, "PARAM5");
	assert_string_equal(msg.params[5].entry, "PARAM6");
	assert_string_equal(msg.params[6].entry, "PARAM7");

	assert_string_equal(
		msg.params[7].entry,
		"MSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMS"
		"GMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGM"
		"SGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSG"
		"MSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMSGMS"
		"GMSGMSGM");
}

int main(void)
{
	static const struct CMUnitTest tests[] = {
		[0] = cmocka_unit_test(accepts_normal),
		[1] = cmocka_unit_test(accepts_only_cmd),
		[2] = cmocka_unit_test(handles_final_message_with_cmd),
		[3] = cmocka_unit_test(only_colon),
		[4] = cmocka_unit_test(drops_extra_param),
		[5] = cmocka_unit_test(truncate_cmd),
		[6] = cmocka_unit_test(truncate_param),
		[7] = cmocka_unit_test(truncate_params),
		[8] = cmocka_unit_test(truncate_msg)
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
