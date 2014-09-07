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
	delete render;
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

    std::cout << "UI Launcher stoped, launchin reality..." << std::endl;

    render->startRealityRender();
    camera.GrabFrames();
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
			render->loadCam(frame[0], frame[0]);
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
					if (id > 1023)
					{
						if (!child->FirstChildElement("board"))
						{
							std::cerr << "Core.cpp : Identifiant associé à une Board sans nom de board." << std::endl;
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

int Core::get_n_arg()
{
    return this->argc;
}

StereoCamera Core::get_camera()
{
    return camera;
}
