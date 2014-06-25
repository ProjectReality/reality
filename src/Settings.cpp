#include	"Settings.hpp"


Settings::Settings(int ac, char **av)
{
  alive = true;

  try 
    { 
      boost::program_options::options_description desc("Options"); 
      desc.add_options() 
	("help", "Print help messages") 
	("add", "additional options") 
	("like", "this"); 
 

      try 
	{ 
	  boost::program_options::store(boost::program_options::parse_command_line(ac, av, desc),
					options);
 
	  if (options.count("help")) 
	    { 
	      std::cout << "Basic Command Line Parameter App" << std::endl 
			<< desc << std::endl; 
	      alive = false; 
	    } 

	  boost::program_options::notify(options); 
	} 
      catch(boost::program_options::error& e)
	{ 
	  std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
	  std::cerr << desc << std::endl; 
	  alive = false;
	} 
  
    } 
  catch(std::exception& e) 
    { 
      std::cerr << "Unhandled Exception reached the top of main: " 
		<< e.what() << ", application will now exit" << std::endl; 
      alive = false; 
    } 

}

bool	Settings::isAlive()
{
  return alive;
}

void	Settings::setRender(OgreRenderer *_render)
{
  render = _render;
}
