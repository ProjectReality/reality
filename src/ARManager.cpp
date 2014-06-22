#include "ARManager.hpp"
#include "ARma/patterndetector.h"
#include "ARma/pattern.h"


//---------------------------------------- CONSTRUCTOR --------------------------------------//
ARManager::ARManager()
{
	this->cameraMatrix.readFromXMLFile("Data/camera.yml");

	this->frameChange = false;
	this->markerChange = false;
}

//---------------------------------------- MAIN FUNCTIONS --------------------------------------//

void ARManager::addPatternInList(std::string name, int id)
{
	listObjects[id] = name;
}

void ARManager::start()
{
	this->arThread = boost::thread(&arLoop, this);
}

void ARManager::stop()
{
	this->arThread.interrupt();
}

void ARManager::arLoop(ARManager *ar)
{

	std::vector<aruco::Marker> markerDetected;
	while (1)
	{
		
		if (ar->frameChange)
		{
			cv::Mat fr;
			if (true)
			{
				boost::mutex::scoped_lock lock(ar->m_marker);
				fr = ar->frame.clone();
				ar->frameChange = false;
			}
			ar->markerDetector.detect(fr, markerDetected, ar->getCamParam(), 0.05);
			ar->clearMarker();
			ar->addMarker(markerDetected);
		}
#ifdef _WIN32
		Sleep(10);
#else
		usleep(10);
#endif
		markerDetected.clear();
	}
}

bool ARManager::isChanged()
{
	return (this->markerChange);
}


void ARManager::clearMarker()
{
	this->markerFound.clear();
}

void ARManager::addMarker(std::vector<aruco::Marker> markers)
{
<<<<<<< HEAD
    std::map<int, int>::iterator	it = this->markerList.find(info.id);
	if (it != this->markerList.end())
=======
	boost::mutex::scoped_lock lock(m_marker);
	for each(aruco::Marker m in markers)
>>>>>>> remotes/origin/Test_aruco
	{
		this->markerFound.insert(std::pair<int, aruco::Marker>(m.id, m));
		this->markerChange = true;
	}
	markerFoundCopy = std::map<int, aruco::Marker>(this->markerFound.begin(), this->markerFound.end());
}

std::map<int, aruco::Marker> ARManager::computeNewMap()
{
	std::map<int, aruco::Marker> next_map;
	/*
	std::map<std::string, ARma::Pattern> prev_map = histo.front();

	if (prev_map.size() > 0)
	{
		for each (pair<std::string, ARma::Pattern> pa in prev_map)
		{
			float p_x = pa.second.transVec.at<float>(0) * 4;
			float p_y = -pa.second.transVec.at<float>(1) * 9;
			float p_z = -pa.second.transVec.at<float>(2);
			if (markerFound.find(pa.first) != markerFound.end())
			{
				next_map[pa.first] = markerFound[pa.first];
			}
			else
			{
				if ((p_x < 600 && p_x > -600) && (p_y < 600 && p_y > -600))
					next_map[pa.first] = pa.second;
			}
		}
	}
	for each (pair<std::string, ARma::Pattern> pa in markerFound)
	{
		if (next_map.find(pa.first) == next_map.end())
			next_map[pa.first] = pa.second;
	}
	histo.pop_back();*/
	return (next_map);
}
//---------------------------------------- GETTER & SETTER --------------------------------------//
bool ARManager::setFrame(cv::Mat p_frame)
{
	boost::mutex::scoped_lock	lock(this->m_marker);
	this->frame = p_frame.clone();
	this->frameChange = true;
	return true;
}

cv::Mat ARManager::getFrame() const
{
	return (this->frame);
}

void ARManager::setMarkerList(std::map<int, std::string> &markers)
{
	this->listObjects = markers;
}

std::map<int, aruco::Marker> ARManager::getMarkers()
{
	boost::mutex::scoped_lock lock(m_marker);
	if (this->markerChange)
	{
		this->markerChange = false;
	}
	//std::map<int, aruco::Marker> map(this->markerFound.begin(), this->markerFound.end());
	return (markerFoundCopy);
}

aruco::CameraParameters ARManager::getCamParam() const
{
	return (this->cameraMatrix);
}

//---------------------------------------- DESTRUCTOR --------------------------------------//
ARManager::~ARManager() {}


