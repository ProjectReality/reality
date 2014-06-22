#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include    <string>

#include    <OgreRoot.h>
#include    <OgreCompositorInstance.h>
#include    <OgreCompositionPass.h>
#include    <OgreCompositionTargetPass.h>
#include    <OgreCompositorManager.h>
#include    <OgreWindowEventUtilities.h>
#include    <OgreSceneManager.h>
#include    <OgreCamera.h>
#include    <OgreEntity.h>
#include    <OgreLogManager.h>
#include    <OgreViewport.h>
#include    <OgreRenderWindow.h>
#include    <OgreConfigFile.h>


#include    <boost/date_time/gregorian/gregorian.hpp>
#include    <boost/date_time/time_zone_base.hpp>


#include    "Entity.hpp"

/**
 * @brief Class for light and shadow stuff in Ogre
 * @details Handle all the lights in Ogre, shadow settings, and
 * light placement
 */


class Light : public Entity
{
public:
    Light(std::string _name, Ogre::SceneManager* scene);
    ~Light();
    void    createSun();
    void    getTime();
    int     getUTC(int localhour, int timezone);
    double  getPositionSun(int year, int month, int day, int hour, int min);
    void    remove(Ogre::Light *l);
    void    remove(std::string name);
private:
    double  meanLong(double x);
private:
    Ogre::SceneManager* scene;
    std::list<Ogre::Light*> Lights;
};

#endif
