#include	"Core.hpp"
#include	"Logger.hpp"

int   main(int argc, char** argv)
{
    Core core(argc, argv);

	Logger::init();
	Logger::log("Ceci est une erreur", Logger::error);
	Logger::log("Ceci est une debug", Logger::debug);
	Logger::log("Ceci est une warning", Logger::warning);
	Logger::log("Ceci est une fatal", Logger::fatal);

	Logger::create_tag("YannVaillant");

	Logger::log("Ceci est une erreur", Logger::error, "YannVaillant");
	Logger::log_tag("Ceci est une info", "YannVaillant");
	Logger::log_tag("Ceci est une info", "FilleAPoile");


	core.init();
    core.start();

    return 0;
}