#ifndef _R_OGRERENDERER_HPP_
#define _R_OGRERENDERER_HPP_

#include  <string>

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

#include  "Oculus.hpp"
#include  "Object.hpp"
#include  "SystemInterfaceOgre3D.h"
#include  "RenderInterfaceOgre3D.h"
#include  "RocketFrameListener.h"


 /**
 * @brief 3D Engine
 * @details Class manage everything in the 3D Engine
 *
 */
 class       OgreRenderer : public Ogre::RenderQueueListener
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
    * @brief
    * @details
    * s
    */
    void init_rocket();

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


    /**
     * @brief [brief description]
     * @details [long description]
     */
    void init_background_camera();

    /**
     * @brief [brief description]
     * @details [long description]
     *
     * @param _name [description]
     * @param _file [description]
     */
    void        loadMesh(std::string _name, std::string _file);

    /**
     * @brief [brief description]
     * @details [long description]
     *
     * @param _name [description]
     * @param _file [description]
     */
    void        loadTexture(std::string _name, std::string _file);

    /**
     * @brief [brief description]
     * @details [long description]
     *
     * @param left [description]
     * @param right [description]
     */
    void        loadCam(cv::Mat left, cv::Mat right);

    /**
     * @brief [brief description]
     * @details [long description]
     */
    void        render();

    /**
     * @brief [brief description]
     * @details [long description]
     * @return [description]
     */
    bool        isAlive();

    /**
     * @brief [brief description]
     * @details [long description]
     *
     * @param in [description]
     * @return [description]
     */
    Ogre::Image* MatToImage(cv::Mat in);

    /**
     * @brief [brief description]
     * @details [long description]
     *
     * @param matrix [description]
     * @return [description]
     */
    Ogre::Matrix4 OVRMat4toOgreMat4(OVR::Matrix4f matrix);

    void setFrameSize(double size[2]);

    bool  getShutDown();
    Ogre::SceneManager*  getScene();



    void createFrameListener();
    /// Called from Ogre before a queue group is rendered.
    virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
    /// Called from Ogre after a queue group is rendered.
    virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);
    void ConfigureRenderSystem();
    void BuildProjectionMatrix(Ogre::Matrix4& matrix);
    void startUI();
    void startRealityRender();



private:
    VirtualOculus               *rift;
    double                      cam_frame_size[2];

    Ogre::Root*                 ogre;
    Ogre::SceneManager*         scene;
    Ogre::SceneManager*         sceneUI;
    Ogre::RenderWindow*         window;

    Ogre::Viewport*             viewports[2];
    Ogre::Viewport*             viewportUI;
    Ogre::Camera*               cameraUI;
    Ogre::Camera*               cameras[2];
    Ogre::CompositorInstance    *compos[2];
    Ogre::MaterialPtr           mats[2];
    Ogre::Rectangle2D*          rects[2];
    Ogre::TexturePtr            tex[2];

    Rocket::Core::Context* context;

    SystemInterfaceOgre3D* ogre_system;
    RenderInterfaceOgre3D* ogre_renderer;
     RocketFrameListener*  mFrameListener;

    bool alive;
    bool ShutDown;
};

#endif //_R_OGRERENDERER_HPP_
