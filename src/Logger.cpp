#include "Logger.hpp"

bool Logger::isEnable = true;
bool Logger::ogreEnable = false;
bool Logger::OVREnable = false;
std::vector<std::string> Logger::tag_vector;

void Logger::init(bool active)
{
	isEnable = active;
	if (isEnable)
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

void Logger::log_tag_sev(std::string tag, severity_level sev, std::string msg, ...)
{
	if (isEnable)
	{
		if (tag != "Global" && find(tag_vector.begin(), tag_vector.end(), tag) == tag_vector.end())
		{
			create_tag(tag);
		}
		reality_logger_mt reality_logger = getLogger();
		reality_logger.add_attribute("Tag", attrs::constant< std::string >(tag));
		va_list vl;
		va_start(vl, msg);
		msg = getFullMsg(msg, vl);
		va_end(vl);
		BOOST_LOG_SEV(reality_logger, sev) << msg;
	}
}

void Logger::log_sev(severity_level sev, std::string msg, ...)
{
	va_list vl;
	va_start(vl, msg);
	msg = getFullMsg(msg, vl);
	va_end(vl);
	log_tag_sev("Global", sev, msg);
}

void Logger::log(std::string msg, ...)
{
	va_list vl;
	va_start(vl, msg);
	msg = getFullMsg(msg, vl);
	va_end(vl);
	log_tag_sev("Global", info, msg);
}

void Logger::create_tag(std::string tag)
{
	if (isEnable)
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

void Logger::log_tag(std::string tag, std::string msg, ...)
{
	if (isEnable)
	{
		if (tag != "Global" && find(tag_vector.begin(), tag_vector.end(), tag) == tag_vector.end())
		{
			create_tag(tag);
		}
		reality_logger_mt reality_logger = getLogger();
		reality_logger.add_attribute("Tag", attrs::constant< std::string >(tag));
		va_list vl;
		va_start(vl, msg);
		msg = getFullMsg(msg, vl);
		va_end(vl);
		BOOST_LOG_SEV(reality_logger, info) << msg;
	}
}

void Logger::log_ogre()
{
	if (!ogreEnable)
	{
		Ogre::LogManager* logMgr = OGRE_NEW Ogre::LogManager;
		logMgr->createLog("DefaultLog", true, false, false);
	}
	else
	{
		Ogre::LogManager* logMgr = OGRE_NEW Ogre::LogManager;
		logMgr->createLog("DefaultLog", true, true, false);
	}
}

OVR::Log *Logger::log_OVR()
{
	if (!OVREnable)
	{
		OVR::Log *OVRLogger = OVR::Log::ConfigureDefaultLog(OVR::LogMask_None);
		OVRLogger->DefaultLogOutput("", 0);
		return (OVRLogger);
	}
	else
	{
		OVR::Log *OVRLogger = OVR::Log::ConfigureDefaultLog(OVR::LogMask_All);
		OVRLogger->DefaultLogOutput("", 0);
		return (OVRLogger);
	}
}

std::string Logger::getFullMsg(std::string msg, va_list vl)
{
	int pos = msg.find_first_of('%');
	while (pos != std::string::npos){
		char c = msg.at(pos + 1);
		std::ostringstream os;
		switch (c)
		{
		case 's':
			msg.replace(pos, 2, va_arg(vl, char *));
			break;
		case 'c':
			os << va_arg(vl, char);
			msg.replace(pos, 2, os.str());
			break;
		case 'f':
			os << va_arg(vl, float);
			msg.replace(pos, 2, os.str());
			break;
		case 'd':
			os << va_arg(vl, double);
			msg.replace(pos, 2, os.str());
			break;
		case 'i':
			os << va_arg(vl, int);
			msg.replace(pos, 2, os.str());
			break;
		case 'b':
			os << va_arg(vl, bool);
			msg.replace(pos, 2, os.str());
			break;
		}
		pos = msg.find_first_of("%");
	}
	return (msg);
}