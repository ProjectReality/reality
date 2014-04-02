#include "ARManager.hpp"
#include "ARma/patterndetector.h"
#include "ARma/pattern.h"


//---------------------------------------- CONSTRUCTOR --------------------------------------//
ARManager::ARManager()
{
	detector = new ARma::PatternDetector(fixed_thresh, adapt_thresh, adapt_block_size, confidenceThreshold, norm_pattern_size, mode);
}

//---------------------------------------- MAIN FUNCTIONS --------------------------------------//

void ARManager::init(std::string filename)
{
	if (verbose) std::cout << "ARManager.init() : Try to load database named " << filename.c_str() << std::endl;
	TiXmlDocument doc(filename.c_str());
	if (doc.LoadFile())
	{
		TiXmlElement* root = doc.FirstChildElement("db");
		if (root)
		{
			TiXmlElement*	child = root->FirstChildElement("object");
			while (child)
			{
				if (child->FirstChildElement("pattern") && child->FirstChildElement("asset"))
				{
					AssetInfo	tmpAss(child->FirstChildElement("pattern")->GetText(), child->FirstChildElement("asset")->GetText());
					loadPattern(tmpAss.getPattName().c_str(), patternLibrary, patternCount);
				}
				child = child->NextSiblingElement("object");
			}
		}
		else
		{
			std::cerr << "ARManager.init() : No root node found" << std::endl;
		}
	}
	else
		std::cerr << "ARManager.init() : Cannot load the database File" << std::endl;
	// Temp printer
	if (verbose)
		for (std::map<int, AssetInfo>::iterator it = markerList.begin(); it != markerList.end(); *it++)
		{
			std::cout << "Object" << std::endl;
			std::cout << "Pattern : " << it->second.getPattName() << std::endl;
			std::cout << "Asset : " << it->second.getAssName() << std::endl;
			std::cout << "---------------------------" << std::endl;
		}

}

void ARManager::start()
{
	if (verbose)
		std::cout << "Lauch the AR thread" << std::endl;
	this->arThread = boost::thread(&arLoop, this);
}

void ARManager::stop()
{
	if (verbose)
		std::cout << "AR thread Stop" << std::endl;
	this->arThread.interrupt();
}

void ARManager::arLoop(ARManager *ar)
{
	while (1)
	{
		if (ar->frameChange)
		{
			ar->m_marker.lock();
			ar->frameChange = false;
			ar->m_marker.unlock();

			ar->detector->detect(ar->frame, cameraMatrix, distortions, ar->patternLibrary, ar->detectedPattern);
			if (!ar->detectedPattern.empty())
			{
				if (verbose) std::cout << "||||| Marker NUM : " << ar->detectedPattern.size() << std::endl;
				ar->clearMarker();
				for (std::vector<ARma::Pattern>::iterator it = ar->detectedPattern.begin(); it != ar->detectedPattern.end(); it++)
				{
					ar->addMarker(*it);
				}
				ar->validMarkers();
			}
		}
#ifdef _WIN32
		Sleep(10);
#else
		usleep(10)
#endif
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

void ARManager::addMarker(ARma::Pattern info)
{

	double								patt_trans[3][4];
	std::map<int, AssetInfo>::iterator	it = this->markerList.find(info.id);
	AssetInfo							ass = it->second;

	ass.setInfo(info);
	this->markerFound.push_back(ass);
}

void ARManager::validMarkers()
{
	this->m_marker.lock();
	this->markerChange = true;
	this->m_marker.unlock();
}

int ARManager::loadPattern(const char* filename, std::vector<cv::Mat>& library, int& patternCount)
{
	Mat img = imread(filename, 0);

	if (img.cols != img.rows){
		std::cerr << "Not a square pattern" << std::endl;
		return -1;
	}

	Mat src(norm_pattern_size, norm_pattern_size, CV_8UC1);
	Point2f center((norm_pattern_size - 1) / 2.0f, (norm_pattern_size - 1) / 2.0f);
	Mat rot_mat(2, 3, CV_32F);

	resize(img, src, Size(norm_pattern_size, norm_pattern_size));
	Mat subImg = src(Range(norm_pattern_size / 4, 3 * norm_pattern_size / 4), Range(norm_pattern_size / 4, 3 * norm_pattern_size / 4));
	library.push_back(subImg);

	rot_mat = getRotationMatrix2D(center, 90, 1.0);

	for (int i = 1; i<4; i++){
		Mat dst = Mat(norm_pattern_size, norm_pattern_size, CV_8UC1);
		rot_mat = getRotationMatrix2D(center, -i * 90, 1.0);
		warpAffine(src, dst, rot_mat, Size(norm_pattern_size, norm_pattern_size));
		Mat subImg = dst(Range(norm_pattern_size / 4, 3 * norm_pattern_size / 4), Range(norm_pattern_size / 4, 3 * norm_pattern_size / 4));
		library.push_back(subImg);
	}

	patternCount++;
	return 1;
}

//---------------------------------------- GETTER & SETTER --------------------------------------//
bool ARManager::setFrame(cv::Mat p_frame)
{
	if (this->m_marker.try_lock())
	{
		this->frame = p_frame;
		this->frameChange = true;
		this->m_marker.unlock();
		return true;
	}
	return false;
}

cv::Mat ARManager::getFrame() const
{
	return (this->frame);
}

void ARManager::setMarkerList(std::map<int, AssetInfo> &markers)
{
	this->markerList = markers;
}

std::list<AssetInfo> ARManager::getMarkers()
{
	if (this->markerChange)
	{
		this->m_marker.lock();
		this->markerChange = false;
		this->m_marker.unlock();
	}
	return (this->markerFound);
}

int ARManager::getCount() const
{
	return (patternCount);
}

//---------------------------------------- DESTRUCTOR --------------------------------------//
ARManager::~ARManager() {}

//---------------------------------------- Debug Function --------------------------------------//
void ARManager::draw(cv::Mat& frame)
{
	for (std::list<AssetInfo>::iterator it = markerFound.begin(); it != markerFound.end(); it++)
		it->getInfo().draw(frame, cameraMatrix, distortions);
}