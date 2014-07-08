#include "Gui.hpp"

Gui::Gui(OgreRenderer *c) {
    OgreContext = c;
}

Gui::~Gui() {

}

void Gui::ProcessEvent(Rocket::Core::Event& event)
{
	string classname = event.GetCurrentElement()->GetClassNames().CString();
 
	std::cout << "Processing event of " << classname << std::endl;
	if (classname == "butlaunch") OgreContext->getGui()->stop();
	else if (classname == "butsetogre") {
        OgreContext->getRoot()->showConfigDialog();
    }
}

void Gui::initRocket()
{
    mScene = OgreContext->getRoot()->createSceneManager("OctreeSceneManager");
    mCamera = mScene->createCamera("camui");
    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,0,500));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);

    // Create one viewport, entire window
    mViewport = OgreContext->getWindow()->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
		Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

    try {
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Rocket");
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/libRocket", "FileSystem", "Rocket");
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/overlay", "FileSystem", "Rocket");
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets", "FileSystem", "Rocket");
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem", "Rocket");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/OgreCore.zip", "Zip", "Rocket");
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    } catch( Ogre::Exception& e ) {
        std::cerr << "An exception has occured: " <<
                     e.getFullDescription().c_str() << std::endl;
    }

    // Rocket initialisation.
    ogre_renderer = new RenderInterfaceOgre3D(OgreContext->getWindow()->getWidth(), OgreContext->getWindow()->getHeight());
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

    RocketContext = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(OgreContext->getWindow()->getWidth(), OgreContext->getWindow()->getHeight()));
    Rocket::Debugger::Initialise(RocketContext);

    Rocket::Core::ElementDocument* document = RocketContext->LoadDocument("assets/mainUI/mainui.rml");
    if (document)
    {
        Rocket::Core::Element* element2 = document->GetElementById("butlaunch");
        Rocket::Core::Element* element = document->GetElementById("butsetogre");
        element->AddEventListener("click", this, false);
        element2->AddEventListener("click", this, false);
        document->Show();
        document->RemoveReference();
    }

    // Add the application as a listener to Ogre's render queue so we can render during the overlay.
    mScene->addRenderQueueListener(this);
    mScene->addRenderQueueListener(mOverlaySystem);

    std::cout << "Finish init rocket" << std::endl;
}

void Gui::stop()
{
    std::cout << "Sarting to stop ui rendering" << std::endl;
    uialive = false;
    mScene->getRootSceneNode()->setVisible(false);
    //ogre->destroySceneManager(sceneUI);

    OgreContext->getWindow()->removeViewport(mViewport->getZOrder());
}

void Gui::start()
{
	mOverlaySystem = OGRE_NEW Ogre::OverlaySystem();
    initRocket();
    createFrameListener();
    uialive = true;
    while(uialive) {
        Ogre::WindowEventUtilities::messagePump();
        OgreContext->getRoot()->renderOneFrame();
        if (OgreContext->getWindow()->isClosed())
            exit(11);
		if (!mFrameListener->isRunning()) {
			std::cerr << "GUI stopped running" << std::endl;
			exit(1111); 
		}
    }
    std::cout << "out of ui rendering loop" << std::endl;
}

void Gui::createFrameListener()
{
    // Create the RocketFrameListener.
    mFrameListener = new RocketFrameListener(OgreContext->getWindow(), mCamera, RocketContext);

    // Show the frame stats overlay.
    mFrameListener->showDebugOverlay(true);

    OgreContext->getRoot()->addFrameListener(mFrameListener);
}

void Gui::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& ROCKET_UNUSED(skipThisInvocation))
{
    if (queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getOverlaysEnabled())
    {
        RocketContext->Update();
        ConfigureRenderSystem();
        RocketContext->Render();
    }
}

void Gui::renderQueueEnded(Ogre::uint8 ROCKET_UNUSED(queueGroupId), const Ogre::String& ROCKET_UNUSED(invocation), bool& ROCKET_UNUSED(repeatThisInvocation))
{
}

void Gui::ConfigureRenderSystem()
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

void Gui::BuildProjectionMatrix(Ogre::Matrix4& projection_matrix)
{
    float z_near = -1;
    float z_far = 1;

    projection_matrix = Ogre::Matrix4::ZERO;

    // Set up matrices.
    projection_matrix[0][0] = 2.0f / OgreContext->getWindow()->getWidth();
    projection_matrix[0][3]= -1.0000000f;
    projection_matrix[1][1]= -2.0f / OgreContext->getWindow()->getHeight();
    projection_matrix[1][3]= 1.0000000f;
    projection_matrix[2][2]= -2.0f / (z_far - z_near);
    projection_matrix[3][3]= 1.0000000f;
}
