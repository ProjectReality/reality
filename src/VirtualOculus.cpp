#include "Oculus.hpp"

VirtualOculus::VirtualOculus() {
	
}

VirtualOculus::~VirtualOculus() {
}

VirtualOculus* VirtualOculus::Init() {
	VirtualOculus *ocu;

	System::Init(Log::ConfigureDefaultLog(LogMask_All));

	OVR::Ptr<DeviceManager>  pManager = *DeviceManager::Create();
	OVR::Ptr<HMDDevice> pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();

	if (pHMD) {
		//System::Destroy();
		pManager.Clear();
		ocu = new Oculus();
	}
	else {
		ocu = this;
	}
	return ocu;
}

int* VirtualOculus::getResolution() {
	int res[2];

	res[0] = 800;
	res[1] = 600;
	return res;
}

void VirtualOculus::setDistordScale() {
	
}

float VirtualOculus::getDistordScale() {
	return 1.7;
}

StereoEyeParams* VirtualOculus::getEyesParams() {
	StereoEyeParams Eyes[2];

	//Eyes[0] = stereo.GetEyeRenderParams(StereoEye_Left);
	//Eyes[1] = stereo.GetEyeRenderParams(StereoEye_Right);

	return Eyes;
}