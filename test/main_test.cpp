#include <gtest/gtest.h>

#include "Core.hpp"

TEST(CoreTest, ArgCorrect) {
    Core *c = new Core(3, 0);

    ASSERT_EQ(3, c->get_n_arg());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
