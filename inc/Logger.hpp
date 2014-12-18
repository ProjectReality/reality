#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>
#include <vector>
#include <stdarg.h>
#include <algorithm>
#include <iostream>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>

#include <boost/log/sources/logger.hpp>

#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

#include    <OgreLogManager.h>
#include    <OVR.h>
//#include    <../Src/Kernel/OVR_Log.h>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

class Logger
{
public:
	enum severity_level
	{
		debug,
		info,
		warning,
		error,
		fatal
	};

	typedef boost::log::sources::severity_logger_mt<
		severity_level     // the type of the severity level
	> reality_logger_mt;

private:
	static reality_logger_mt getLogger(bool create = false);

public:
	static void init(bool active = true);
	static void create_tag(std::string tag);
	static void log_tag_sev(std::string tag, severity_level sev, std::string msg, ...);
	static void log_sev(severity_level sev, std::string msg, ...);
	static void log(std::string msg, ...);
	static void log_tag(std::string tag, std::string msg, ...);
	static void log_ogre();
	//static OVR::Log *log_OVR();
	static std::string getFullMsg(std::string msg, va_list vl);
	static void enableAll();
	static void disableAll();

public:
	static bool isEnable;
	static bool ogreEnable;
	static bool OVREnable;
	static std::vector<std::string> tag_vector;

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
