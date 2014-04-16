#ifndef _R_OGRERENDERER_HPP_
#define _R_OGRERENDERER_HPP_

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

#include  <OISEvents.h>
#include  <OISInputManager.h>
#include  <OISKeyboard.h>
#include  <OISMouse.h>


#include  "Oculus.hpp"
#include  "IRenderer.hpp"

struct        Elem
{
    std::string     name;
    Ogre::Entity*     ent;
    Ogre::SceneNode*    node;
};

 /**
 * @brief 3D Engine
 * @details Class manage everything in the 3D Engine
 *
 */
 class       OgreRenderer: public IRenderer
 {
 public:

    OgreRenderer(double camsize[2], VirtualOculus *rift);
    ~OgreRenderer();

    /**
    * @brief Init everything for Ogre
    * @details Init everything for Ogre
    */
    void init_all();

    /**
    * @brief Init the two cameras
    * @details Add two camera to the scene, on for each eye with
    * settings value from the Oculus Rift SDK
    */
    void init_cameras();

    /**
    * @brief Init two viewports
    * @details Init two viewports with a visibility mask for each.
    */
    void init_viewports();

    /**
    * @brief
    * @details
    */
    void init_compositor();


    void init_background_camera();


    /**
    * @brief Create a new entity
    * @details add a new entity to the scene
    *
    * @param _name Name of the entity
    * @param _mesh Name of the mesh file to load
    */
    void        createEntity(std::string _name, std::string _mesh);

    /**
    * @brief Move an entity in a direction
    * @details Move in any direction (not reposition)
    *
    * @param _name Name of the entity to move
    * @param x add or reduce the X position in space
    * @param y add or reduce the Y position in space
    * @param z add or reduce the Z position in space
    */
    void        moveEntity(std::string _name, float x, float y, float z);

    /**
    * @brief Set a new position for an entity
    * @details New coord in space
    *
    * @param _name Name of the entity
    * @param x x
    * @param y y
    * @param z z
    */
    void setPosEntity(std::string _name, float x, float y, float z);

    /**
    * @brief Rotate an entity in a direction
    * @details Rotate an entity in a direction
    *
    * @param _name Name of the entity to rotate
    * @param yaw [description]
    * @param pitch [description]
    * @param roll [description]
    */
    void        rotateEntity(std::string _name, float yaw, float pitch, float roll);

    //! Create a new light in the scene
    void        createLight(std::string _name);

    //! Change the position of the light
    void        moveLight(std::string _name, float x, float y, float z);

    //! Load a new mesh in memory
    void        loadMesh(std::string _name, std::string _file);

    //! Load a new texture in memory
    void        loadTexture(std::string _name, std::string _file);

    //! Update the frame of the camera
    void        loadCam(cv::Mat left, cv::Mat right);

    //! Produce one frame
    void        render();

    //! If windows still open
    bool        isAlive();

    //! Convert an Opencv Mat into a Ogre Image
    Ogre::Image* MatToImage(cv::Mat in);

    //! Convert an OVR::Matrix4f to Ogre::Matrix4
    Ogre::Matrix4 OVRMat4toOgreMat4(OVR::Matrix4f matrix);

    void setFrameSize(double size[2]);

    bool  getShutDown();

private:
    VirtualOculus               *rift;
    double                      cam_frame_size[2];

    Ogre::Root*                 ogre;
    Ogre::SceneManager*         scene;
    Ogre::RenderWindow*         window;

    Ogre::Viewport*             viewports[2];
    Ogre::Camera*               cameras[2];
    Ogre::CompositorInstance    *compos[2];
    Ogre::MaterialPtr           mats[2];
    Ogre::Rectangle2D*          rects[2];
    Ogre::TexturePtr            tex[2];

    std::map<std::string, Elem> entities;
};

#endif //_R_OGRERENDERER_HPP_
