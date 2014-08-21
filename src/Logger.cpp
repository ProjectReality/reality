#include "Logger.hpp"

void Logger::init(bool active)
{
	if (RealityLog::isEnable)
	{
		logging::add_file_log("log/All.log",
			keywords::format = expr::stream
			<< " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S.%f")
			<< "] [" << expr::attr< std::string >("Tag")
			<< "] <" << expr::attr< severity_level >("Severity")
			<< "> " << expr::message
			);
		logging::add_file_log("log/Global.log",
			keywords::filter = expr::attr< std::string >("Tag") == "Global",
			keywords::format = expr::stream
			<< " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S.%f")
			<< "] [" << expr::attr< std::string >("Tag")
			<< "] <" << expr::attr< severity_level >("Severity")
			<< "> " << expr::message
			);
		logging::add_file_log("log/Debug.log",
			keywords::filter = expr::attr< severity_level >("Severity") == debug,
			keywords::format = expr::stream
			<< " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S.%f")
			<< "] [" << expr::attr< std::string >("Tag")
			<< "] " << expr::message
			);
		logging::add_file_log("log/Warning.log",
			keywords::filter = expr::attr< severity_level >("Severity") == warning,
			keywords::format = expr::stream
			<< " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S.%f")
			<< "] [" << expr::attr< std::string >("Tag")
			<< "] " << expr::message
			);
		logging::add_file_log("log/Error.log",
			keywords::filter = expr::attr< severity_level >("Severity") == error,
			keywords::format = expr::stream
			<< " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S.%f")
			<< "] [" << expr::attr< std::string >("Tag")
			<< "] " << expr::message
			);
		logging::add_file_log("log/Fatal.log",
			keywords::filter = expr::attr< severity_level >("Severity") == fatal,
			keywords::format = expr::stream
			<< " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S.%f")
			<< "] [" << expr::attr< std::string >("Tag")
			<< "] " << expr::message
			);
		getLogger(true);
	}
}

Logger::reality_logger_mt Logger::getLogger(bool create)
{
	static reality_logger_mt reality_logger;

	if (create)
	{
		reality_logger.add_attribute("Uptime", attrs::timer());
	}

	return (reality_logger);
}

void Logger::log(std::string msg, severity_level sev, std::string tag)
{
	if (RealityLog::isEnable)
	{
		reality_logger_mt reality_logger = getLogger();
		reality_logger.add_attribute("Tag", attrs::constant< std::string >(tag));
		BOOST_LOG_SEV(reality_logger, sev) << msg;
	}
}

void Logger::create_tag(std::string tag)
{
	if (RealityLog::isEnable)
	{
		logging::add_file_log("log/tagged/" + tag + ".log",
			keywords::filter = expr::attr< std::string >("Tag") == tag,
			keywords::format = expr::stream
			<< " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S.%f")
			//<< "] [" << attrs::constant< std::string >("Tag")
			<< "] <" << expr::attr< severity_level >("Severity")
			<< "> " << expr::message
			);
	}
}

void Logger::log_tag(std::string msg, std::string tag)
{
	if (RealityLog::isEnable)
	{
		reality_logger_mt reality_logger = getLogger();
		reality_logger.add_attribute("Tag", attrs::constant< std::string >(tag));
		BOOST_LOG_SEV(reality_logger, info) << msg;
	}
}