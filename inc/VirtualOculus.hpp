#include <iostream>
#include <array>
#include <OVR.h>

using namespace OVR;
using namespace OVR::Util::Render;

class VirtualOculus
{
public:
	VirtualOculus();
	virtual ~VirtualOculus();

	virtual void setDistordScale();

	virtual float getDistordScale();

	virtual int* getResolution();

	virtual StereoEyeParams* getEyesParams();

	virtual VirtualOculus *Init();
};

