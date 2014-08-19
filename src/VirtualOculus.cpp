#include    "Oculus.hpp"

VirtualOculus::VirtualOculus()
{

}

VirtualOculus::~VirtualOculus()
{
	pHMD.Clear();
	pManager.Clear();
	System::Destroy();
}

VirtualOculus* VirtualOculus::Init()
{
    VirtualOculus *ocu;
	System::Init(Log::ConfigureDefaultLog(LogMask_All));
    pManager = *DeviceManager::Create();
    pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();

    if (pHMD)
	{
		pHMD.Clear();
		pManager.Clear();
		System::Destroy();
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

StereoEyeParams* VirtualOculus::getEyesParams()
{
    static StereoEyeParams Eyes[2];
    //TODO

    return Eyes;
}

OVR::Ptr<DeviceManager> VirtualOculus::getpManager() { return pManager; }
OVR::Ptr<HMDDevice> VirtualOculus::getpHMD() { return pHMD; }
HMDInfo VirtualOculus::getHMDInfo() { return hmd; }
StereoConfig VirtualOculus::getStereo() { return stereo; }
