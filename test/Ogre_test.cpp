#include <gtest/gtest.h>

#include	"OgreRenderer.hpp"
#include "VirtualOculus.hpp"



TEST(OgreTest, CreatOgre) {

	Ogre::LogManager* logMgr = OGRE_NEW Ogre::LogManager;
	logMgr->createLog("DefaultLog", true, false, false);

	Ogre::Root* ogre = new Ogre::Root();

	ASSERT_TRUE(ogre != NULL);

	delete ogre;
}

TEST(OgreTest, CreatScene) {

	Ogre::LogManager* logMgr = OGRE_NEW Ogre::LogManager;
	logMgr->createLog("DefaultLog", true, false, false);

	Ogre::Root* ogre = new Ogre::Root();
	if (!ogre->restoreConfig()){
		ogre->showConfigDialog();
	}

	Ogre::SceneManager*	mScene = ogre->createSceneManager(Ogre::ST_GENERIC);

	ASSERT_TRUE(mScene != NULL);

	delete ogre;
}


TEST(OgreTest, CreatOgreRender) {

	VirtualOculus *v = new VirtualOculus();
	v = v->Init();

	double video_size[2];

	video_size[0] = 800;
	video_size[1] = 600;

	OgreRenderer *render = new OgreRenderer(video_size, v);

	ASSERT_TRUE(render != NULL);

	delete render;
	delete v;
}