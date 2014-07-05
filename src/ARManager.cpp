#include "ARManager.hpp"


//---------------------------------------- CONSTRUCTOR --------------------------------------//
ARManager::ARManager()
{
	this->cameraMatrix.readFromXMLFile("Data/camera.yml");

	this->frameChange = false;
	this->markerChange = false;
	this->markerDetector.setDesiredSpeed(2);
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
			for (int i = 0; i < ar->boards.size(); i++)
				ar->boards.at(i).likelihood = ar->boardDetector.detect(markerDetected, ar->boards.at(i).boardConf, ar->boards.at(i).board, ar->getCamParam(), 0.05);
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
		if (this->alphaVector.find(m.id) == alphaVector.end())
			this->alphaVector.insert(std::pair<int, std::vector<float>>(m.id, { 0, 0, 0, 0, 0, 0, 0 }));
	}
	for (t_board t_b : this->boards)
	{
		if (t_b.likelihood > 0.2)
		{
			aruco::Marker m;
			m.id = t_b.id;
			m.Tvec.at<float>(0) = t_b.board.Tvec.at<float>(0);
			m.Tvec.at<float>(1) = t_b.board.Tvec.at<float>(1);
			m.Tvec.at<float>(2) = t_b.board.Tvec.at<float>(2);
			m.Rvec.at<float>(0) = t_b.board.Rvec.at<float>(0);
			m.Rvec.at<float>(1) = t_b.board.Rvec.at<float>(1);
			m.Rvec.at<float>(2) = t_b.board.Rvec.at<float>(2);
			this->markerFound.insert(std::pair<int, aruco::Marker>(m.id, m));
			if (this->alphaVector.find(m.id) == alphaVector.end())
				this->alphaVector.insert(std::pair<int, std::vector<float>>(m.id, { 0, 0, 0, 0, 0, 0, 0 }));
		}
		t_b.likelihood = 0.0;
	}
	this->markerChange = true;
	markerFoundCopy = std::map<int, aruco::Marker>(this->markerFound.begin(), this->markerFound.end());
	this->detectedHisto.insert(detectedHisto.begin(), markerFoundCopy);
}

aruco::Marker ARManager::computeMarker(std::pair<int, aruco::Marker> pMarker)
{
	bool	allFound = false;

	if (this->computedHisto.size() > 3)
	{
		aruco::Marker t3Marker, t2Marker, t1Marker;
		std::map<int, aruco::Marker> t3Map = this->computedHisto.at(2);
		std::map<int, aruco::Marker> t2Map = this->computedHisto.at(1);
		std::map<int, aruco::Marker> t1Map = this->computedHisto.at(0);
		if (t3Map.find(pMarker.first) != t3Map.end())
		{
			t3Marker = t3Map.find(pMarker.first)->second;
			if (t2Map.find(pMarker.first) != t2Map.end())
			{
				t2Marker = t2Map.find(pMarker.first)->second;
				if (t1Map.find(pMarker.first) != t1Map.end())
				{
					t1Marker = t1Map.find(pMarker.first)->second;
					allFound = true;
				}
			}
		}
		if (allFound)
		{
			if (this->alphaVector.find(pMarker.first) != this->alphaVector.end())
			{
				aruco::Marker newMarker;
				newMarker.id = pMarker.first;
				std::vector<float> alpha = this->alphaVector.find(pMarker.first)->second;
				alpha.at(0) = (t1Marker.Rvec.at<float>(0) - t2Marker.Rvec.at<float>(0) + t1Marker.Rvec.at<float>(0) - t3Marker.Rvec.at<float>(0)) / 2;
				alpha.at(1) = (t1Marker.Rvec.at<float>(1) - t2Marker.Rvec.at<float>(1) + t1Marker.Rvec.at<float>(1) - t3Marker.Rvec.at<float>(1)) / 2;
				alpha.at(2) = (t1Marker.Rvec.at<float>(2) - t2Marker.Rvec.at<float>(2) + t1Marker.Rvec.at<float>(2) - t3Marker.Rvec.at<float>(2)) / 2;
				alpha.at(3) = (t1Marker.Tvec.at<float>(0) - t2Marker.Tvec.at<float>(0) + t1Marker.Tvec.at<float>(0) - t3Marker.Tvec.at<float>(0)) / 2;
				alpha.at(4) = (t1Marker.Tvec.at<float>(1) - t2Marker.Tvec.at<float>(1) + t1Marker.Tvec.at<float>(1) - t3Marker.Tvec.at<float>(1)) / 2;
				alpha.at(5) = (t1Marker.Tvec.at<float>(2) - t2Marker.Tvec.at<float>(2) + t1Marker.Tvec.at<float>(2) - t3Marker.Tvec.at<float>(2)) / 2;

				newMarker.Tvec.at<float>(0) = t1Marker.Tvec.at<float>(0) + alpha.at(3);
				newMarker.Tvec.at<float>(1) = t1Marker.Tvec.at<float>(1) + alpha.at(4);
				newMarker.Tvec.at<float>(2) = t1Marker.Tvec.at<float>(2) + alpha.at(5);
				newMarker.Rvec.at<float>(0) = t1Marker.Rvec.at<float>(0) + (alpha.at(0) / 2);
				newMarker.Rvec.at<float>(1) = t1Marker.Rvec.at<float>(1) + (alpha.at(1) / 2);
				newMarker.Rvec.at<float>(2) = t1Marker.Rvec.at<float>(2) + (alpha.at(2) / 2);
				return (newMarker);
			}
		}
	}
	return (pMarker.second);
}

bool		ARManager::isInThePrevious(aruco::Marker pMarker)
{
	if (this->alphaVector.find(pMarker.id) != this->alphaVector.end())
	{
		if (this->alphaVector.find(pMarker.id)->second.at(6) < 5)
		{
			this->alphaVector.find(pMarker.id)->second.at(6)++;
			return true;
		}
		else
		{
			this->alphaVector.find(pMarker.id)->second.at(6) = 0;
		}
	}
	return (false);
}

std::map<int, aruco::Marker> ARManager::computeNewMap()
{
	std::map<int, aruco::Marker> next_map;
	if (!computedHisto.empty())
	{
		std::map<int, aruco::Marker> prev_map = computedHisto.front();
		if (prev_map.size() > 0)
		{
			for (std::pair<int, aruco::Marker> p : markerFoundCopy)
			{
				if (prev_map.find(p.first) != prev_map.end())
				{
					if (this->alphaVector.find(p.first) != this->alphaVector.end())
					{
						this->alphaVector.find(p.first)->second.at(6) = 0;
					}
					p.second.Rvec.at<float>(0) = boost::math::round(p.second.Rvec.at<float>(0) * 100) / 100;
					p.second.Rvec.at<float>(1) = boost::math::round(p.second.Rvec.at<float>(1) * 100) / 100;
					p.second.Rvec.at<float>(2) = boost::math::round(p.second.Rvec.at<float>(2) * 100) / 100;
					next_map[p.first] = p.second;
					prev_map.erase(p.first);
				}
				else
				{
					next_map[p.first] = p.second;
				}
			}
			for (std::pair<int, aruco::Marker> p : prev_map)
			{
				if (isInThePrevious(p.second))
				{
					next_map[p.first] = computeMarker(p);
				}
			}
			computedHisto.insert(computedHisto.begin(), next_map);
		}
		else
		{
			next_map = markerFoundCopy;
			computedHisto.insert(computedHisto.begin(), markerFoundCopy);
		}
		if (computedHisto.size() > 5)
			computedHisto.pop_back();
	}
	else
	{
		next_map = markerFoundCopy;
		computedHisto.insert(computedHisto.begin(), markerFoundCopy);
	}
	return (next_map);
}

void		ARManager::addBoard(int id, const char* boardName)
{
	t_board tmp;

	tmp.id = id;
	tmp.boardConf.readFromFile(boardName);
	tmp.likelihood = 0.0;
	boards.push_back(tmp);
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


