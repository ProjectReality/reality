#include "Logger.hpp"

void Logger::log(std::string msg)
{
	reality_logger_mt slg;
	BOOST_LOG_SEV(slg, fatal) << msg;
}