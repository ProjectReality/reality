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
#include	"AssetInfo.hpp"
#include	"ARma/cameraparams.h"
#include	"ARma/patterndetector.h"

#pragma once
class ARManager
{
private:
	const int					norm_pattern_size = 64;
	const double				fixed_thresh = 40;
	const double				adapt_thresh = 5;			//non-used with FIXED_THRESHOLD mode
	const int					adapt_block_size = 45;		//non-used with FIXED_THRESHOLD mode
	const double				confidenceThreshold = 0.35;
	const int					mode = 2;					//1:FIXED_THRESHOLD, 2: ADAPTIVE_THRESHOLD

public:
	static const bool			verbose = true;

private:
	boost::mutex				m_marker;
	boost::thread				arThread;
	cv::Mat						frame;
	std::map<int, AssetInfo>	markerList;
	std::list<AssetInfo>		markerFound;
	std::vector<cv::Mat>		patternLibrary;
	std::vector<ARma::Pattern>	detectedPattern;
	bool						frameChange = false;
	bool						markerChange = false;
	ARma::PatternDetector		*detector;
	int							patternCount;

public:
	ARManager();
	~ARManager();

public:
	void						init(std::string filename = "db.xml");
	void						start();
	void						stop();
	bool						setFrame(cv::Mat p_frame);
	cv::Mat						getFrame() const;
	void						setMarkerList(std::map<int, AssetInfo> &markers);
	std::list<AssetInfo>		getMarkers();
	bool						isChanged();
	void						clearMarker();
	void						addMarker(ARma::Pattern info);
	void						validMarkers();
	void						change(cv::Mat frame);
	int							loadPattern(const char* filename, std::vector<cv::Mat>& library, int& patternCount);
	int							getCount() const;

private:
	static void arLoop(ARManager *ar);

// Debug Function
public:
	void	draw(cv::Mat& frame);
};

#endif