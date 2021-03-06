#include	"Settings.hpp"

OgreRenderer				*Settings::render;
boost::program_options::variables_map	Settings::options; 
std::map<std::string, std::string>	Settings::strings;
std::map<std::string, int>		Settings::values;
std::map<std::string, bool>		Settings::flags;
bool					Settings::alive;


void	Settings::init(int ac, char **av)
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


  // Initial Settings
  Settings::setValue("Algo", 0);
  
}

  
bool	Settings::isAlive()
{
  return alive;
}

void	Settings::setRender(OgreRenderer *_render)
{
  render = _render;
}


void		Settings::setString(std::string param, std::string val)
{
  strings[param] = val;
}

std::string	Settings::getString(std::string param)
{
  if (strings.find(param) == strings.end())
    {
      return ("EMPTY");
    }
  else
    {
      return strings[param];
    }
}
 
void		Settings::setValue(std::string param, int val)
{
  values[param] = val;
}
 
int		Settings::getValue(std::string param)
{
  if (values.find(param) == values.end())
    {
      return 0;
    }
  else
    {
      return values[param];
    }
}
 
void		Settings::setFlag(std::string param, bool val)
{
  flags[param] = val;
}
 
bool		Settings::getFlag(std::string param)
{
    if (flags.find(param) == flags.end())
    {
      return false;
    }
    else
    {
      return flags[param];
    }

}

Settings::~Settings()
{

}
