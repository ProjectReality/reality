#include "Oculus.hpp"

VirtualOculus::VirtualOculus() {
	
}

VirtualOculus::~VirtualOculus() {
}

VirtualOculus* VirtualOculus::Init() {
	VirtualOculus *ocu;

	System::Init(Log::ConfigureDefaultLog(LogMask_All));

	pManager = *DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();

	if (pHMD) {
		pHMD->GetDeviceInfo(&hmd);
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
	//TODO

	return Eyes;
}

OVR::Ptr<DeviceManager> VirtualOculus::getpManager() { return pManager; }
OVR::Ptr<HMDDevice> VirtualOculus::getpHMD() { return pHMD; }
HMDInfo VirtualOculus::getHMDInfo() { return hmd; }
StereoConfig VirtualOculus::getStereo() { return stereo; }