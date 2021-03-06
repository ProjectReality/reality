#include "ARManager.hpp"


void ARManager::motionLoop(ARManager *ar)
{
	while (1)
	{
		if (ar->motionFrameChange)
		{
			cv::Mat fr;
			if (true)
			{
				boost::mutex::scoped_lock lock(ar->m_marker);
				fr = ar->frame.clone();
				ar->motionFrameChange = false;
			}
			if (ar->prev_frame.empty())
			{
				cvtColor(fr, ar->prev_frame, CV_RGB2GRAY);
				createHanningWindow(ar->hann, ar->frame.size(), CV_64F);
			}
			if (!ar->prev_frame.empty() && !ar->frame.empty())
			{
				// Calcul the motion
				cv::Mat curr, curr64f, prev64f;
				cvtColor(fr, curr, CV_RGB2GRAY);
				if (ar->dAlgo == PATTERN_SIMPLE_MOTION)
				{
					ar->prev_frame.convertTo(prev64f, CV_64F);
					curr.convertTo(curr64f, CV_64F);
					ar->motion = phaseCorrelate(prev64f, curr64f, ar->hann);
				}
				else if (ar->dAlgo == PATTERN_MATRIX_MOTION)
				{
					ar->FindCameraMatrices(curr, ar->prev_frame);
				}
				else
				{
					return;
				}
				ar->prev_frame = curr.clone();
				// End motion set
			}
		}
	}
}

std::vector<cv::DMatch> ARManager::getOpticalMatches(cv::Mat curr, cv::Mat prev, std::vector<cv::KeyPoint>& left_keypoints, std::vector<cv::KeyPoint>& right_keypoints)
{
	std::vector<cv::DMatch> matches;

	std::vector<cv::Point2f> left_points;
	cv::KeyPoint::convert(left_keypoints, left_points);

	std::vector<cv::Point2f> right_points(left_points.size());

	// making sure images are grayscale
	cv::Mat prevgray, gray;
	if (prev.channels() == 3) {
		cv::cvtColor(prev, prevgray, CV_RGB2GRAY);
		cv::cvtColor(curr, gray, CV_RGB2GRAY);
	}
	else {
		prevgray = prev;
		gray = curr;
	}

	// Calculate the optical flow field:
	//  how each left_point moved across the 2 images
	std::vector<uchar> vstatus; 
	std::vector<float> verror;
	calcOpticalFlowPyrLK(prevgray, gray, left_points, right_points, vstatus, verror);

	// First, filter out the points with high error
	std::vector<cv::Point2f> right_points_to_find;
	std::vector<int> right_points_to_find_back_index;
	for (unsigned int i = 0; i < vstatus.size() ; i++) {
		if (vstatus[i] && verror[i] < 12.0) {
			// Keep the original index of the point in the
			// optical flow array, for future use
			right_points_to_find_back_index.push_back(i);
			// Keep the feature point itself
			right_points_to_find.push_back(right_points[i]); // <== j_pts
		}
		else {
			vstatus[i] = 0; // a bad flow
		}
	}

	// for each right_point see which detected feature it belongs to
	cv::Mat right_points_to_find_flat = cv::Mat(right_points_to_find).reshape(1, right_points_to_find.size()); // <== to_find

	vector<cv::Point2f> right_features; // detected features
	cv::KeyPoint::convert(right_keypoints, right_features);

	cv::Mat right_features_flat = cv::Mat(right_features).reshape(1, right_features.size());

	// Look around each OF point in the right image
	//  for any features that were detected in its area
	//  and make a match.
	cv::BFMatcher matcher(CV_L2);
	vector<vector<cv::DMatch>>nearest_neighbors;
	matcher.radiusMatch(
		right_points_to_find_flat,
		right_features_flat,
		nearest_neighbors,
		2.0f);

	// Check that the found neighbors are unique (throw away neighbors
	//  that are too close together, as they may be confusing)
	std::set<int>found_in_right_points; // for duplicate prevention
	for (int i = 0; i < nearest_neighbors.size(); i++) {
		cv::DMatch _m;
		if (nearest_neighbors[i].size() == 1) {
			_m = nearest_neighbors[i][0]; // only one neighbor
		}
		else if (nearest_neighbors[i].size()>1) {
			// 2 neighbors � check how close they are
			double ratio = nearest_neighbors[i][0].distance / nearest_neighbors[i][1].distance;
			if (ratio < 0.7) { // not too close
				// take the closest (first) one
				_m = nearest_neighbors[i][0];
			}
			else { // too close � we cannot tell which is better
				continue; // did not pass ratio test � throw away
			}
		}
		else {
			continue; // no neighbors... :(
		}

		// prevent duplicates
		if (found_in_right_points.find(_m.trainIdx) == found_in_right_points.end()) {
			// The found neighbor was not yet used:
			// We should match it with the original indexing 
			// ofthe left point
			_m.queryIdx = right_points_to_find_back_index[_m.queryIdx];
			matches.push_back(_m); // add this match
			found_in_right_points.insert(_m.trainIdx);
		}
	}
	if (this->verbose)
	{
		cout << "pruned " << matches.size() << " / " << nearest_neighbors.size() << " matches" << endl;
	}
	return (matches);
}

bool ARManager::CheckCoherentRotation(cv::Mat_<double>& R) 
{
	if (fabsf(determinant(R)) - 1.0 > 1e-07) {
		cerr << "det(R) != +-1.0, this is not a rotation matrix" << endl;
		return false;
	}
	return true;
}

void ARManager::FindCameraMatrices(cv::Mat curr, cv::Mat prev)
{
	//Find camera matrices
	std::vector<cv::KeyPoint>	imgpts1;
	std::vector<cv::KeyPoint>	imgpts2;
	cv::Matx34d					P;
	std::vector<cv::DMatch>		matches;

	cv::FastFeatureDetector ffd;
	ffd.detect(prev, imgpts1); 
	ffd.detect(curr, imgpts2);

	/*
	namespace pt = boost::posix_time;
	pt::ptime now1 = pt::microsec_clock::universal_time();
	pt::ptime now2 = pt::microsec_clock::universal_time();
	pt::time_duration dur = now2 - now1;
	std::cout << "before matcher : " << dur.total_microseconds() << std::endl;
	*/

	matches = this->getOpticalMatches(curr, prev, imgpts1, imgpts2);

	//Get Fundamental Matrix
	vector<cv::Point2f> pt1, pt2; // detected features
	for( unsigned int i = 0; i < matches.size(); i++ )
	{
		// queryIdx is the "left" image
		pt1.push_back(imgpts1[matches[i].queryIdx].pt);
		// trainIdx is the "right" image
		pt2.push_back(imgpts2[matches[i].trainIdx].pt);
	}
	cv::Mat F;
	if (pt1.size() > 1 && pt2.size() > 1)
		F = cv::findFundamentalMat(pt2, pt1);
	else
		return ;

	//Essential matrix: compute then extract cameras [R|t]
	F.convertTo(F, K.type());
	cv::Mat_<double> E = K.t() * F * K; //according to HZ (9.12)

	//decompose E to P' , HZ (9.19)
	cv::SVD svd(E, cv::SVD::MODIFY_A);
	cv::Mat svd_u = svd.u;
	cv::Mat svd_vt = svd.vt;
	cv::Mat svd_w = svd.w;

	cv::Matx33d W(0, -1, 0,//HZ 9.13
		1, 0, 0,
		0, 0, 1);
	cv::Mat_<double> R = svd_u * cv::Mat(W) * svd_vt; //HZ 9.19
	cv::Mat_<double> t = svd_u.col(2); //u3

	if (!CheckCoherentRotation(R)) {
		if (this->verbose)
		{
			cout << "resulting rotation is not coherent" << std::endl;
		}
		cv::Mat rotMat(3, 3, CV_32F);
		rotMat.at<double>(0, 0) = 0;
		rotMat.at<double>(0, 1) = 0;
		rotMat.at<double>(0, 2) = 0;
		rotMat.at<double>(1, 0) = 0;
		rotMat.at<double>(1, 1) = 0;
		rotMat.at<double>(1, 2) = 0;
		rotMat.at<double>(2, 0) = 0;
		rotMat.at<double>(2, 1) = 0;
		rotMat.at<double>(2, 2) = 0;
		this->motionMarker.Tvec.at<double>(0) = 0;
		this->motionMarker.Tvec.at<double>(1) = 0;
		this->motionMarker.Tvec.at<double>(2) = 0;
		cv::Rodrigues(rotMat, this->motionMarker.Rvec);
		return;
	}

	P = cv::Matx34d(R(0, 0), R(0, 1), R(0, 2), t(0),
		R(1, 0), R(1, 1), R(1, 2), t(1),
		R(2, 0), R(2, 1), R(2, 2), t(2));

	cv::Mat rotMat(3, 3, CV_32F);
	rotMat.at<double>(0, 0) = R(0, 0);
	rotMat.at<double>(0, 1) = R(0, 1);
	rotMat.at<double>(0, 2) = R(0, 2);
	rotMat.at<double>(1, 0) = R(1, 0);
	rotMat.at<double>(1, 1) = R(1, 1);
	rotMat.at<double>(1, 2) = R(1, 2);
	rotMat.at<double>(2, 0) = R(2, 0);
	rotMat.at<double>(2, 1) = R(2, 1);
	rotMat.at<double>(2, 2) = R(2, 2);
	this->motionMarker.Tvec.at<double>(0) = t(0);
	this->motionMarker.Tvec.at<double>(1) = t(1);
	this->motionMarker.Tvec.at<double>(2) = t(2);
	cv::Rodrigues(rotMat, this->motionMarker.Rvec);
}
