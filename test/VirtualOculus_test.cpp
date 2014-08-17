#include <gtest/gtest.h>

#include "VirtualOculus.hpp"

TEST(VirtualOculusTest, InitCorrect) {
    VirtualOculus *v = new VirtualOculus();

    v = v->Init();

    ASSERT_TRUE(v != NULL);

    delete v;
}

TEST(VirtualOculusTest, FakeDataifNoOulus) {
    VirtualOculus *v = new VirtualOculus();

    v = v->Init();

    if (v->getpHMD()) {
        ASSERT_TRUE(v->getResolution()[0] > 0);
        ASSERT_TRUE(v->getResolution()[1] > 0);
    } else {
        ASSERT_TRUE(v->getResolution()[0] == 800);
        ASSERT_TRUE(v->getResolution()[1] == 600);
    }

    delete v;
}
