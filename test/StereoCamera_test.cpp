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
    s->TestCamera();
}
