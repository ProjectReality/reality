#include	"Core.hpp"
#include	"Logger.hpp"

int   main(int argc, char** argv)
{
	try {
		Core core(argc, argv);

		Logger::init();
		Logger::log("Initialisation of Reality");
		Logger::log("Log Ogre is enable : %b", Logger::ogreEnable);
		Logger::log("Log OVR is enable : %b", Logger::OVREnable);

		core.init();
		core.start();
	}
	catch (...) {}
    return 0;
}