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
            _e2 = tuple<int, double>(74, 84.8);
        }
        vector<unpack<tuple<int, double>>> _v0;
        tuple<int, double> _e0, _e1, _e2;
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
    ASSERT_TRUE(std::is_reference
            <decltype(std::get<1>(_v0[0]))>::value);
}

TEST_F(UnpackTest, IteratorBeginCorrect) {
    _v0.push_back(_e0);
    auto it = _v0.begin(); 
    ASSERT_EQ(std::get<0>(*it), 3);
}

TEST_F(UnpackTest, IteratorIncrementCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    auto it = _v0.begin();
    it++;
    ASSERT_EQ(std::get<0>(*it), 123);
}

TEST_F(UnpackTest, IteratorPrefixIncrCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    auto it = _v0.begin();
    ASSERT_EQ(std::get<0>(*(++it)), 123); 
}

TEST_F(UnpackTest, IteratorPostfixIncrCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    auto it = _v0.begin();
    ASSERT_EQ(std::get<0>(*(it++)), 3); 
}

TEST_F(UnpackTest, IteratorEqualityCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    _v0.push_back(_e2);
    auto it0 = _v0.begin();
    auto it1 = _v0.begin(); 
    ASSERT_TRUE(it0 == it1);
    it0++;
    it1++;
    ASSERT_TRUE(it0 == it1);
    it1++;
    ASSERT_FALSE(it0 == it1);
}

TEST_F(UnpackTest, IteratorInequalityCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    _v0.push_back(_e2);
    auto it0 = _v0.begin();
    auto it1 = _v0.begin(); 
    ASSERT_FALSE(it0 != it1);
    it0++;
    it1++;
    ASSERT_FALSE(it0 != it1);
    it1++;
    ASSERT_TRUE(it0 != it1);
}

TEST_F(UnpackTest, IteratorEndCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    _v0.push_back(_e2);
    auto it0 = _v0.begin();
    auto it1 = _v0.end();
    ASSERT_FALSE(it0 == it1);
    it0 = _v0.end();
    ASSERT_TRUE(it0 == it1);
}

TEST_F(UnpackTest, IteratorDistanceCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    _v0.push_back(_e2);
    auto it0 = _v0.begin();
    auto it1 = _v0.begin();
    ASSERT_EQ(std::distance(it0, it1), 0);
    it1++;
    it1++;
    ASSERT_EQ(std::distance(it0, it1), 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
