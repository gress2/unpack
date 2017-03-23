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
            _e0 = tuple<int, double>(3, 5.0); 
            _e1 = tuple<int, double>(123, 92.2);
        }
        vector<unpack<tuple<int, double>>> _v0;
        tuple<int, double> _e0, _e1;
};

TEST_F(UnpackTest, IsEmptyInitially) {
    ASSERT_EQ(_v0.size(), 0);
}

TEST_F(UnpackTest, SizeIncreasesOnPushBack) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    ASSERT_EQ(_v0.size(), 2); 
}

TEST_F(UnpackTest, BracketOperatorReturnsCorrectTuple) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    ASSERT_EQ(std::get<0>(_v0[1]), 123);
    ASSERT_EQ(std::is_reference
            <decltype(std::get<1>(_v0[0]))>::value, true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
