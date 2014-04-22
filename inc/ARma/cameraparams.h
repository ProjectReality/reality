#ifndef _CAMERAPARAMS_H
#define _CAMERAPARAMS_H

#include <opencv2/core/core.hpp>

using namespace cv;

	CvMat* intrinsic = (CvMat*)cvLoad("Data/intrinsic.xml");	
	CvMat* distor = (CvMat*)cvLoad("Data/distortion.xml");

	Mat cameraMatrix = cvarrToMat(intrinsic);
	Mat distortions = cvarrToMat(distor);



#endif


