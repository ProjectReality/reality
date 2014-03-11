#ifndef		_R_VIRTUAL_OCULUS_HPP_
#define		_R_VIRTUAL_OCULUS_HPP_

#include <iostream>
#include <array>
#include <OVR.h>

using namespace OVR;
using namespace OVR::Util::Render;

//! VirtualOculus
/*!
Allow to emulate an Oculus Rift by returning default value for each function

see Oculus.hpp for more doc

# TODO 

Implement all SDK

*/

class VirtualOculus
{
public:
	VirtualOculus();
	virtual ~VirtualOculus();

	virtual void setDistordScale();

	virtual float getDistordScale();

	virtual int* getResolution();

	virtual StereoEyeParams* getEyesParams();

	
	//! Init and return correct oculus
	/*!
	Detect if Oculus Rift exist and create a true Oculus class.
	if doesn't exist it use this class
	*/
	virtual VirtualOculus *Init();


	virtual OVR::Ptr<DeviceManager> getpManager();
	virtual OVR::Ptr<HMDDevice> getpHMD();
	virtual HMDInfo getHMDInfo();
	virtual StereoConfig getStereo();

private:
	OVR::Ptr<DeviceManager> pManager;
	OVR::Ptr<HMDDevice> pHMD;
	HMDInfo hmd;
	StereoConfig stereo;
};

#endif // _R_VIRTUAL_OCULUS_HPP_
