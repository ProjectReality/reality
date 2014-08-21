#include "Logger.hpp"

void Logger::log(std::string msg)
{
	logging::add_file_log("log/Global.log",
		keywords::format = expr::stream
		<< " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S.%f")
		<< "] [" << expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
		<< "] <" << expr::attr< severity_level >("Severity")
		<< "> " << expr::message
		/*
		keywords::format = expr::format("%1% [%2%] [%3%] <%4%> %5%")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
		% expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S")
		% expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
		% expr::attr< severity_level >("Severity")
		% expr::message
		*/
		);

	logging::add_common_attributes();
	logging::core::get()->add_thread_attribute("Scope", attrs::named_scope());


	reality_logger_mt slg; 
	slg.add_attribute("Uptime", attrs::timer());
	BOOST_LOG_SEV(slg, fatal) << msg;
	BOOST_LOG_SEV(slg, fatal) << msg;
	BOOST_LOG_SEV(slg, fatal) << msg;
}