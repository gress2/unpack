#include <iostream>
#include <vector>
#include <type_traits>
#include "unpack.hpp"
#include "gtest/gtest.h"

using std::vector;
using std::tuple;

class UnpackTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
            _e0 = tuple<int, double>(3, 5.0f); 
        }
        vector<unpack<tuple<int, double>>> _v0;
        tuple<int, double> _e0;
};

TEST_F(UnpackTest, IsEmptyInitially) {
    ASSERT_EQ(_v0.size(), 0);
}

TEST_F(UnpackTest, SizeIncreasesOnPushBack) {
    _v0.push_back(_e0);
    ASSERT_EQ(_v0.size(), 1); 
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
