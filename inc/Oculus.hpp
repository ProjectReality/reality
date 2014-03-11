#ifndef		_R_OCULUS_HPP_
#define		_R_OCULUS_HPP_

#include "VirtualOculus.hpp"

//! Oculus
/*!
Wrap the oculus SDK

# Terminology

*IPD (Interpupillary distance) The distance between the eye pupils
*FOV (Field of view) The full vertical viewing angle used to configure rendering. This is
computed based on the eye distance and display size.
*HMD (Head Mounted Display) aka the Oculus rift

# Important

*/

class Oculus : public VirtualOculus
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
	int* getResolution();

	//! Get the stereo paramaters for each eyes first left [0] then right [1] 
	StereoEyeParams* Oculus::getEyesParams();
};


#endif // _R_OCULUS_HPP_