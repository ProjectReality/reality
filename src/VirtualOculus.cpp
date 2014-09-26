#include    "Oculus.hpp"

VirtualOculus::VirtualOculus()
{

}

VirtualOculus::~VirtualOculus()
{
	pHMD.Clear();
	System::Destroy();
}

VirtualOculus* VirtualOculus::Init()
{
    VirtualOculus *ocu;

	System::Init(Logger::log_OVR());
	pHMD = ovrHmd_Create(0);

    if (pHMD)
	{
		pHMD.Clear();
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

ovrHmd VirtualOculus::getpHMD() { return pHMD; }
HMDInfo VirtualOculus::getHMDInfo() { return hmd; }
StereoConfig VirtualOculus::getStereo() { return stereo; }
