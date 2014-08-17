#include <gtest/gtest.h>

#include "Core.hpp"

TEST(CoreTest, ArgcCorrect) {
    Core *c = new Core(3, 0);

    ASSERT_EQ(3, c->get_n_arg());
}

TEST(CoreTest, ArgcSameadLenArgv) {
    char* argv[] {"foo", "bar", "foo"};
    int len = 0;

    Core *c = new Core(3, argv);

    for (; argv[len]; len++) {
    }
    ASSERT_TRUE(c->get_n_arg() == len);
}

TEST(CoreTest, FOO) {

}
