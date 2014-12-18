#include    "Oculus.hpp"

VirtualOculus::VirtualOculus()
{

}

VirtualOculus::~VirtualOculus()
{
	ovrHmd_Destroy(pHMD);
	ovr_Shutdown();
}

VirtualOculus* VirtualOculus::Init()
{
  VirtualOculus *ocu;

  ovr_Initialize();
  //System::Init(Logger::log_OVR());
  pHMD = ovrHmd_Create(0);
  if (pHMD)
  {
    ovrHmd_Destroy(pHMD);
    ovr_Shutdown();
    ocu = new Oculus();
  }
  else
    ocu = this;
  if (ocu == NULL)
    std::cerr << ">>>> Error of oculus Initialisation..." << std::endl;
  return ocu;
}

int* VirtualOculus::getResolution()
{
  static int res[2];

  res[0] = 800;
  res[1] = 600;
  return res;
}

void VirtualOculus::setDistordScale()
{

}

float VirtualOculus::getDistordScale()
{
  return 1.7f;
}

ovrHmd VirtualOculus::getpHMD() { return pHMD; }
//StereoConfig VirtualOculus::getStereo() { return stereo; }
