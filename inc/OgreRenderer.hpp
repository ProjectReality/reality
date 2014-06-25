#ifndef _R_OGRERENDERER_HPP_
#define _R_OGRERENDERER_HPP_

#include  <string>
#include  <iostream>
#include  <boost/current_function.hpp>


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

#include  <opencv2/opencv.hpp>

#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>

#include  "Oculus.hpp"
#include  "Object.hpp"
#include  "SystemInterfaceOgre3D.h"
#include  "RocketFrameListener.h"
#include  "Gui.hpp"

class Gui; // forward declaration I <3 CPP

 /**
 * @brief 3D Engine
 * @details Class manage everything in the 3D Engine
 *
 */
 class       OgreRenderer
 {

 public:
     /**
     * @brief OgreRenderer
     * @param camsize
     * @param rift
     */
    OgreRenderer(double camsize[2], VirtualOculus *rift);

    ~OgreRenderer();

    /**
    * @brief Init everything for Ogre Rendering
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
    * @brief Init the two compositor
    * @details Enable the Oculus Rift shader distortion
    */
    void init_compositor();


    /**
     * @brief Init the background camera
     * @details Convert the two webcame frame into texture to be applied on two rect in Ogre
     */
    void init_background_camera();

    /**
     * @brief Load a new mesh
     * @details
     *
     * @param _name name of the mesh
     * @param _file file name
     */
    void        loadMesh(std::string _name, std::string _file);

    /**
     * @brief Load a new texture
     * @details [long description]
     *
     * @param _name Name of the texture
     * @param _file File name
     */
    void        loadTexture(std::string _name, std::string _file);

    /**
     * @brief Load new image from the cameras
     * @details Get the latest frame from the camera and load it in texture
     *
     * @param left opencv image data for the left camera
     * @param right opencv image data for the right camera
     */
    void        loadCam(cv::Mat left, cv::Mat right);

    /**
     * @brief Render one frame
     * @details Render one frame and check if still need to render
     */
    void        render();

    /**
     * @brief isAlive
     * @return
     */
    bool        isAlive();

    /**
     * @brief MatToImage : Convert Opencv Mat to Ogre Image
     * @param in : The Opencv image data from the camera
     * @return An Ogre Image
     */
    Ogre::Image* MatToImage(cv::Mat in);

    /**
     * @brief OVRMat4toOgreMat4 : Convert Oculus Matrix4f to and Ogre Matrix4
     * @param matrix : Matrix from Oculus SDK
     * @return A Ogre matrix 4
     */
    Ogre::Matrix4 OVRMat4toOgreMat4(OVR::Matrix4f matrix);


    /**
     * @brief setFrameSize : Set the frame size of the camera
     * @param size : array of double width / height
     */
    void setFrameSize(double size[2]);


    /**
     * @brief getShutDown
     * @return
     */
    bool  getShutDown();

    /**
     * @brief getScene
     * @return
     */
    Ogre::SceneManager*  getScene();

    /**
     * @brief getRoot
     * @return
     */
    Ogre::Root*          getRoot();

    /**
     * @brief getWindow
     * @return
     */
    Ogre::RenderWindow*  getWindow();

    /**
     * @brief startUI : Start the Rocket UI
     */
    void startUI();

    /**
     * @brief stopUI : Stop the Rocket UI
     */
    void stopUI();

    /**
     * @brief startRealityRender : Will start the main rendering with two viewport and everything
     */
    void startRealityRender();

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

    Rocket::Core::Context* context;

    Gui* ui;

    bool alive;
    bool ShutDown;
};

#endif //_R_OGRERENDERER_HPP_
