#include <gtest/gtest.h>

#include "Object.hpp"

TEST(ObjectEntityTest, ObjectCreation) {

	Object *c = new Object("1234", "BeerBarrel.mesh", NULL);

	ASSERT_TRUE(c != NULL);

	delete c;
}

//TEST(ObjectEntityTest, AddMeshObjectToScene) {
//
//
//	Ogre::Root* ogre = new Ogre::Root();
//	if (!ogre->restoreConfig()){
//		ogre->showConfigDialog();
//	}
//
//	Ogre::SceneManager*	mScene = ogre->createSceneManager(Ogre::ST_GENERIC);
//
//	Object *c = new Object("1234", "BeerBarrel.mesh", mScene);
//
//	ASSERT_TRUE(c != NULL);
//
//	delete c;
//}

