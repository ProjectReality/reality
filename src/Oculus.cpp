#include    "Oculus.hpp"

Oculus::Oculus()
{
	Log *OVRLogger = Log::ConfigureDefaultLog(LogMask_None);
	OVRLogger->DefaultLogOutput("", 0);
	System::Init(OVRLogger);
	pManager = *DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
	pHMD->GetDeviceInfo(&hmd);
    setDistordScale();
}

Oculus::~Oculus()
{
	pHMD.Clear();
	pManager.Clear();
	System::Destroy();
}

int* Oculus::getResolution()
{
    res[0] = getHMDInfo().HResolution;
    res[1] = getHMDInfo().VResolution;
    return res;
}

void Oculus::setDistordScale()
{
    getStereo().SetFullViewport(Viewport(0, 0, getHMDInfo().HResolution, getHMDInfo().VResolution));
    getStereo().SetStereoMode(Stereo_LeftRight_Multipass);
    getStereo().SetHMDInfo(getHMDInfo());
    getStereo().SetDistortionFitPointVP(-1.f, 0.f);
}

float Oculus::getDistordScale()
{
    return getStereo().GetDistortionScale();
}

StereoEyeParams* Oculus::getEyesParams()
{
    Eyes[0] = getStereo().GetEyeRenderParams(StereoEye_Left);
    Eyes[1] = getStereo().GetEyeRenderParams(StereoEye_Right);

    return Eyes;
}
