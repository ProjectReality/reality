#include	"Light.hpp"
#include	"Core.hpp"
#include	"aruco.h"

Core::Core(int iargc, char** iargv)
{
	rift = new VirtualOculus();
    argc = iargc;
    argv = iargv;
}

Core::~Core()
{
	ar.stop();
	if (render != NULL)
		delete render;
	if (rift != NULL)
		delete rift;
}


void Core::init()
{
    rift = rift->Init();
	camera.OpenCamera();
	video_size[0] = camera.CameraGet(CV_CAP_PROP_FRAME_WIDTH, 0);
	video_size[1] = camera.CameraGet(CV_CAP_PROP_FRAME_HEIGHT, 0); 
	render = new OgreRenderer(video_size, rift);
}

void Core::start()
{
	render->getGui()->start();

	if (render->getGui()->isExiting()) {
		render->getGui()->shutdown();
		return;
	}

    Logger::log("UI Launcher stoped, launchin reality...", Logger::info);

	startOgre();
	buildObjectsList();
    for (std::map<int, Object*>::iterator it = objects.begin(); it != objects.end(); it++)
        ar.addPatternInList(it->second->getName(), it->first);

	ar.start();
    Reality::Light	sun("sun", render->getScene());
	sun.createSun();
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
			for (std::pair<int, Object*> p : objects)
			{
				objects[p.first]->visible(false);
			}
			for (std::pair<int, aruco::Marker> p : mFound)
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

int Core::buildObjectsList(std::string filename)
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
					if (id > 1023)
					{
						if (!child->FirstChildElement("board"))
						{
							Logger::log_sev(Logger::error, "Core.cpp : Identifiant associé à une Board sans nom de board.");
							exit(-1);
						}
						ar.addBoard(id, child->FirstChildElement("board")->GetText());
					}
					objects[id] = new Object(child->FirstChildElement("pattern")->GetText(), child->FirstChildElement("asset")->GetText(), render->getScene());
					if (child->FirstChildElement("avisible"))
						(objects[id])->setAVisible(true);
					else
						(objects[id])->setAVisible(false);
					if (child->FirstChildElement("yaw"))
					{
						(objects[id])->setBaseYaw(Utils::c_to_d(child->FirstChildElement("yaw")->GetText()));
					}
					if (child->FirstChildElement("pitch"))
					{
						(objects[id])->setBasePitch(Utils::c_to_d(child->FirstChildElement("pitch")->GetText()));
					}
					if (child->FirstChildElement("roll"))
					{
						(objects[id])->setBaseRoll(Utils::c_to_d(child->FirstChildElement("roll")->GetText()));
					}
					if (child->FirstChildElement("scale"))
					{
						double s = Utils::c_to_d(child->FirstChildElement("scale")->GetText());
						(objects[id])->setScale(Ogre::Vector3(s, s, s));

					}
					patternCount++;
				}
				child = child->NextSiblingElement("object");
			}
		}
		else
		{
			Logger::log_sev(Logger::fatal, "Core.init() : No root node found");
		}
	}
	else
		Logger::log_sev(Logger::fatal, "Core.init() : Cannot load the database File");
	return patternCount;
}

int Core::get_n_arg()
{
    return this->argc;
}

StereoCamera Core::get_camera()
{
    return camera;
}

void Core::startOgre()
{
	render->startRealityRender();
	camera.GrabFrames();
}
