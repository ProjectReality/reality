#include <gtest/gtest.h>

#include "StereoCamera.hpp"


TEST(StereoCameraTest, Create) {
    StereoCamera *s = new StereoCamera();

    ASSERT_EQ(s->FrameAvailable(), false);
    ASSERT_EQ(s->isOpen(), false);

    delete s;
}

TEST(StereoCameraTest, OpenCamera) {
    StereoCamera *s = new StereoCamera();

    s->OpenCamera();


    ASSERT_GT(s->CameraGet(CV_CAP_PROP_FRAME_WIDTH, 0), 0);
    ASSERT_GT(s->CameraGet(CV_CAP_PROP_FRAME_WIDTH, 1), 0);
    ASSERT_GT(s->CameraGet(CV_CAP_PROP_FRAME_HEIGHT, 0), 0);
    ASSERT_GT(s->CameraGet(CV_CAP_PROP_FRAME_HEIGHT, 1), 0);

    EXPECT_EQ(s->getCamera(LEFT).isOpened(), s->getCamera(RIGHT).isOpened());
}
