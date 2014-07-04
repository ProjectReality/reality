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

namespace Reality {

class Light : public Entity
{
public:
    /**
     * @brief Light
     * @param _name
     * @param scene
     */
    Light(std::string _name, Ogre::SceneManager* scene);
    ~Light();

    /**
     * @brief createSun
     */
    void    createSun();

    /**
     * @brief getTime
     */
    void    getTime();

    /**
     * @brief getUTC
     * @param localhour
     * @param timezone
     * @return
     */
    int     getUTC(int localhour, int timezone);

    /**
     * @brief getPositionSun
     * @param year
     * @param month
     * @param day
     * @param hour
     * @param min
     * @return
     */
    double  getPositionSun(int year, int month, int day, int hour, int min);

    /**
     * @brief remove
     */
    void    remove();
private:
    double  meanLong(double x);
private:
    Ogre::SceneManager* scene;
    Ogre::Light* OLight;
};

}

#endif
