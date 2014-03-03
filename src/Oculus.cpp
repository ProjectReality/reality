#include "Oculus.hpp"

Oculus::Oculus() {
	System::Init(Log::ConfigureDefaultLog(LogMask_All));
	
	pManager = *DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
	pHMD->GetDeviceInfo(&hmd);
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

float Oculus::getDistordScale() {
	//TODO : Remove hardcod screen res
	using namespace Util::Render;

	float renderscale;
	StereoConfig stereo;
	
	stereo.SetFullViewport(Viewport(0, 0, 800, 600));
	stereo.SetStereoMode(Stereo_LeftRight_Multipass);
	stereo.SetHMDInfo(hmd);
	stereo.SetDistortionFitPointVP(-1.f, 0.f);

	renderscale = stereo.GetDistortionScale();

	return renderscale;
}