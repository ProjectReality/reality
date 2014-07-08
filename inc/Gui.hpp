#ifndef __GUI_HPP___
#define __GUI_HPP___

#include  "RenderInterfaceOgre3D.h"
#include  "SystemInterfaceOgre3D.h"
#include  "OgreRenderer.hpp"

class OgreRenderer; // forward declaration I <3 CPP


/**
 * @brief The Gui class
 * @details Use librocket (HTML/CSS) to handle all the UI, also act as and listener for the render queue
 * and for event in the UI
 */
class Gui : public Ogre::RenderQueueListener, Rocket::Core::EventListener
{
public:
    /**
     * @brief Gui : Create a new UI
     * @param c : Ogre context
     */
    Gui(OgreRenderer* c);
    ~Gui();

    /**
     * @brief initRocket : Init everything (Scene, Camera, Viewport and Rocket)
     */
    void initRocket();

    /**
     * @brief createFrameListener : Create the Frame Listener and Enable de debug overlay
     */
    void createFrameListener();

    /**
     * @brief renderQueueStarted
     * @param queueGroupId
     * @param invocation
     * @param skipThisInvocation
     */
    virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);

    /**
     * @brief renderQueueEnded
     * @details Called from Ogre after a queue group is rendered.
     * @param queueGroupId
     * @param invocation
     * @param repeatThisInvocation
     */
    virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);

    /**
     * @brief ConfigureRenderSystem
     * @details Configures Ogre's rendering system for rendering Rocket.
     */
    void ConfigureRenderSystem();

    /**
     * @brief BuildProjectionMatrix
     * @details Builds an OpenGL-style orthographic projection matrix.
     * @param matrix
     */
    void BuildProjectionMatrix(Ogre::Matrix4& matrix);

    /**
     * @brief start : Rendering loop
     */
    void start();

    /**
     * @brief stop : Stop the UI rendering, hide the scene, disable the viewport.
     */
    void stop();

	/**
	* @brief shutdown : shutting down the ui for exiting program
	*/
	void shutdown();


	/**
	* @brief exiting : do we need to exit project ?
	*/
	bool isExiting();

    /**
     * @brief ProcessEvent : Callback for all event in the UI
     * @param event
     */
    void ProcessEvent(Rocket::Core::Event& event);

private:
    OgreRenderer*               OgreContext;
    Ogre::SceneManager*         mScene;
    Ogre::Viewport*             mViewport;
    Ogre::Camera*               mCamera;
    Rocket::Core::Context*      RocketContext;
    SystemInterfaceOgre3D*      ogre_system;
    RenderInterfaceOgre3D*      ogre_renderer;
    RocketFrameListener*        mFrameListener;
    Ogre::OverlaySystem* mOverlaySystem;
    bool uialive;
	bool exiting;
};


#endif //__GUI_HPP___
