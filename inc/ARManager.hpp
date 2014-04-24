/*
**	Class created by Alexandre Haulotte for Reality project
**	This class is used to detect marker in environement
*/

#ifndef		__AR_MANAGER__
# define	__AR_MANAGER__

#include	<list>
#include	<map>
#include	<string>
#include	<iostream>

#include	<boost/thread.hpp>

#include	<opencv2/opencv.hpp>
#include	<opencv2/highgui/highgui.hpp>

#ifdef _WIN32
	#include	<Windows.h>
#endif // _WIN32

#include	"TinyXML/tinyxml.h"
#include	"ARma/patterndetector.h"

#pragma once
class ARManager
{
private:
	int					norm_pattern_size = 64;
	double				fixed_thresh = 50;
	double				adapt_thresh = 5;			//non-used with FIXED_THRESHOLD mode
	int					adapt_block_size = 45;		//non-used with FIXED_THRESHOLD mode
	double				confidenceThreshold = 0.40;
	int					mode = 2;					//1:FIXED_THRESHOLD, 2: ADAPTIVE_THRESHOLD

public:
	static const bool			verbose = false;

private:
	boost::mutex				m_marker;
	boost::thread				arThread;
	cv::Mat						frame;
    std::map<int, int>	markerList; //TODO : modify this useless
    std::list<ARma::Pattern>		markerFound;
	std::vector<cv::Mat>		patternLibrary;
	std::vector<ARma::Pattern>	detectedPattern;
	bool						frameChange;
	bool						markerChange;
	ARma::PatternDetector		*detector;
	int							patternCount;
	cv::Mat						cameraMatrix;
	cv::Mat						distortions;

public:
	ARManager();
	~ARManager();

public:
	void						init(std::string filename = "Data/db.xml");
	void						start();
	void						stop();
	bool						setFrame(cv::Mat p_frame);
	cv::Mat						getFrame() const;
    void						setMarkerList(std::map<int, int> &markers);
    std::list<ARma::Pattern>		getMarkers();
	bool						isChanged();
	void						clearMarker();
	void						addMarker(ARma::Pattern info);
	void						validMarkers();
	void						change(cv::Mat frame);
	int							loadPattern(const char* filename, std::vector<cv::Mat>& library, int& patternCount);
	int							getCount() const;
	void						cleanVector(std::vector<ARma::Pattern>& p_Pattern);
	int							calcDist(std::vector<ARma::Pattern>& p_Pattern);

private:
	static void arLoop(ARManager *ar);

// Debug Function
public:
	void	draw(cv::Mat& frame);
};

#endif