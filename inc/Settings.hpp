#include	<string>
#include	<map>
#include	"boost/program_options.hpp" 
#include	"OgreRenderer.hpp"

class Settings
{
public:
		Settings();
		~Settings();

  static void		init(int ac, char **av);
  static void		setRender(OgreRenderer *_render);
  static void		setString(std::string, std::string);
  static std::string	getString(std::string);
  static void		setValue(std::string, int);
  static int		getValue(std::string);
  static void		setFlag(std::string, bool);
  static bool		getFlag(std::string);
  static bool		isAlive();

private:
  static OgreRenderer	*render;
  static boost::program_options::variables_map options; 
  static std::map<std::string, std::string>	strings;
  static std::map<std::string, int>		values;
  static std::map<std::string, bool>		flags;
  static bool					alive;
};

// extern Settings g_settings;
