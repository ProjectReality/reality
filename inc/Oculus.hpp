#ifndef		_R_OCULUS_HPP_
#define		_R_OCULUS_HPP_

#include <iostream>
#include <array>
#include <OVR.h>

using namespace OVR;
using namespace OVR::Util::Render;

//! Oculus
/*!
Wrap the oculus SDK

# Terminology

*IPD (Interpupillary distance) The distance between the eye pupils
*FOV (Field of view) The full vertical viewing angle used to configure rendering. This is
computed based on the eye distance and display size.
*HMD (Head Mounted Display) aka the Oculus rift

# Stuff

nothing..

*/

class Oculus
{
public:
	Oculus();
	~Oculus();

	void Oculus::setDistordScale();

	//! Computes the rendering scale that should be applied to the render texture
	float getDistordScale();

	//! Get the resolution of the entire HMD screen in pixels.
	/*!
		Half the HResolution is used for each eye.
		The reported values are 1280 800 for the DK
		@return respectivly vertical [0] and horizontal [1] resolution
	*/
	std::array<int, 2> getResolution();

	StereoEyeParams* Oculus::getEyesParams();

private:
	OVR::Ptr<DeviceManager> pManager;
	OVR::Ptr<HMDDevice> pHMD;
	HMDInfo hmd;
	StereoConfig stereo;
};


#endif // _R_OCULUS_HPP_