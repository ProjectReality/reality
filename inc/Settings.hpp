#include	<string>
#include	<map>
#include "boost/program_options.hpp" 

#include "OgreRenderer.hpp"

class Settings
{
public:
		Settings();
		~Settings();

  void		init(int ac, char **av);
  void		setRender(OgreRenderer *_render);
  void		setString(std::string, std::string);
  std::string	getString(std::string);
  void		setValue(std::string, int);
  int		getValue(std::string);
  void		setFlag(std::string, bool);
  bool		getFlag(std::string);
  bool		isAlive();

private:
  OgreRenderer	*render;
  boost::program_options::variables_map options; 
  std::map<std::string, std::string>	strings;
  std::map<std::string, int>		values;
  std::map<std::string, bool>		flags;
  bool					alive;
};

extern Settings g_settings;
