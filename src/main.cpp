#include	"Core.hpp"
#include	"Logger.hpp"

int   main(int argc, char** argv)
{
    Core core(argc, argv);

	Logger::log("poile aux moustaches");

	core.init();
    core.start();

    return 0;
}

