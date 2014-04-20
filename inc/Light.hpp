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


#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/time_zone_base.hpp>


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
    void    getTime();
    int     getUTC(int hour, int timezone);
    double  meanLong(double x);
    double  GetPositionSun(int year, int month, int day, int hour, int min);
private:
    Ogre::SceneManager* scene;
};

#endif
