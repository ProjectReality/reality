#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>

#include <boost/log/sources/logger.hpp>

#include <boost/log/support/date_time.hpp>

#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

class Logger
{
public:
	static const unsigned short	LOG_TIMED = 1;
	static const unsigned short	LOG_LINEID = 2;
	static const unsigned short	LOG_SCOPED = 4;
	static const unsigned short	LOG_TAG = 8;

	static enum severity_level
	{
		debug,
		info,
		warning,
		error,
		fatal
	};

	typedef boost::log::sources::severity_channel_logger_mt<
		severity_level,     // the type of the severity level
		std::string         // the type of the channel name
	> reality_logger_mt;

//public:
//	static reality_logger_mt reality_logger;

public:
	static void log(std::string msg);
};

// The formatting logic for the severity level
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
	std::basic_ostream< CharT, TraitsT >& strm, Logger::severity_level lvl)
{
	static const char* const str[] =
	{
		"debug",
		"info",
		"warning",
		"error",
		"fatal"
	};
	if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
		strm << str[lvl];
	else
		strm << static_cast< int >(lvl);
	return strm;
}



#endif /*LOGGER_HPP_*/
