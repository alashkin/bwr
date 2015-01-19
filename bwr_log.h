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

#pragma once

#pragma warning(push)
#pragma warning(disable: 4996) //4996 for _CRT_SECURE_NO_WARNINGS equivalent
#pragma warning(disable: 4503)

#include <boost/log/common.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

#pragma warning(pop)

#define _LOG_INIT(filename, lvl, auto_flush, console_logging) bwr::InitLogging(filename, bwr::lvl, auto_flush, console_logging);
#define _LOG(lvl) BOOST_LOG_SEV(bwr::global_logger, bwr::severity_level::lvl)
#define _LOG_PREPARE_REC() boost::log::record __log_rec;boost::log::record_ostream __log_strm; 
#define _LOG_OPEN_REC(lvl) __log_rec = bwr::global_logger.open_record(boost::log::keywords::severity = bwr::lvl); if( __log_rec) __log_strm.attach_record( __log_rec);  
#define _LOG_ADD_TO_REC() if( __log_rec) __log_strm
#define _LOG_CLOSE_REC() if( __log_rec) { __log_strm.flush(); bwr::global_logger.push_record(boost::move(__log_rec)); };
#define _LOG_REC() __log_strm
#define _LOG_REC_TYPE boost::log::record_ostream

namespace bwr
{
	enum severity_level
	{
		debug,
		info,
		warning,
		error,
		fatal
	};

	void InitLogging(const std::wstring& filename, severity_level lvl, bool auto_flush, bool console_logging);

	severity_level GetLevelFromStr(const std::wstring& str);
	std::wstring GetStrFromLevel(severity_level lvl);

	extern boost::log::sources::wseverity_logger< severity_level > global_logger;
}

