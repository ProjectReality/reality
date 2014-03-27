#include "ARManager.h"


//---------------------------------------- CONSTRUCTOR --------------------------------------//
ARManager::ARManager()
{
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
					AssetInfo	tmpAss = AssetInfo(AssetInfo(child->FirstChildElement("pattern")->GetText(), child->FirstChildElement("asset")->GetText()));
					tmpAss.setId(arLoadPatt(tmpAss.getPattName().c_str()));
					if (tmpAss.getId() != -1)
						markerList.insert(std::pair<int, AssetInfo>(tmpAss.getId(), tmpAss));
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
		std::cout << "Try to lauch the AR thread" << std::endl;
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
		std::cout << "Loop start" << std::endl;
		if (ar->frameChange && ar->frame != NULL)
		{
			ARMarkerInfo    *a_markerInfo;
			int				markerNum;

			ar->m_marker.lock();
			ar->frameChange = false;
			ar->m_marker.unlock();

			if (arDetectMarker(ar->getFrame(), ar->getTresh(), &a_markerInfo, &markerNum) == 0)
			{
				ar->clearMarker();
				while (markerNum > 0)
				{
					markerNum--;
					ar->addMarker(a_markerInfo[markerNum]);
				}
				ar->validMarkers();
			}
		}
#ifdef _WIN32
		Sleep(10);
#else
		usleep(10)
#endif
		std::cout << "Loop End" << std::endl;
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

//---------------------------------------- GETTER & SETTER --------------------------------------//
bool ARManager::setFrame(ARUint8 *p_frame)
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
	if (this->markerChange && this->m_marker.try_lock())
	{
		this->markerChange = false;
		this->m_marker.unlock();
		return (this->markerFound);
	}
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