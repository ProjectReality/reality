#include "Oculus.hpp"

Oculus::Oculus() {
	System::Init(Log::ConfigureDefaultLog(LogMask_All));

	pManager = *DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();

	if (pHMD) {
		pHMD->GetDeviceInfo(&hmd);
		//add sensor
	} else {
		//problem
	}
	setDistordScale();
}

Oculus::~Oculus() {
	pManager.Clear();
	System::Destroy();
}

int* Oculus::getResolution() {
	int res[2];

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