#include "ARManager.hpp"


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
	boost::mutex::scoped_lock lock(m_marker);
	for (aruco::Marker m : markers)
	{
		this->markerFound.insert(std::pair<int, aruco::Marker>(m.id, m));
		this->markerChange = true;
	}
	markerFoundCopy = std::map<int, aruco::Marker>(this->markerFound.begin(), this->markerFound.end());
}

aruco::Marker ARManager::computeMarker(aruco::Marker nMarker, aruco::Marker pMarker)
{
	/*double pposition[3], porientation[4], nposition[3], norientation[4];

	nMarker.OgreGetPoseParameters(nposition, norientation);
	pMarker.OgreGetPoseParameters(pposition, porientation);

	if (nposition[0] - pposition[0] > 0.05 || nposition[0] - pposition[0] < -0.05 ||
		nposition[1] - pposition[1] > 0.05 || nposition[1] - pposition[1] < -0.05 ||
		nposition[2] - pposition[2] > 0.05 || nposition[2] - pposition[2] < -0.05)
	{
		return (pMarker);
	}
	else if (norientation[0] - porientation[0] > 0.1 || norientation[0] - porientation[0] < -0.1 ||
		norientation[1] - porientation[1] > 0.1 || norientation[1] - porientation[1] < -0.1 ||
		norientation[2] - porientation[2] > 0.1 || norientation[2] - porientation[2] < -0.1 ||
		norientation[3] - porientation[3] > 0.1 || norientation[3] - porientation[3] < -0.1)
	{
		return (pMarker);
	}*/
	return (nMarker);
}

std::map<int, aruco::Marker> ARManager::computeNewMap()
{
	std::map<int, aruco::Marker> next_map;
	if (!histo.empty())
	{
		std::map<int, aruco::Marker> prev_map = histo.front();

		if (prev_map.size() > 0)
		{
			for (std::pair<int, aruco::Marker> p : markerFoundCopy)
			{
				if (prev_map.find(p.first) != prev_map.end())
				{
					next_map[p.first] = computeMarker(p.second, prev_map.find(p.first)->second);
					prev_map.erase(prev_map.find(p.first));
				}
				else
					next_map[p.first] = p.second;
			}
			for (std::pair<int, aruco::Marker> p : prev_map)
			{
				double position[3], orientation[4];
				p.second.OgreGetPoseParameters(position, orientation);
				if ((position[0] > -0.08 && position[0] < 0.08) ||
					(position[1] > -0.08 && position[1] < 0.08))
					next_map[p.first] = p.second;
			}
			histo.insert(histo.begin(), next_map);
		}
		else
		{
			next_map = markerFoundCopy;
			histo.insert(histo.begin(), markerFoundCopy);
		}
		if (histo.size() > 5)
			histo.pop_back();
	}
	else
	{
		next_map = markerFoundCopy;
		histo.insert(histo.begin(), markerFoundCopy);
	}
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
	return (computeNewMap());
}

aruco::CameraParameters ARManager::getCamParam() const
{
	return (this->cameraMatrix);
}

//---------------------------------------- DESTRUCTOR --------------------------------------//
ARManager::~ARManager() {}


