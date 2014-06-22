#ifndef _ARMA_PATTERN_
#define _ARMA_PATTERN_
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>


using namespace cv;

namespace ARma {

class Pattern
{
	public:
		vector<Point2f> vertices;
		int id;
		int orientation;//{0,1,2,3}
		double size; //in milimeters
		double confidence;//min: -1, max: 1
		Mat rotVec, transVec, rotMat;
		std::vector<cv::Point2f> pointImage;

		Pattern(double param1=80);

		~Pattern(){};
		
		Mat&	getRotvec();
		Mat&	getTransvec();
		Mat&	getRotMat();


		//solves the exterior orientation problem between patten and camera
		void getExtrinsics(double patternSize, const Mat& cameraMatrix, const Mat& distortions);

		//augments image with 3D cubes. It;s too simple augmentation jsut for checking
		void draw(Mat& frame, const Mat& camMatrix, const Mat& distMatrix);

		//computes the rotation matrix from the rotation vector using Rodrigues
		void rotationMatrix(const Mat& rotation_vector, Mat& rotation_matrix);
		
		//prints the properties of the patten and its transformation matrix
		void showPattern(void);

		void calcPoint(const Mat& camMatrix, const Mat& distMatrix);

};


}

#endif
