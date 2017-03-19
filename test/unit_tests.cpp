#include <iostream>
#include "unpack.hpp"
#include "gtest/gtest.h"

TEST(MyFirstTest, all) {
    EXPECT_EQ(0, 0);
}

TEST(MySecondTest, all) {
    EXPECT_EQ(1, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
