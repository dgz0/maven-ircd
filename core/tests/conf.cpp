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

#include <gtest/gtest.h>
#include "core/conf.h"

TEST(CoreConfiguration, AcceptsNormalPortNumber)
{
	struct irc_conf conf = {};

	enum irc_conf_status_code code;

	static const struct irc_conf_listener listener = {
		.host = "irc.test.net", .port = "6667"
	};

	const bool valid = irc_conf_listener_add(&conf, &listener, &code);

	ASSERT_TRUE(valid);
	ASSERT_EQ(code, IRC_CONF_STATUS_OK);
}

TEST(CoreConfiguration, RejectsTooLargePort)
{
	struct irc_conf conf = {};

	enum irc_conf_status_code code;

	static const struct irc_conf_listener listener = {
		.host = "irc.test.net", .port = "99999"
	};

	const bool valid = irc_conf_listener_add(&conf, &listener, &code);

	ASSERT_FALSE(valid);
	ASSERT_EQ(code, IRC_CONF_INVALID_PORT_RANGE);
}

TEST(CoreConfiguration, RejectsNegativePortNumbers)
{
	struct irc_conf conf = {};

	enum irc_conf_status_code code;

	static const struct irc_conf_listener listener = {
		.host = "irc.test.net", .port = "-1222"
	};

	const bool valid = irc_conf_listener_add(&conf, &listener, &code);

	ASSERT_FALSE(valid);
	ASSERT_EQ(code, IRC_CONF_INVALID_PORT_RANGE);
}

TEST(CoreConfiguration, RejectsAlphaCharsPort)
{
	struct irc_conf conf = {};

	enum irc_conf_status_code code;

	static const struct irc_conf_listener listener = {
		.host = "irc.test.net", .port = "abdbd"
	};

	const bool valid = irc_conf_listener_add(&conf, &listener, &code);

	ASSERT_FALSE(valid);
	ASSERT_EQ(code, IRC_CONF_INVALID_PORT_RANGE);
}

TEST(CoreConfiguration, RejectsMixedWhitespaceAlphaPort)
{
	struct irc_conf conf = {};

	enum irc_conf_status_code code;

	static const struct irc_conf_listener listener = {
		.host = "irc.test.net", .port = "na 33"
	};

	const bool valid = irc_conf_listener_add(&conf, &listener, &code);

	ASSERT_FALSE(valid);
	ASSERT_EQ(code, IRC_CONF_INVALID_PORT_RANGE);
}

TEST(CoreConfiguration, RejectsAllWhitespacePort)
{
	struct irc_conf conf = {};

	enum irc_conf_status_code code;

	static const struct irc_conf_listener listener = {
		.host = "irc.test.net", .port = "     "
	};

	const bool valid = irc_conf_listener_add(&conf, &listener, &code);

	ASSERT_FALSE(valid);
	ASSERT_EQ(code, IRC_CONF_INVALID_PORT_RANGE);
}
