#include	"Core.hpp"

int   main(int argc, char** argv)
{
    Core core(argc, argv);

	core.init();
    core.start();

    return 0;
}

