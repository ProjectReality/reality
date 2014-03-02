#ifndef		_R_OCULUS_HPP_
#define		_R_OCULUS_HPP_

#include <OVR.h>

using namespace OVR;

class Oculus
{
public:
	Oculus();
	~Oculus();
	float getDistordScale();
private:
	OVR::Ptr<DeviceManager> pManager;
	OVR::Ptr<HMDDevice> pHMD;
};


#endif // _R_OCULUS_HPP_