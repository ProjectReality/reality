#include    "Oculus.hpp"

Oculus::Oculus()
{
	Log *OVRLogger = Log::ConfigureDefaultLog(LogMask_None);
	OVRLogger->DefaultLogOutput("", 0);
	ovr_Initialize();
	System::Init(OVRLogger);
	pHMD = ovrHmd_Create(0);
    setDistordScale();
}

Oculus::~Oculus()
{
	ovrHmd_Destroy(pHMD);
	ovr_Shutdown();
}

int* Oculus::getResolution()
{
	res[0] = pHMD->Resolution.w;
	res[1] = pHMD->Resolution.h;
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

ovrEyeRenderDesc* Oculus::getEyesParams()
{
	Eyes[0] = ovrHmd_GetRenderDesc(pHMD, ovrEye_Left, eyeFov[0]);
	Eyes[1] = ovrHmd_GetRenderDesc(pHMD, ovrEye_Left, eyeFov[1]);

    return Eyes;
}
