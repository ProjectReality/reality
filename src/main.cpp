#include	"Core.hpp"
#include	"Logger.hpp"

int   main(int argc, char** argv)
{
		Core core(argc, argv);

		Logger::init();
		Logger::log("Initialisation of Reality");
		Logger::log("Log Ogre is enable : %b", Logger::ogreEnable);
		Logger::log("Log OVR is enable : %b", Logger::OVREnable);

		Logger::log_tag("Main msg", "This is a simple message tagged by the main");
		Logger::log_tag_sev("Main msg", Logger::fatal, "This is a fatal message send by the main : %s", "vive Reality");
		Logger::log_tag_sev("Main msg", Logger::fatal, "This is a fatal message send by the main : %s", "vive Reality");
		Logger::log_tag_sev("Main msg", Logger::fatal, "This is a fatal message send by the main : %s", "vive Reality");

		core.init();
		core.start();
}