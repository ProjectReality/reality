#include "ARManager.hpp"


//---------------------------------------- CONSTRUCTOR --------------------------------------//
ARManager::ARManager()
{
	this->cameraMatrix.readFromXMLFile("Data/camera.yml");
	K = this->cameraMatrix.CameraMatrix;
	this->prev_frame.data = NULL;
	this->frame.data = NULL;
	this->frameChange = false;
	this->motionFrameChange = false;
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
	this->motionThread = boost::thread(&motionLoop, this);
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
	for (Hexapat hex : this->hexapats)
	{
		this->markerFound = hex.detect(this->markerFound);
	}
	this->markerChange = true;
	markerFoundCopy = std::map<int, aruco::Marker>(this->markerFound.begin(), this->markerFound.end());
}

aruco::Marker ARManager::computeMarker(std::pair<int, aruco::Marker> pMarker)
{
	bool	allFound = false;
	double	radius = 0;

	radius = cv::sqrt(this->motion.x*this->motion.x + this->motion.y*this->motion.y);
	//if (radius < 2)
	//{
		/*if (this->computedHisto.size() > 3)
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
		}*/
	//}
	//else
	//{
		switch (this->dAlgo)
		{
		case PATTERN_SIMPLE_MOTION:
			if (radius > 2)
			{
				pMarker.second.Tvec.at<float>(0) = pMarker.second.Tvec.at<float>(0) + (motion.x / (FRAME_WIDTH * 1.5));
				pMarker.second.Tvec.at<float>(1) = pMarker.second.Tvec.at<float>(1) + (motion.y / (FRAME_HEIGHT * 3));
				this->alphaVector.find(pMarker.first)->second.at(6) = 0;
			}
			break;
		case PATTERN_MATRIX_MOTION:
			pMarker.second.Tvec.at<float>(0) += (motionMarker.Tvec.at<double>(0) / 800);
			pMarker.second.Tvec.at<float>(1) += (motionMarker.Tvec.at<double>(1) / 800);
			pMarker.second.Tvec.at<float>(2) += (motionMarker.Tvec.at<double>(2) / 800);
			pMarker.second.Rvec.at<float>(0) += (motionMarker.Rvec.at<double>(0) / 800);
			pMarker.second.Rvec.at<float>(1) += (motionMarker.Rvec.at<double>(1) / 800);
			pMarker.second.Rvec.at<float>(2) += (motionMarker.Rvec.at<double>(2) / 800);
			this->alphaVector.find(pMarker.first)->second.at(6) = 0;
			break;
		}
	//}
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

aruco::Marker	ARManager::isInMotion(aruco::Marker mark)
{
	float	sumYaw = 0;
	float	sumPitch = 0;
	float	sumRoll = 0;
	float	sumX = 0;
	float	sumY = 0;
	float	sumZ = 0;
	int		iter = 0;
	for (int i = 0; i < detectedHisto.size() - 1; i++)
	{
		for (int j = i + 1; j < detectedHisto.size(); j++)
		{
			if (detectedHisto.at(i).find(mark.id) != detectedHisto.at(i).end() && detectedHisto.at(j).find(mark.id) != detectedHisto.at(j).end())
			{
				iter++;
				sumYaw = sumYaw + (detectedHisto.at(i).find(mark.id)->second.Rvec.at<float>(0) - detectedHisto.at(j).find(mark.id)->second.Rvec.at<float>(0));
				sumPitch = sumPitch + (detectedHisto.at(i).find(mark.id)->second.Rvec.at<float>(1) - detectedHisto.at(j).find(mark.id)->second.Rvec.at<float>(1));
				sumRoll = sumRoll + (detectedHisto.at(i).find(mark.id)->second.Rvec.at<float>(2) - detectedHisto.at(j).find(mark.id)->second.Rvec.at<float>(2));
				sumX = sumX + (detectedHisto.at(i).find(mark.id)->second.Tvec.at<float>(0) - detectedHisto.at(j).find(mark.id)->second.Tvec.at<float>(0));
				sumY = sumY + (detectedHisto.at(i).find(mark.id)->second.Tvec.at<float>(1) - detectedHisto.at(j).find(mark.id)->second.Tvec.at<float>(1));
				sumZ = sumZ + (detectedHisto.at(i).find(mark.id)->second.Tvec.at<float>(2) - detectedHisto.at(j).find(mark.id)->second.Tvec.at<float>(2));
			}
		}
	}
	aruco::Marker m;
	bool found = false;
	if (computedHisto.front().find(mark.id) != computedHisto.front().end()) {
		m = computedHisto.front().find(mark.id)->second;
		found = true;
	}
	else if (computedHisto.at(1).find(mark.id) != computedHisto.front().end()) {
		m = computedHisto.at(1).find(mark.id)->second;
		found = true;
	}
	if (sumYaw < ANTI_FLICK_ROT && sumYaw > -ANTI_FLICK_ROT)
	{
		// std::cout << "sumYaw : " << sumYaw << std::endl;
		if (found)
			mark.Rvec.at<float>(0) = computedHisto.front().find(mark.id)->second.Rvec.at<float>(0);
	}
	if (sumPitch < ANTI_FLICK_ROT && sumPitch > -ANTI_FLICK_ROT)
	{
		// std::cout << "sumPitch : " << sumPitch << std::endl;
		if (found)
			mark.Rvec.at<float>(1) = computedHisto.front().find(mark.id)->second.Rvec.at<float>(1);
	}
	if (sumRoll < ANTI_FLICK_ROT && sumRoll > -ANTI_FLICK_ROT)
	{
		// std::cout << "sumRoll : " << sumRoll << std::endl;
		if (found)
			mark.Rvec.at<float>(2) = computedHisto.front().find(mark.id)->second.Rvec.at<float>(2);
	}
	if (sumX < ANTI_FLICK_TRANS && sumX > -ANTI_FLICK_TRANS)
	{
		// std::cout << "sumYaw : " << sumYaw << std::endl;
		if (found)
			mark.Tvec.at<float>(0) = computedHisto.front().find(mark.id)->second.Tvec.at<float>(0);
	}
	if (sumY < ANTI_FLICK_TRANS && sumY > -ANTI_FLICK_TRANS)
	{
		// std::cout << "sumPitch : " << sumPitch << std::endl;
		if (found)
			mark.Tvec.at<float>(1) = computedHisto.front().find(mark.id)->second.Tvec.at<float>(1);
	}
	if (sumZ < ANTI_FLICK_TRANS && sumZ > -ANTI_FLICK_TRANS)
	{
		// std::cout << "sumRoll : " << sumRoll << std::endl;
		if (found)
			mark.Tvec.at<float>(2) = computedHisto.front().find(mark.id)->second.Tvec.at<float>(2);
	}
	return (mark);
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
					next_map[p.first] = isInMotion(p.second);
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
		if (computedHisto.size() > 10)
		{
			computedHisto.pop_back();
		}
	}
	else
	{
		next_map = markerFoundCopy;
		computedHisto.insert(computedHisto.begin(), markerFoundCopy);
	}
	if (detectedHisto.size() > 10)
	{
		detectedHisto.pop_back();
	}
	detectedHisto.insert(detectedHisto.begin(), markerFoundCopy);
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

void		ARManager::addHexapat(int id, int basepat)
{
	Hexapat tmp(id, basepat);

	hexapats.push_back(tmp);
}
//---------------------------------------- GETTER & SETTER --------------------------------------//
bool ARManager::setFrame(cv::Mat p_frame)
{
	boost::mutex::scoped_lock	lock(this->m_marker);
	this->frame = p_frame.clone();
	this->frameChange = true;
	this->motionFrameChange = true;
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


