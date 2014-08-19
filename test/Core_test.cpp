#include <gtest/gtest.h>

#include "Core.hpp"

TEST(CoreTest, ArgcCorrect) {
    Core *c = new Core(3, 0);

    ASSERT_EQ(3, c->get_n_arg());
}

TEST(CoreTest, ArgcSameadLenArgv) {
    char* argv[] {"foo", "bar", "foo", '\0'};
    int len = 0;

    Core *c = new Core(3, argv);

    for (; argv[len]; len++) {
    }
    ASSERT_EQ(c->get_n_arg(), len);
}

TEST(CoreTest, InitCameraWorking) {
    Core *c = new Core(3, 0);

    c->init();

    ASSERT_EQ(c->get_camera().isOpen(), true);
    ASSERT_GT(c->get_camera().CameraGet(CV_CAP_PROP_FRAME_WIDTH, 0), 0);
    ASSERT_GT(c->get_camera().CameraGet(CV_CAP_PROP_FRAME_HEIGHT, 0), 0);

    c->get_camera().CloseCamera();

    delete c;
}

TEST(CoreTest, LoadDataBase) {
	Core *c = new Core(3, 0);

	c->init();
	c->startOgre();

	EXPECT_EQ(9, c->buildObjectsList("Data/db_test.xml"));

	c->get_camera().CloseCamera();

	delete c;
}
