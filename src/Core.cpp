#include	"Core.hpp"
#include	"aruco.h"

Core::Core()
{
	rift = new VirtualOculus();
}

Core::~Core()
{
	ar.stop();
	delete render;
}


void Core::init()
{
	rift = rift->Init();
	camera.OpenCamera();
	video_size[0] = camera.CameraGet(CV_CAP_PROP_FRAME_WIDTH, 0);
	video_size[1] = camera.CameraGet(CV_CAP_PROP_FRAME_HEIGHT, 0); 
	render = new OgreRenderer(video_size, rift);
	camera.GrabFrames();
	buildObjectsList();
	for (std::map<int, Object*>::iterator it = objects.begin(); it != objects.end(); it++)
		ar.addPatternInList(it->second->getName(), it->first);
}


void Core::start()
{
	ar.start();
	while (render->isAlive())
	{
		if (render->getShutDown())
		{
			return ;
		}
		if (camera.FrameAvailable())
		{
			frame = camera.GetFrame();
			ar.setFrame(frame[0]);
			std::map<int, aruco::Marker> mFound = ar.getMarkers();
			for each(std::pair<int, aruco::Marker> p in mFound)
			{
				if (objects.find(p.first) != objects.end())
				{
					objects[p.first]->updateData(p.second);
					objects[p.first]->visible(true);
				}
			}
			render->loadCam(frame[0], frame[1]);
			boost::thread new_pic(&StereoCamera::camWorker, camera);
		}
		render->render();
	}
}

void Core::buildObjectsList(std::string filename)
{
    int patternCount = 0;
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
					int id = Utils::c_to_i(child->FirstChildElement("pattern")->GetText());
					objects[id] = new Object(child->FirstChildElement("pattern")->GetText(), child->FirstChildElement("asset")->GetText(), render->getScene());
					if (child->FirstChildElement("avisible"))
						(objects[id])->setAVisible(true);
					else
						(objects[id])->setAVisible(false);
					if (child->FirstChildElement("yaw"))
					{
						(objects[id])->setBaseYaw(Utils::c_to_d(child->FirstChildElement("yaw")->GetText()));
						(objects[id])->rotate(Ogre::Vector3(0, 0, 1), Ogre::Radian(Ogre::Degree((0))));
					}
					if (child->FirstChildElement("pitch"))
					{
						(objects[id])->setBasePitch(Utils::c_to_d(child->FirstChildElement("pitch")->GetText()));
						(objects[id])->rotate(Ogre::Vector3(0, 1, 0), Ogre::Radian(Ogre::Degree((objects[id])->getBasePitch())));
					}
					if (child->FirstChildElement("roll"))
					{
						(objects[id])->setBaseRoll(Utils::c_to_d(child->FirstChildElement("roll")->GetText()));
						(objects[id])->rotate(Ogre::Vector3(1, 0, 0), Ogre::Radian(Ogre::Degree((objects[id])->getBaseRoll())));
					}
					if (child->FirstChildElement("scale"))
					{
						double s = Utils::c_to_d(child->FirstChildElement("scale")->GetText());
						(objects[id])->setScale(Ogre::Vector3(s, s, s));

					}
				}
				child = child->NextSiblingElement("object");
			}
		}
		else
		{
			std::cerr << "Core.init() : No root node found" << std::endl;
		}
	}
	else
		std::cerr << "Core.init() : Cannot load the database File" << std::endl;
}