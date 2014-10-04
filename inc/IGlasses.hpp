#ifndef _R_IGLASSES_HPP_
#define _R_IGLASSES_HPP_

class IGlasses
{
public:
	virtual IGlasses*   Init() = 0;
	virtual void        setDistordScale() = 0;
	virtual float       getDistordScale() = 0;
	virtual int*        getResolution() = 0;
};


#endif
