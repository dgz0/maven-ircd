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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

#include "cmocka.h"

#pragma GCC diagnostic pop

#include "core/conf.h"

static void accept_normal_port_num(void **state)
{
	(void)state;

	struct irc_conf conf = {};

	enum irc_conf_status_code code;

	static const struct irc_conf_listener listener = {
		.host = "irc.test.net", .port = "6667"
	};

	const bool valid = irc_conf_listener_add(&conf, &listener, &code);

	assert_true(valid);
	assert_int_equal(code, IRC_CONF_STATUS_OK);
}

static void reject_too_large_port(void **state)
{
	(void)state;

	struct irc_conf conf = {};

	enum irc_conf_status_code code;

	static const struct irc_conf_listener listener = {
		.host = "irc.test.net", .port = "99999"
	};

	const bool valid = irc_conf_listener_add(&conf, &listener, &code);

	assert_false(valid);
	assert_int_equal(code, IRC_CONF_INVALID_PORT_RANGE);
}

static void reject_neg_port_num(void **state)
{
	(void)state;

	struct irc_conf conf = {};

	enum irc_conf_status_code code;

	static const struct irc_conf_listener listener = {
		.host = "irc.test.net", .port = "-1222"
	};

	const bool valid = irc_conf_listener_add(&conf, &listener, &code);

	assert_false(valid);
	assert_int_equal(code, IRC_CONF_INVALID_PORT_RANGE);
}

static void reject_alpha_port(void **state)
{
	(void)state;

	struct irc_conf conf = {};

	enum irc_conf_status_code code;

	static const struct irc_conf_listener listener = {
		.host = "irc.test.net", .port = "abdbd"
	};

	const bool valid = irc_conf_listener_add(&conf, &listener, &code);

	assert_false(valid);
	assert_int_equal(code, IRC_CONF_INVALID_PORT_RANGE);
}

static void reject_mixed_whitespace_alpha_port(void **state)
{
	(void)state;

	struct irc_conf conf = {};

	enum irc_conf_status_code code;

	static const struct irc_conf_listener listener = {
		.host = "irc.test.net", .port = "na 33"
	};

	const bool valid = irc_conf_listener_add(&conf, &listener, &code);

	assert_false(valid);
	assert_int_equal(code, IRC_CONF_INVALID_PORT_RANGE);
}

static void reject_all_whitespace_port(void **state)
{
	(void)state;

	struct irc_conf conf = {};

	enum irc_conf_status_code code;

	static const struct irc_conf_listener listener = {
		.host = "irc.test.net", .port = "     "
	};

	const bool valid = irc_conf_listener_add(&conf, &listener, &code);

	assert_false(valid);
	assert_int_equal(code, IRC_CONF_INVALID_PORT_RANGE);
}

int main(void)
{
	static const struct CMUnitTest tests[] = {
		[0] = cmocka_unit_test(accept_normal_port_num),
		[1] = cmocka_unit_test(reject_too_large_port),
		[2] = cmocka_unit_test(reject_neg_port_num),
		[3] = cmocka_unit_test(reject_alpha_port),
		[4] = cmocka_unit_test(reject_mixed_whitespace_alpha_port),
		[5] = cmocka_unit_test(reject_all_whitespace_port)
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
