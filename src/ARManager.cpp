#include "ARManager.hpp"
#include "ARma/patterndetector.h"
#include "ARma/pattern.h"


//---------------------------------------- CONSTRUCTOR --------------------------------------//
ARManager::ARManager()
{
	this->detector = new ARma::PatternDetector(fixed_thresh, adapt_thresh, adapt_block_size, confidenceThreshold, norm_pattern_size, mode);
	CvMat* intrinsic = (CvMat*)cvLoad("Data/intrinsic.xml");
	CvMat* distor = (CvMat*)cvLoad("Data/distortion.xml");
	this->cameraMatrix = cvarrToMat(intrinsic);
	this->distortions = cvarrToMat(distor);
	this->patternCount = 0;

	this->frameChange = false;
	this->markerChange = false;

	this->norm_pattern_size = 64;
	this->fixed_thresh = 50;
	this->adapt_thresh = 5;
	this->adapt_block_size = 45;
	this->confidenceThreshold = 0.40;
	this->mode = 2;
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
                    loadPattern(child->FirstChildElement("pattern")->GetText(), patternLibrary, patternCount);
                    markerList.insert(std::pair<int, int>(patternCount - 1, 0));
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
//	if (verbose)
//        for (std::map<int, int>::iterator it = markerList.begin(); it != markerList.end(); *it++)
//		{
//			std::cout << "Object" << std::endl;
//			std::cout << "Pattern : " << it->second.getPattName() << std::endl;
//			std::cout << "Asset : " << it->second.getAssName() << std::endl;
//			std::cout << "Id : " << it->second.getId() << std::endl;
//			std::cout << "---------------------------" << std::endl;
//		}
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
			cv::Mat fr;
			if (true)
			{
				boost::mutex::scoped_lock lock(ar->m_marker);
				fr = ar->frame.clone();
				ar->frameChange = false;
			}
			ar->detector->detect(fr, ar->cameraMatrix, ar->distortions, ar->patternLibrary, ar->detectedPattern);

			if (!ar->detectedPattern.empty())
			{
				boost::mutex::scoped_lock lock(ar->m_marker);
				ar->clearMarker();
				if (verbose) {
					std::cout << "Number of pattern Found : " << ar->detectedPattern.size() << std::endl;
				}
				for (std::vector<ARma::Pattern>::iterator it = ar->detectedPattern.begin(); it != ar->detectedPattern.end(); it++)
				{
					ar->addMarker(*it);
				}
			}
		}
#ifdef _WIN32
		Sleep(10);
#else
		usleep(10);
#endif
		ar->detectedPattern.clear();
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
    std::map<int, int>::iterator	it = this->markerList.find(info.id);
	if (it != this->markerList.end())
	{
    if (verbose) std::cout << "Marker id Found : " << info.id << std::endl;
        this->markerFound.push_back(info);
		this->markerChange = true;
	}
}

int ARManager::loadPattern(const char* filename, std::vector<cv::Mat>& library, int& patternCount)
{
	Mat img = imread(filename, 0);

	if (img.data == NULL || img.cols != img.rows){
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

void ARManager::cleanVector(std::vector<ARma::Pattern>& p_Pattern)
{

}

int ARManager::calcDist(std::vector<ARma::Pattern>& p_Pattern)
{
	return (0);
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

void ARManager::setMarkerList(std::map<int, int> &markers)
{
	this->markerList = markers;
}

std::list<ARma::Pattern> ARManager::getMarkers()
{
	boost::mutex::scoped_lock lock(m_marker);
	if (this->markerChange)
	{
		this->markerChange = false;
	}
    std::list<ARma::Pattern> m(this->markerFound.begin(), this->markerFound.end());
	return (m);
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
	boost::mutex::scoped_lock lock(this->m_marker);
    for (std::list<ARma::Pattern>::iterator it = markerFound.begin(); it != markerFound.end(); it++)
        it->draw(frame, cameraMatrix, distortions);
}
