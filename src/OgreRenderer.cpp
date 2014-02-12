#include	<iostream>

#include	"OgreRenderer.hpp"



OgreRenderer::OgreRenderer(double camsize[2])
{
	//TODO : find better way
	cam_frame_size[0] = camsize[0];
	cam_frame_size[1] = camsize[1];
  
	// -- Ogre init --

  ogre = new Ogre::Root("plugins.cfg");

  // Use the config file
  Ogre::ConfigFile	cfgFile;
  cfgFile.load("resources.cfg");

  // Go through all sections & settings in the file
  Ogre::ConfigFile::SectionIterator seci = cfgFile.getSectionIterator();
 
  Ogre::String secName, typeName, archName;
  while (seci.hasMoreElements())
    {
      secName = seci.peekNextKey();
      Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
      Ogre::ConfigFile::SettingsMultiMap::iterator i;
      for (i = settings->begin(); i != settings->end(); ++i)
        {
	  typeName = i->first;
	  archName = i->second;
	  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
  // Either restore the configuration from the file, or show dialog to set new options
  if(!(ogre->restoreConfig() || ogre->showConfigDialog())) // If both fail
    {
      alive = false;
      return;
    }

  window = ogre->initialise(true, "Reality Rendering Window");
  

  alive = true; // Ogre is now running


  // -- Scene init --

  scene = ogre->createSceneManager(Ogre::ST_GENERIC);

  camera = scene->createCamera("Center"); // Currently only 1 cam

  viewport =  window->addViewport(camera);
  viewport->setBackgroundColour(Ogre::ColourValue(1.0,0.0,1.0));
 

  camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()));
  camera->setNearClipDistance(5);
  //  camera->setFarClipDistance(1000);

  // Initial Camera position
  camera->setPosition(Ogre::Vector3(0,0,300));
  camera->lookAt(Ogre::Vector3(0,0,-300));

  scene->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

  rightRect = new Ogre::Rectangle2D(true);
  rightRect->setCorners(-1.0, 1.0, 1.0, -1.0);
  rightRect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
  // rightRect->setMaterial("rightCap");  

  Ogre::AxisAlignedBox aabInf;
  aabInf.setInfinite();
  rightRect->setBoundingBox(aabInf);

  scene->getRootSceneNode()->attachObject(rightRect);

  // Init Right
  rightTex = Ogre::TextureManager::getSingleton().createManual("backgrounnndd2", 
										 Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
										 Ogre::TEX_TYPE_2D,      
										 cam_frame_size[0], cam_frame_size[1],			//VIDEO SIZE
										 0,                
										 Ogre::PF_B8G8R8,     // PIXEL FORMAT OF OPENCV FRAME
										 Ogre::TU_DEFAULT);

  rightMat = Ogre::MaterialManager::getSingleton().create("rightCap2", // name
									     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

  rightMat->getTechnique(0)->getPass(0)->createTextureUnitState("backgrounnndd2");
  rightMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
  rightMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
  rightMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
  rightMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
}

OgreRenderer::~OgreRenderer()
{
  delete ogre; // Apparently handle deletes for most ogre-related things.
}

// General scene functions
void	OgreRenderer::createEntity(std::string _name, std::string _mesh)
{
  if ((_name.length() < 1))
    std::cout << "createEntity: invalid name" << std::endl;     
  else if (entities[_name].name != _name)
    {
      entities[_name].name = _name;
      if ((_mesh == "") || (&_mesh == NULL)) 
	entities[_name].ent = scene->createEntity(_name, "knot.mesh"); // Ogre::SceneManager::PT_CUBE
      else
	entities[_name].ent = scene->createEntity(_name, _mesh);
      entities[_name].node = scene->getRootSceneNode()->createChildSceneNode();
      entities[_name].node->attachObject(entities[_name].ent);
    }
  else
    std::cout << "createEntity: An entity already exists with this name" << std::endl; 
}

void	OgreRenderer::createLight(std::string _name)
{

}

void	OgreRenderer::moveEntity(std::string _name, float x, float y, float z)
{
  
}

void	OgreRenderer::rotateEntity(std::string _name, float yaw, float pitch, float roll)
{
  entities[_name].node->yaw(Ogre::Degree(yaw));
  entities[_name].node->pitch(Ogre::Degree(pitch));
  entities[_name].node->roll(Ogre::Degree(roll));
}

void	OgreRenderer::moveLight(std::string _name, float x, float y, float z)
{

}

// Functions related to assets loading
void	OgreRenderer::loadMesh(std::string _name, std::string _file)
{

}

void	OgreRenderer::loadTexture(std::string _name, std::string _file)
{
  // Not implemented
}

bool	OgreRenderer::isAlive()
{
  return alive;
}

void	OgreRenderer::render()
{
  Ogre::WindowEventUtilities::messagePump();
  if (ogre->renderOneFrame() && !window->isClosed()) // If frame rendering succeed and window is still open
    alive = true;
  else
    alive = false;
}

Ogre::Image* OgreRenderer::MatToImage(cv::Mat in) {
	Ogre::Image* out = new Ogre::Image();

	Ogre::MemoryDataStream* rightStream = new Ogre::MemoryDataStream((void*) in.data, cam_frame_size[0] * cam_frame_size[1] * 3, false, false);
	Ogre::DataStreamPtr ptr2(rightStream);
	ptr2->seek(0);
	out->loadRawData(ptr2, cam_frame_size[0], cam_frame_size[1], Ogre::PF_B8G8R8);
	return out;
}

void	OgreRenderer::loadCam(cv::Mat left, cv::Mat right)
{
	Ogre::Image* imageLeft = MatToImage(left);

	if (imageLeft->getSize() >= 0) {
      rightTex->unload();
	  rightTex->loadImage(*imageLeft);
    } else {
		std::cerr << "OgreRenderer::loadCam() - Couldn't load image cause empty" << std::endl;
    }
  rightRect->setMaterial("rightCap2");
  imageLeft->freeMemory();
}

void OgreRenderer::setFrameSize(double size[2]) {
	cam_frame_size[0] = size[0];
	cam_frame_size[1] = size[1];
}