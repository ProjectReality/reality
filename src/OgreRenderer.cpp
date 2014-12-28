#include "OgreRenderer.hpp"

OgreRenderer::OgreRenderer(double camsize[2], IGlasses *rift)
{
    this->rift = rift;

    cam_frame_size[0] = camsize[0];
    cam_frame_size[1] = camsize[1];

    if (cam_frame_size[0] <= 0 || cam_frame_size[1] <= 0)
    {
        std::cerr << "Fatal Error: OgreRenderer(): frame size can't be 0, bye" << std::endl;
        exit(11);
    }
	Logger::log_ogre();
    // Ogre init
	ogre = new Ogre::Root();
	ui = new Gui(this);

    if (!ogre->restoreConfig()){
        ogre->showConfigDialog();
    }

    window = ogre->initialise(true, "Rendering Window"); //must init windows before load rsrc
}

OgreRenderer::~OgreRenderer()
{
	ui->shutdown();
	Ogre::Root::getSingleton().queueEndRendering();
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
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/Oculus", "FileSystem", "Assets");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/Model/images", "FileSystem", "Assets");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/Model/mat", "FileSystem", "Assets");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("assets/Model/mesh", "FileSystem", "Assets");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("Assets");


    Ogre::MaterialPtr matLeft = Ogre::MaterialManager::getSingleton().getByName("Ogre/Compositor/Oculus");
    Ogre::MaterialPtr matRight = matLeft->clone("Ogre/Compositor/Oculus/Right");
    Ogre::GpuProgramParametersSharedPtr pParamsLeft = matLeft->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
    Ogre::GpuProgramParametersSharedPtr pParamsRight = matRight->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
/*
    Ogre::Vector4 hmdwarp = Ogre::Vector4(
                rift->getStereo().GetDistortionK(0),
                rift->getStereo().GetDistortionK(1),
                rift->getStereo().GetDistortionK(2),
                rift->getStereo().GetDistortionK(3));*/

    //// param used in the oculus cg program files
    //pParamsLeft->setNamedConstant("HmdWarpParam", hmdwarp);
    //pParamsRight->setNamedConstant("HmdWarpParam", hmdwarp);
    //pParamsLeft->setNamedConstant("LensCentre", 0.5f + (rift->getStereo().GetProjectionCenterOffset() / 50.0f));
    //pParamsRight->setNamedConstant("LensCentre", 0.5f - (rift->getStereo().GetProjectionCenterOffset() / 50.0f));

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

void OgreRenderer::init_cameras()
{
	aruco::CameraParameters camParams;
	camParams.readFromXMLFile("Data/camera.yml");

    for (size_t i = 0; i < 2; i++)
	{

		cameras[i] = scene->createCamera(i == 0 ? "Left" : "Right");
		//cameras[i]->setNearClipDistance(rift->getStereo().GetEyeToScreenDistance());
		cameras[i]->setFarClipDistance(10.0f);
		cameras[i]->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
		cameras[i]->setPosition((i * 2 - 1) /** rift->getStereo().GetIPD()*/ * 0.5f, 0, 500);
		cameras[i]->lookAt(0, 0, 1);
		double pMatrix[16];
		camParams.OgreGetProjectionMatrix(camParams.CamSize, camParams.CamSize, pMatrix, 0.05, 10, false);
		Ogre::Matrix4 PM(pMatrix[0], pMatrix[1], pMatrix[2], pMatrix[3],
			pMatrix[4], pMatrix[5], pMatrix[6], pMatrix[7],
			pMatrix[8], pMatrix[9], pMatrix[10], pMatrix[11],
			pMatrix[12], pMatrix[13], pMatrix[14], pMatrix[15]);
		cameras[i]->setCustomProjectionMatrix(true, PM);
		cameras[i]->setCustomViewMatrix(true, Ogre::Matrix4::IDENTITY);
		//cameras[i]->setAspectRatio(rift->getStereo().GetAspect());
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
		viewports[i] = window->addViewport(cameras[1], i, i == 0 ? 0.015f : 0.485f, 0, 0.5f, 1.0f);
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

Ogre::Root* OgreRenderer::getRoot()
{
    return ogre;
}

Ogre::RenderWindow* OgreRenderer::getWindow()
{
    return window;
}


bool OgreRenderer::getShutDown()
{
    return ShutDown;
}

Gui* OgreRenderer::getGui()
{
    return ui;
}
