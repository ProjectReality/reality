#include "boost/program_options.hpp" 

#include "OgreRenderer.hpp"

class Settings
{
public:
  Settings(int ac, char **av);
  ~Settings();

  void setRender(OgreRenderer *_render);
  bool isAlive();

private:
  OgreRenderer	*render;
  boost::program_options::variables_map options; 
  bool		alive;
};
