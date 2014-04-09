#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include  <OgreRoot.h>
#include  <OgreCompositorInstance.h>
#include  <OgreCompositionPass.h>
#include  <OgreCompositionTargetPass.h>
#include  <OgreCompositorManager.h>
#include  <OgreWindowEventUtilities.h>
#include  <OgreSceneManager.h>
#include  <OgreCamera.h>
#include  <OgreEntity.h>
#include  <OgreLogManager.h>
#include  <OgreViewport.h>
#include  <OgreRenderWindow.h>
#include  <OgreConfigFile.h>

/**
 * @brief Class for light and shadow stuff in Ogre
 * @details Handle all the lights in Ogre, shadow settings, and
 * light placement
 */

 class Light
 {
 public:
    Light(Ogre::SceneManager* scene);
    ~Light();
private:
};

#endif
