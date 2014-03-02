#include "Oculus.hpp"

Oculus::Oculus() {
	System::Init(Log::ConfigureDefaultLog(LogMask_All));
	
	pManager = *DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
}

Oculus::~Oculus() {
	System::Destroy();
}

float Oculus::getDistordScale() {
	//TODO : Remove hardcod screen res
	using namespace Util::Render;

	StereoConfig stereo;
	HMDInfo hmd;
	float renderscale;

	pHMD->GetDeviceInfo(&hmd);

	stereo.SetFullViewport(Viewport(0, 0, 800, 600));
	stereo.SetStereoMode(Stereo_LeftRight_Multipass);
	stereo.SetHMDInfo(hmd);
	stereo.SetDistortionFitPointVP(-1.f, 0.f);

	renderscale = stereo.GetDistortionScale();

	return renderscale;
}