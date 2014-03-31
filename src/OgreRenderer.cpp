#include    <iostream>
#include    <boost/current_function.hpp>

#include    "OgreRenderer.hpp"

OgreRenderer::OgreRenderer(double camsize[2], VirtualOculus *rift)
{
    this->rift = rift;
    const Ogre::ColourValue g_defaultViewportColour(0,0,0);

    cam_frame_size[0] = camsize[0];
    cam_frame_size[1] = camsize[1];

    if (cam_frame_size[0] <= 0 || cam_frame_size[1] <= 0) {
        std::cerr << "Fatal Error: OgreRenderer(): frame size can't be 0, bye" << std::endl;
        exit(11);
    }

  // Ogre init

  ogre = new Ogre::Root();

  if (!ogre->showConfigDialog()) { //show the config window
     exit(11);
 }

  window = ogre->initialise(true, "Rendering Window"); //must init windows before load rsrc


  // Scene init
  scene = ogre->createSceneManager("OctreeSceneManager");

  Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Assets");

  scene->setAmbientLight(Ogre::ColourValue(.6, .6, .6));
  scene->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
  scene->setShadowColour(Ogre::ColourValue(0.6, 0.6, 0.6));
  scene->setShadowFarDistance(700);

  Ogre::Light* sunLight = scene->createLight("VayaLight");
  sunLight->setPosition(1200, 100, 600);
  sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
  sunLight->setDiffuseColour(.35, .35, 0.38);
  sunLight->setSpecularColour(.9, .9, 1);

  Ogre::Vector3 dir(-1, -1, 0.5);
  dir.normalise();
  sunLight->setDirection(dir);
  sunLight->setCastShadows(true);

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

  Ogre::Vector4 hmdwarp = Ogre::Vector4(rift->getStereo().GetDistortionK(0),
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


  /*
  * GENERATE CAM AND VIEWPORT
  */

  for (size_t i = 0; i < 2; i++) {
     cameras[i] = scene->createCamera(i == 0 ? "Left" : "Right");
     viewports[i] = window->addViewport(cameras[i], i, 0.5f * i, 0, 0.5f, 1.0f);
     viewports[i]->setBackgroundColour(g_defaultViewportColour);
      viewports[i]->setVisibilityMask(i == 0 ? 0xFFFFFF00 : 0xFFFF0F0); //to hide some stuff between each viewport

      cameras[i]->setNearClipDistance(rift->getStereo().GetEyeToScreenDistance());
      cameras[i]->setFarClipDistance(10000.0f); //TODO get from class virtuaocu
      cameras[i]->setPosition((i * 2 - 1) * rift->getStereo().GetIPD() * 0.5f, 0, 500);
      cameras[i]->setAspectRatio(rift->getStereo().GetAspect());
      cameras[i]->setFOVy(Ogre::Radian(rift->getStereo().GetYFOVRadians()));
      cameras[i]->lookAt(Ogre::Vector3(0, 0, -300));

      Ogre::Matrix4 proj = Ogre::Matrix4::IDENTITY;
      proj.setTrans(Ogre::Vector3(-rift->getStereo().GetProjectionCenterOffset() * (2 * i - 1), 0, 0));
      cameras[i]->setCustomProjectionMatrix(true, proj * cameras[i]->getProjectionMatrix());

      compos[i] = Ogre::CompositorManager::getSingleton().addCompositor(viewports[i], i == 0 ? "OculusLeft" : "OculusRight");
      compos[i]->setEnabled(true);
  }

  /*
  * GENERATE RECT FOR CAM TEX
  */

  Ogre::AxisAlignedBox aabInf;
  aabInf.setInfinite();

  for (size_t i = 0; i < 2; i++) {
     rects[i] = new Ogre::Rectangle2D(true);
     rects[i]->setCorners(-1, 1.0, 1, -1.0);
     rects[i]->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
      rects[i]->setVisibilityFlags(i == 0 ? 0xF00 : 0xF0); //hide left rect in right viewport and inverse.
      rects[i]->setBoundingBox(aabInf);
      scene->getRootSceneNode()->attachObject(rects[i]);

      tex[i] = Ogre::TextureManager::getSingleton().createManual(i == 0 ? "TexLeft" : "TexRight",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Ogre::TEX_TYPE_2D,
          cam_frame_size[0], cam_frame_size[1], //VIDEO SIZE
          0,
          Ogre::PF_B8G8R8,     // PIXEL FORMAT OF OPENCV FRAME (maybe not..)
          Ogre::TU_DEFAULT);
      mats[i] = Ogre::MaterialManager::getSingleton().create(i == 0 ? "MatLeft" : "MatRight",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
      mats[i]->getTechnique(0)->getPass(0)->createTextureUnitState(i == 0 ? "TexLeft" : "TexRight");
      mats[i]->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
      mats[i]->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
      mats[i]->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
      mats[i]->getTechnique(0)->getPass(0)->setLightingEnabled(false);

      rects[i]->setMaterial(i == 0 ? "MatLeft" : "MatRight");
  }
  alive = true; // Ogre is now running
  ShutDown = false;
}

OgreRenderer::~OgreRenderer()
{
  delete ogre; // Apparently handle deletes for most ogre-related things.
}

// General scene functions
void    OgreRenderer::createEntity(std::string _name, std::string _mesh)
{
  if ((_name.length() < 1))
    std::cerr << "Error: createEntity: invalid name" << std::endl;
else if (entities[_name].name != _name)
{
  entities[_name].name = _name;
  if ((_mesh == "") || (&_mesh == NULL))
   entities[_name].ent = scene->createEntity(_name, "knot.mesh");
else
    entities[_name].ent = scene->createEntity(_name, _mesh);
entities[_name].node = scene->getRootSceneNode()->createChildSceneNode();
entities[_name].node->attachObject(entities[_name].ent);
}
else
 std::cerr << "Error: createEntity: An entity already exists with this name" << std::endl;
}

void    OgreRenderer::createLight(std::string _name) {
    if (entities.find(_name) == entities.end()) {
        std::cerr << "Error: " << BOOST_CURRENT_FUNCTION << ": No Entity exist with this name: " << _name << std::endl;
        return;
    }

}

void OgreRenderer::newPosEntity(std::string _name, float x, float y, float z) {
    if (entities.find(_name) == entities.end()) {
        std::cerr << "Error: " << BOOST_CURRENT_FUNCTION << ": No Entity exist with this name: " << _name << std::endl;
        return;
    }
    entities[_name].node->setPosition(Ogre::Vector3(x, y, z));
}

void    OgreRenderer::moveEntity(std::string _name, float x, float y, float z) {
    if (entities.find(_name) == entities.end()) {
        std::cerr << "Error: " << BOOST_CURRENT_FUNCTION << ": No Entity exist with this name: " << _name << std::endl;
        return;
    }
    entities[_name].node->setPosition(entities[_name].node->getPosition() + Ogre::Vector3(x, y, z));
}

void    OgreRenderer::rotateEntity(std::string _name, float yaw, float pitch, float roll) {
    if (entities.find(_name) == entities.end()) {
        std::cerr << "Error: " << BOOST_CURRENT_FUNCTION << ": No Entity exist with this name: " << _name << std::endl;
        return;
    }
  entities[_name].node->yaw(Ogre::Degree(yaw));
  entities[_name].node->pitch(Ogre::Degree(pitch));
  entities[_name].node->roll(Ogre::Degree(roll));
}

void    OgreRenderer::moveLight(std::string _name, float x, float y, float z) {
    if (entities.find(_name) == entities.end()) {
        std::cerr << "Error: " << BOOST_CURRENT_FUNCTION << ": No Entity exist with this name: " << _name << std::endl;
        return;
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

Ogre::Matrix4 OgreRenderer::OVRMat4toOgreMat4(OVR::Matrix4f matrix) {
    return Ogre::Matrix4(
        matrix.M[0][0], matrix.M[0][1], matrix.M[0][2], matrix.M[0][3],
        matrix.M[1][0], matrix.M[1][1], matrix.M[1][2], matrix.M[1][3],
        matrix.M[2][0], matrix.M[2][1], matrix.M[2][2], matrix.M[2][3],
        matrix.M[3][0], matrix.M[3][1], matrix.M[3][2], matrix.M[3][3]);
}

Ogre::Image* OgreRenderer::MatToImage(cv::Mat in) {
    Ogre::Image* out = new Ogre::Image();

    Ogre::MemoryDataStream* rightStream = new Ogre::MemoryDataStream((void*) in.data, cam_frame_size[0] * cam_frame_size[1] * 3, false, false);
    Ogre::DataStreamPtr ptr2(rightStream);
    ptr2->seek(0);
    out->loadRawData(ptr2, cam_frame_size[0], cam_frame_size[1], Ogre::PF_R8G8B8);
    return out;
}

void    OgreRenderer::loadCam(cv::Mat left, cv::Mat right)
{
    Ogre::Image* imageLeft = MatToImage(left);
    Ogre::Image* imageRight = MatToImage(right);

    if (imageLeft->getSize() >= 0 || imageRight->getSize() >= 0) {
     tex[0]->unload();
     tex[0]->loadImage(*imageLeft);
     tex[1]->unload();
     tex[1]->loadImage(*imageRight);
 } else {
  std::cerr << "OgreRenderer::loadCam() - Couldn't load image cause empty" << std::endl;
}
imageLeft->freeMemory();
imageRight->freeMemory();
}

void OgreRenderer::setFrameSize(double size[2]) {
    cam_frame_size[0] = size[0];
    cam_frame_size[1] = size[1];
}

bool OgreRenderer::getShutDown() {
    return ShutDown;
}

//temp input handling

bool OgreRenderer::keyPressed(const OIS::KeyEvent& ev)
{
    switch (ev.key)
    {
       case OIS::KC_ESCAPE:
       ShutDown = true;
       break;
       default:
       break;
   }
   return true;
}
