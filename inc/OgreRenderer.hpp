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

#include	"Oculus.hpp"
#include	"IRenderer.hpp"

struct				Elem
{
  std::string			name;
  Ogre::Entity*			ent;
  Ogre::SceneNode*		node;
};


//! 3D Engine
/*!
Class manage everything in the 3D Engine
*/
class				OgreRenderer: public IRenderer
{
public:

	OgreRenderer(double camsize[2], VirtualOculus *rift);
				~OgreRenderer();

  //! Create a new entity in the scene
  void				createEntity(std::string _name, std::string _mesh);

  //! Change the position of an entity
  void				moveEntity(std::string _name, float x, float y, float z);

  //! Change the rotation of an entity
  void				rotateEntity(std::string _name, float yaw, float pitch, float roll);

  //! Create a new light in the scene
  void				createLight(std::string _name);

  //! Change the position of the light
  void				moveLight(std::string _name, float x, float y, float z);

  //! Load a new mesh in memory
  void				loadMesh(std::string _name, std::string _file);

  //! Load a new texture in memory
  void				loadTexture(std::string _name, std::string _file);

  //! Update the frame of the camera
  void				loadCam(cv::Mat left, cv::Mat right);

  //! Produce one frame
  void				render();

  //! If windows still open
  bool				isAlive();

  //! Convert an Opencv Mat into a Ogre Image
  Ogre::Image* MatToImage(cv::Mat in);

  //! Convert an OVR::Matrix4f to Ogre::Matrix4
  Ogre::Matrix4 OVRMat4toOgreMat4(OVR::Matrix4f matrix);

  void setFrameSize(double size[2]);

private:
	VirtualOculus *rift;
	double	cam_frame_size[2];

  Ogre::Root*			ogre;
  Ogre::SceneManager*	scene;
  Ogre::RenderWindow*	window;

  Ogre::Viewport*		viewports[2];
  Ogre::Camera*			cameras[2];
  Ogre::CompositorInstance *compos[2];
  Ogre::MaterialPtr		mats[2];
  Ogre::Rectangle2D*	rects[2];
  Ogre::TexturePtr		tex[2];

  std::map<std::string, Elem>	entities;
};

#endif // _R_OGRERENDERER_HPP_
