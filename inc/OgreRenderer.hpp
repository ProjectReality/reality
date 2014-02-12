
#ifndef		_R_OGRERENDERER_HPP_
#define		_R_OGRERENDERER_HPP_

#include	<OgreCamera.h>
#include	<OgreEntity.h>
#include	<OgreLogManager.h>
#include	<OgreRoot.h>
#include	<OgreViewport.h>
#include	<OgreSceneManager.h>
#include	<OgreRenderWindow.h>
#include	<OgreConfigFile.h>

#include	<OISEvents.h>
#include	<OISInputManager.h>
#include	<OISKeyboard.h>
#include	<OISMouse.h>

#include	<SdkTrays.h>
#include	<SdkCameraMan.h>

#include	"IRenderer.hpp"

struct				Elem
{
  std::string			name;
  Ogre::Entity*			ent;
  Ogre::SceneNode*		node;
};

class				OgreRenderer: public IRenderer
{
public:

				OgreRenderer();
				~OgreRenderer();

  // General scene functions
  void				createEntity(std::string _name, std::string _mesh);
  void				createLight(std::string _name);
  void				moveEntity(std::string _name, float x, float y, float z);
  void				rotateEntity(std::string _name, float yaw, float pitch, float roll);
  void				moveLight(std::string _name, float x, float y, float z);

  // Functions related to assets loading
  void				loadMesh(std::string _name, std::string _file);
  void				loadTexture(std::string _name, std::string _file);

  // Other functions
  void				loadCam(cv::Mat left, cv::Mat right);
  void				render();
  bool				isAlive();

private:

  Ogre::Root*			ogre;
  Ogre::Camera*			camera;
  Ogre::SceneManager*		scene;
  Ogre::RenderWindow*		window;
  Ogre::Viewport*		viewport;

  std::map<std::string, Elem>	entities;
  //  std::map<std::string, Elem>	meshes;

  cv::Mat			leftCam;
  Ogre::MaterialPtr		leftMat;
  Ogre::Rectangle2D*		leftRect;

  cv::Mat			rightCam;
  Ogre::MaterialPtr		rightMat;
  Ogre::Rectangle2D*		rightRect;
};

#endif // _R_OGRERENDERER_HPP_
