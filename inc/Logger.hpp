#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

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
		normal,
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

#endif /*LOGGER_HPP_*/
