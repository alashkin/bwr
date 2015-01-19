/*

The MIT License (MIT)

Copyright (c) 2013 Alexander Alashkin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "stdafx.h"

#include "bwr_log.h"

#include <windows.h>
#include <iostream>
#include <ios>
#include <iomanip>

#pragma warning(push)
#pragma warning(disable:4996) //4996 for _CRT_SECURE_NO_WARNINGS equivalent
#pragma warning(disable:4180)
#pragma warning(disable:4503)


#include <boost/locale/generator.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/expressions/formatter.hpp>
#include <boost/log/expressions/formatters.hpp>
#include <boost/log/expressions/formatters/format.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/attribute_value_impl.hpp>
#include <boost/phoenix.hpp>
#include <boost/algorithm/string.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;



boost::log::sources::wseverity_logger< bwr::severity_level > bwr::global_logger;

namespace
{
	std::wstring levels[] =
	{
		L"D",
		L"I",
		L"W",
		L"E",
		L"F"
	};
}

bwr::severity_level bwr::GetLevelFromStr(const std::wstring& str)
{
	for (unsigned int i = severity_level::debug; i <= severity_level::fatal; i+=1 )
	{
		if (boost::iequals(levels[i], str))
		{
			return severity_level(i);
		}
	}

	return severity_level(debug - 1);
}

std::wstring bwr::GetStrFromLevel(bwr::severity_level lvl)
{
	return levels[lvl];
}

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", bwr::severity_level)


namespace
{
	void log_formatter(logging::record_view const& rec, logging::formatting_ostream& strm)
	{
		wchar_t buf[1024];

		::SYSTEMTIME st;
		::GetLocalTime(&st);
		wsprintf(buf, L"%hu/%02hu/%02hu %hu:%02hu:%02hu.%03hu", st.wYear - 2000, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		strm << buf;

		wsprintf(buf, L" #%04X", ::GetCurrentThreadId() & 0x0000FFFF);
		strm << buf;
		strm << L" [" << bwr::GetStrFromLevel(rec[severity].get()) << L"] ";
		strm << rec[expr::message];
	}
}

void bwr::InitLogging(const std::wstring& filename, severity_level lvl, bool auto_flush, bool console_logging)
{
	if (lvl != (severity_level)(severity_level::debug - 1))
	{
		auto file_sink = logging::add_file_log(logging::keywords::file_name = filename,
											   logging::keywords::auto_flush = auto_flush);


		std::locale UTF8_loc = boost::locale::generator()("UTF-8");
		file_sink->imbue(UTF8_loc);
		file_sink->set_formatter(&log_formatter);

		if (console_logging)
		{
			auto console_sink = logging::add_console_log(std::cout);
			console_sink->imbue(UTF8_loc);
		}

		logging::core::get()->set_filter(severity >= lvl);
	}
}

