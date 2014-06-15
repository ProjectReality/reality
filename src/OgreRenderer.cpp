#include    <iostream>
#include    <boost/current_function.hpp>

#include    "OgreRenderer.hpp"
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include "RenderInterfaceOgre3D.h"
#include "RocketFrameListener.h"
#include "SystemInterfaceOgre3D.h"

OgreRenderer::OgreRenderer(double camsize[2], VirtualOculus *rift)
{
    this->rift = rift;

    cam_frame_size[0] = camsize[0];
    cam_frame_size[1] = camsize[1];

    if (cam_frame_size[0] <= 0 || cam_frame_size[1] <= 0)
    {
        std::cerr << "Fatal Error: OgreRenderer(): frame size can't be 0, bye" << std::endl;
        exit(11);
    }

    // Ogre init
    ogre = new Ogre::Root();
    mOverlaySystem = OGRE_NEW OverlaySystem();
    Ogre::LogManager::getSingleton().getDefaultLog()->setDebugOutputEnabled(false);


    if (!ogre->showConfigDialog()) //show the config window
        exit(11);

    window = ogre->initialise(true, "Rendering Window"); //must init windows before load rsrc
}

OgreRenderer::~OgreRenderer()
{
    delete ogre; // Apparently handle deletes for most ogre-related things.
}

void OgreRenderer::startRealityRender()
{
    // Scene init
    scene = ogre->createSceneManager("OctreeSceneManager");

    Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Assets");

    scene->setAmbientLight(Ogre::ColourValue(.6f, .6f, .6f));
    scene->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
    scene->setShadowColour(Ogre::ColourValue(0.6f, 0.6f, 0.6f));
    scene->setShadowFarDistance(700);

    // Ressource init
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/Oculus", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/Model/images", "FileSystem", "Assets");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/Model/mat", "FileSystem", "Assets");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/Model/mesh", "FileSystem", "Assets");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("Assets");



    Ogre::MaterialPtr matLeft = Ogre::MaterialManager::getSingleton().getByName("Ogre/Compositor/Oculus");
    Ogre::MaterialPtr matRight = matLeft->clone("Ogre/Compositor/Oculus/Right");
    Ogre::GpuProgramParametersSharedPtr pParamsLeft = matLeft->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
    Ogre::GpuProgramParametersSharedPtr pParamsRight = matRight->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

    Ogre::Vector4 hmdwarp = Ogre::Vector4(
                rift->getStereo().GetDistortionK(0),
                rift->getStereo().GetDistortionK(1),
                rift->getStereo().GetDistortionK(2),
                rift->getStereo().GetDistortionK(3));

    // param used in the oculus cg program files
    pParamsLeft->setNamedConstant("HmdWarpParam", hmdwarp);
    pParamsRight->setNamedConstant("HmdWarpParam", hmdwarp);
    pParamsLeft->setNamedConstant("LensCentre", 0.5f + (rift->getStereo().GetProjectionCenterOffset() / 2.0f));
    pParamsRight->setNamedConstant("LensCentre", 0.5f - (rift->getStereo().GetProjectionCenterOffset() / 2.0f));

    Ogre::CompositorPtr comp = Ogre::CompositorManager::getSingleton().getByName("OculusRight");
    comp->getTechnique(0)->getOutputTargetPass()->getPass(0)->setMaterialName("Ogre/Compositor/Oculus/Right");


    init_all();

    alive = true; // Ogre is now running
    ShutDown = false;
}

void OgreRenderer::init_all()
{
    init_cameras();
    init_viewports();
    init_compositor();
    init_background_camera();
}

void OgreRenderer::startUI()
{
    init_rocket();
    createFrameListener();
    ogre->startRendering();
}

void OgreRenderer::init_rocket()
{
    sceneUI = ogre->createSceneManager("OctreeSceneManager");
    cameraUI = sceneUI->createCamera("camui");
    // Position it at 500 in Z direction
    cameraUI->setPosition(Ogre::Vector3(0,0,500));
    // Look back along -Z
    cameraUI->lookAt(Ogre::Vector3(0,0,-300));
    cameraUI->setNearClipDistance(5);

    // Create one viewport, entire window
    viewportUI = window->addViewport(cameraUI);
    viewportUI->setBackgroundColour(ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    cameraUI->setAspectRatio(
                Real(viewportUI->getActualWidth()) / Real(viewportUI->getActualHeight()));

    try {
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Rocket");
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/libRocket", "FileSystem", "Rocket");
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/overlay", "FileSystem", "Rocket");
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets", "FileSystem", "Rocket");
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem", "Rocket");
        ResourceGroupManager::getSingleton().addResourceLocation("assets/OgreCore.zip", "Zip", "Rocket");
        ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    } catch( Ogre::Exception& e ) {
        std::cerr << "An exception has occured: " <<
                     e.getFullDescription().c_str() << std::endl;
    }

    // Rocket initialisation.
    ogre_renderer = new RenderInterfaceOgre3D(window->getWidth(), window->getHeight());
    Rocket::Core::SetRenderInterface(ogre_renderer);

    ogre_system = new SystemInterfaceOgre3D();
    Rocket::Core::SetSystemInterface(ogre_system);

    Rocket::Core::Initialise();
    Rocket::Controls::Initialise();

    // Load the fonts from the path to the sample directory.

    Rocket::Core::FontDatabase::LoadFontFace("assets/Fonts/Delicious-Roman.otf");
    Rocket::Core::FontDatabase::LoadFontFace("assets/Fonts/Delicious-Bold.otf");
    Rocket::Core::FontDatabase::LoadFontFace("assets/Fonts/Delicious-Italic.otf");
    Rocket::Core::FontDatabase::LoadFontFace("assets/Fonts/Delicious-BoldItalic.otf");

    context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(window->getWidth(), window->getHeight()));
    Rocket::Debugger::Initialise(context);

     //Load the mouse cursor and release the caller's reference.
//        Rocket::Core::ElementDocument* cursor = context->LoadMouseCursor("assets/libRocket/cursor.rml");
//        if (cursor)
//            cursor->RemoveReference();

    //    Rocket::Core::ElementDocument* document = context->LoadDocument("assets/libRocket/demo.rml");
    //    if (document)
    //    {
    //        document->Show();
    //        document->RemoveReference();
    //    }

    Rocket::Core::ElementDocument* document = context->LoadDocument("assets/mainUI/mainui.rml");
    if (document)
    {
        document->Show();
        document->RemoveReference();
    }

    // Add the application as a listener to Ogre's render queue so we can render during the overlay.
    sceneUI->addRenderQueueListener(this);
    sceneUI->addRenderQueueListener(mOverlaySystem);

    std::cout << "Finish init rocket" << std::endl;
}

void OgreRenderer::createFrameListener()
{
    // Create the RocketFrameListener.
    mFrameListener = new RocketFrameListener(window, cameraUI, context);

    // Show the frame stats overlay.
    mFrameListener->showDebugOverlay(true);

    ogre->addFrameListener(mFrameListener);
}

void OgreRenderer::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& ROCKET_UNUSED(skipThisInvocation))
{
    if (queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getOverlaysEnabled())
    {
        context->Update();
        ConfigureRenderSystem();
        context->Render();
    }
}

// Called from Ogre after a queue group is rendered.
void OgreRenderer::renderQueueEnded(Ogre::uint8 ROCKET_UNUSED(queueGroupId), const Ogre::String& ROCKET_UNUSED(invocation), bool& ROCKET_UNUSED(repeatThisInvocation))
{
}

// Configures Ogre's rendering system for rendering Rocket.
void OgreRenderer::ConfigureRenderSystem()
{
    Ogre::RenderSystem* render_system = Ogre::Root::getSingleton().getRenderSystem();

    // Set up the projection and view matrices.
    Ogre::Matrix4 projection_matrix;
    BuildProjectionMatrix(projection_matrix);
    render_system->_setProjectionMatrix(projection_matrix);
    render_system->_setViewMatrix(Ogre::Matrix4::IDENTITY);

    // Disable lighting, as all of Rocket's geometry is unlit.
    render_system->setLightingEnabled(false);
    // Disable depth-buffering; all of the geometry is already depth-sorted.
    render_system->_setDepthBufferParams(false, false);
    // Rocket generates anti-clockwise geometry, so enable clockwise-culling.
    render_system->_setCullingMode(Ogre::CULL_CLOCKWISE);
    // Disable fogging.
    render_system->_setFog(Ogre::FOG_NONE);
    // Enable writing to all four channels.
    render_system->_setColourBufferWriteEnabled(true, true, true, true);
    // Unbind any vertex or fragment programs bound previously by the application.
    render_system->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
    render_system->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);

    // Set texture settings to clamp along both axes.
    Ogre::TextureUnitState::UVWAddressingMode addressing_mode;
    addressing_mode.u = Ogre::TextureUnitState::TAM_CLAMP;
    addressing_mode.v = Ogre::TextureUnitState::TAM_CLAMP;
    addressing_mode.w = Ogre::TextureUnitState::TAM_CLAMP;
    render_system->_setTextureAddressingMode(0, addressing_mode);

    // Set the texture coordinates for unit 0 to be read from unit 0.
    render_system->_setTextureCoordSet(0, 0);
    // Disable texture coordinate calculation.
    render_system->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
    // Enable linear filtering; images should be rendering 1 texel == 1 pixel, so point filtering could be used
    // except in the case of scaling tiled decorators.
    render_system->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
    // Disable texture coordinate transforms.
    render_system->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
    // Reject pixels with an alpha of 0.
    render_system->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
    // Disable all texture units but the first.
    render_system->_disableTextureUnitsFrom(1);

    // Enable simple alpha blending.
    render_system->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    // Disable depth bias.
    render_system->_setDepthBias(0, 0);
}

// Builds an OpenGL-style orthographic projection matrix.
void OgreRenderer::BuildProjectionMatrix(Ogre::Matrix4& projection_matrix)
{
    float z_near = -1;
    float z_far = 1;

    projection_matrix = Ogre::Matrix4::ZERO;

    // Set up matrices.
    projection_matrix[0][0] = 2.0f / window->getWidth();
    projection_matrix[0][3]= -1.0000000f;
    projection_matrix[1][1]= -2.0f / window->getHeight();
    projection_matrix[1][3]= 1.0000000f;
    projection_matrix[2][2]= -2.0f / (z_far - z_near);
    projection_matrix[3][3]= 1.0000000f;
}

void OgreRenderer::init_cameras()
{
    for (size_t i = 0; i < 2; i++)
    {
        Ogre::Matrix4 proj = Ogre::Matrix4::IDENTITY;
        proj.setTrans(Ogre::Vector3(-rift->getStereo().GetProjectionCenterOffset() * (2 * i - 1), 0, 0));

        cameras[i] = scene->createCamera(i == 0 ? "Left" : "Right");
        cameras[i]->setNearClipDistance(rift->getStereo().GetEyeToScreenDistance());
        cameras[i]->setFarClipDistance(10000.0f); //TODO get from class virtuaocu
        cameras[i]->setPosition((i * 2 - 1) * rift->getStereo().GetIPD() * 0.5f, 0, 500);
        cameras[i]->setAspectRatio(rift->getStereo().GetAspect());
        cameras[i]->setFOVy(Ogre::Radian(rift->getStereo().GetYFOVRadians()));
        cameras[i]->lookAt(Ogre::Vector3(0, 0, -300));
        cameras[i]->setCustomProjectionMatrix(true, proj * cameras[i]->getProjectionMatrix());
    }
}

void OgreRenderer::init_viewports()
{
    Ogre::ColourValue g_defaultViewportColour(0,0,0);

    if (cameras == NULL)
    {
        std::cerr << "FatalError: " << BOOST_CURRENT_FUNCTION << ": OgreCameras not set " << std::endl;
        exit(11);
    }

    for (size_t i = 0; i < 2; i++)
    {
        viewports[i] = window->addViewport(cameras[1], i, 0.5f * i, 0, 0.5f, 1.0f);
        viewports[i]->setBackgroundColour(g_defaultViewportColour);
        viewports[i]->setVisibilityMask(i == 0 ? 0xFFFFFF00 : 0xFFFF0F0); //to hide some stuff between each viewport
    }
}

void OgreRenderer::init_compositor()
{
    if (viewports == NULL)
    {
        std::cerr << "FatalError: " << BOOST_CURRENT_FUNCTION << ": Viewports not set " << std::endl;
        exit(11);
    }
    for (size_t i = 0; i < 2; i++)
    {
        compos[i] = Ogre::CompositorManager::getSingleton().addCompositor(viewports[i], i == 0 ? "OculusLeft" : "OculusRight");
        compos[i]->setEnabled(true);
    }
}

void OgreRenderer::init_background_camera()
{
    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();

    for (size_t i = 0; i < 2; i++)
    {
        rects[i] = new Ogre::Rectangle2D(true);
        rects[i]->setCorners(-1, 1.0, 1, -1.0);
        rects[i]->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
        rects[i]->setVisibilityFlags(i == 0 ? 0xF00 : 0xF0); //hide left rect in right viewport and inverse.
        rects[i]->setBoundingBox(aabInf);
        scene->getRootSceneNode()->attachObject(rects[i]);

        tex[i] = Ogre::TextureManager::getSingleton().createManual(
                    i == 0 ? "TexLeft" : "TexRight",
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                    Ogre::TEX_TYPE_2D,
                    (Ogre::uint)cam_frame_size[0], (Ogre::uint)cam_frame_size[1], //VIDEO SIZE
                0,
                Ogre::PF_B8G8R8,
                Ogre::TU_DEFAULT);

        mats[i] = Ogre::MaterialManager::getSingleton().create(
                    i == 0 ? "MatLeft" : "MatRight",
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        mats[i]->getTechnique(0)->getPass(0)->createTextureUnitState(i == 0 ? "TexLeft" : "TexRight");
        mats[i]->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        mats[i]->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
        mats[i]->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
        mats[i]->getTechnique(0)->getPass(0)->setLightingEnabled(false);

        rects[i]->setMaterial(i == 0 ? "MatLeft" : "MatRight");
    }
}

// Functions related to assets loading
void    OgreRenderer::loadMesh(std::string _name, std::string _file)
{

}

void    OgreRenderer::loadTexture(std::string _name, std::string _file)
{
    // Not implemented
}

bool    OgreRenderer::isAlive()
{
    return alive;
}

void    OgreRenderer::render()
{
    Ogre::WindowEventUtilities::messagePump();
    if (ogre->renderOneFrame() && !window->isClosed()) // If frame rendering succeed and window is still open
        alive = true;
    else
        alive = false;
}

Ogre::Matrix4 OgreRenderer::OVRMat4toOgreMat4(OVR::Matrix4f matrix)
{
    return Ogre::Matrix4(
                matrix.M[0][0], matrix.M[0][1], matrix.M[0][2], matrix.M[0][3],
            matrix.M[1][0], matrix.M[1][1], matrix.M[1][2], matrix.M[1][3],
            matrix.M[2][0], matrix.M[2][1], matrix.M[2][2], matrix.M[2][3],
            matrix.M[3][0], matrix.M[3][1], matrix.M[3][2], matrix.M[3][3]);
}

Ogre::Image* OgreRenderer::MatToImage(cv::Mat in)
{
    Ogre::Image* out = new Ogre::Image();

    Ogre::MemoryDataStream* rightStream = new Ogre::MemoryDataStream((void*)in.data, (size_t)cam_frame_size[0] * (size_t)cam_frame_size[1] * 3, false, false);
    Ogre::DataStreamPtr ptr2(rightStream);
    ptr2->seek(0);
    out->loadRawData(ptr2, (Ogre::uint32)cam_frame_size[0], (Ogre::uint32)cam_frame_size[1], Ogre::PF_R8G8B8);
    return out;
}

void    OgreRenderer::loadCam(cv::Mat left, cv::Mat right)
{
    Ogre::Image* imageLeft = MatToImage(left);
    Ogre::Image* imageRight = MatToImage(right);

    if (imageLeft->getSize() >= 0 || imageRight->getSize() >= 0)
    {
        tex[0]->unload();
        tex[0]->loadImage(*imageLeft);
        tex[1]->unload();
        tex[1]->loadImage(*imageRight);
    }
    else
    {
        std::cerr << "OgreRenderer::loadCam() - Couldn't load image cause empty" << std::endl;
    }
    imageLeft->freeMemory();
    imageRight->freeMemory();
}

void OgreRenderer::setFrameSize(double size[2])
{
    cam_frame_size[0] = size[0];
    cam_frame_size[1] = size[1];
}

Ogre::SceneManager* OgreRenderer::getScene()
{
    return scene;
}

bool OgreRenderer::getShutDown()
{
    return ShutDown;
}
