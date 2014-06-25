#ifndef __GUI_HPP___
#define __GUI_HPP___

#include  "RenderInterfaceOgre3D.h"
#include  "SystemInterfaceOgre3D.h"
#include  "OgreRenderer.hpp"

class OgreRenderer; // forward declaration I <3 CPP

class Gui : public Ogre::RenderQueueListener, Rocket::Core::EventListener
{
public:
    Gui(OgreRenderer* c);
    ~Gui();
    void initRocket();

    void createFrameListener();
    virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
    virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);
    void ConfigureRenderSystem();
    void BuildProjectionMatrix(Ogre::Matrix4& matrix);
    void start();
    void stop();
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
    OverlaySystem* mOverlaySystem;
    bool uialive;
};


#endif //__GUI_HPP___
