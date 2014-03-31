#include "ARManager.hpp"


//---------------------------------------- CONSTRUCTOR --------------------------------------//
ARManager::ARManager()
{}

//---------------------------------------- MAIN FUNCTIONS --------------------------------------//

void ARManager::init(std::string filename)
{
//	char           *cparam_name = "Data/camera_para.dat";
//	char			*vconf = "Data\\WDM_camera_flipV.xml";
//	int             xsize, ysize;
//
//	/* set the initial camera parameters */
//	if (arParamLoad(cparam_name, 1, &cparam) < 0) {
//		printf("Camera parameter load error !!\n");
//		exit(0);
//	}
//
//	arInitCparam(&cparam);
//
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
					tmpAss.setId(arLoadPatt(tmpAss.getPattName().c_str()));
					if (tmpAss.getId() != -1)
					{
						markerList.insert(markerList.begin(), std::pair<int, AssetInfo>(tmpAss.getId(), tmpAss));
					}
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
		if (ar->frameChange && ar->frame != NULL)
		{
			std::cout << "Object 2" << std::endl;
			ARMarkerInfo    *a_markerInfo;
			int				markerNum;
			std::cout << arActivatePatt(ar->markerList.begin()->first) << std::endl;
			ar->m_marker.lock();
			ar->frameChange = false;


			std::cout << "Object 3" << std::endl;
			if (arDetectMarker(ar->getFrame(), ar->getTresh(), &a_markerInfo, &markerNum) >= 0)
			{
				std::cout << "Object 1" << std::endl;
				if (verbose) std::cout << "||||| Marker NUM : " << markerNum << std::endl;
				ar->clearMarker();
				while (markerNum > 0)
				{
					markerNum--;
					ar->addMarker(a_markerInfo[markerNum]);
				}
				ar->validMarkers();
			}
			ar->m_marker.unlock();
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

void ARManager::addMarker(ARMarkerInfo info)
{

	double								patt_trans[3][4];
	std::map<int, AssetInfo>::iterator	it = this->markerList.find(info.id);
	AssetInfo							ass = it->second;

	ass.setInfo(info);
	arGetTransMat(&info, ass.getCenter(), ass.getWidth(), patt_trans);
	ass.setTranslation(patt_trans);
	this->markerFound.push_back(ass);
}

void ARManager::validMarkers()
{
	this->m_marker.lock();
	this->markerChange = true;
	this->m_marker.unlock();
}

void ARManager::change(cv::Mat frame)
{
	this->m_marker.lock();
	arParamChangeSize(&cparam, frame.rows, frame.cols, &cparam);
	this->m_marker.unlock();
}

//---------------------------------------- GETTER & SETTER --------------------------------------//
bool ARManager::setFrame(ARUint8 *p_frame)
{
	if (this->m_marker.try_lock())
	{
		/*if (this->frame == NULL || strlen((char*)p_frame) > strlen((char*)this->frame))
		{
			this->frame = new ARUint8[strlen((char*)p_frame)];
		}
		strcpy((char*)this->frame, (char *)p_frame);*/
		this->frame = p_frame;
		this->frameChange = true;
		this->m_marker.unlock();
		return true;
	}
	return false;
}

ARUint8* ARManager::getFrame() const
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

void ARManager::setTresh(int t)
{
	this->threshold = t;
}

int ARManager::getTresh() const
{
	return (this->threshold);
}

//---------------------------------------- DESTRUCTOR --------------------------------------//
ARManager::~ARManager() {}