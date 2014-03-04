#include "Oculus.hpp"

Oculus::Oculus() {
	//TODO : handle no oculus 
	System::Init(Log::ConfigureDefaultLog(LogMask_All));
	
	pManager = *DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
	pHMD->GetDeviceInfo(&hmd);

	setDistordScale();
}

Oculus::~Oculus() {
	System::Destroy();
}

std::array<int, 2> Oculus::getResolution() {
	std::array<int, 2> res;

	res[0] = hmd.HResolution;
	res[1] = hmd.VResolution;
	return res;
}

void Oculus::setDistordScale() {
	stereo.SetFullViewport(Viewport(0, 0, hmd.HResolution, hmd.VResolution));
	stereo.SetStereoMode(Stereo_LeftRight_Multipass);
	stereo.SetHMDInfo(hmd);
	stereo.SetDistortionFitPointVP(-1.f, 0.f);
}

float Oculus::getDistordScale() {
	return stereo.GetDistortionScale();
}

StereoEyeParams* Oculus::getEyesParams() {
	StereoEyeParams Eyes[2];

	Eyes[0] = stereo.GetEyeRenderParams(StereoEye_Left);
	Eyes[1] = stereo.GetEyeRenderParams(StereoEye_Right);

	return Eyes;
}