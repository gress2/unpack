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
            _e3 = tuple<int, double>(8, 98.4);
            _e4 = tuple<int, double>(56, 62.0);
        }
        vector<unpack<tuple<int, double>>> _v0;
        vector<unpack<tuple<int, double>>> _v1;
        tuple<int, double> _e0, _e1, _e2, _e3, _e4;
};

TEST_F(UnpackTest, IsEmptyInitially) {
    ASSERT_EQ(_v0.size(), 0);
}

TEST_F(UnpackTest, SizeIncreasesOnPushBack) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    ASSERT_EQ(_v0.size(), 2); 
}

TEST_F(UnpackTest, PopBackRemovesLastElement) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    _v0.push_back(_e2);
    _v0.pop_back();
    ASSERT_EQ(_v0.size(), 2);
    ASSERT_EQ(_v0[1], _e1);
}

TEST_F(UnpackTest, SwapCorrectlySwapsElements) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    _v1.push_back(_e2);
    _v1.push_back(_e3);
    _v0.swap(_v1);
    ASSERT_EQ(_v0[0], _e2);
    ASSERT_EQ(_v0[1], _e3); 
}

TEST_F(UnpackTest, StdSwapCorrectlySwapsElements) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    _v1.push_back(_e2);
    _v1.push_back(_e3);
    std::swap(_v0, _v1);
    ASSERT_EQ(_v0[0], _e2);
    ASSERT_EQ(_v0[1], _e3);
}

TEST_F(UnpackTest, ClearRemovesAllElements) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    _v0.push_back(_e2);
    _v0.clear();
    ASSERT_EQ(_v0.size(), 0);
}

TEST_F(UnpackTest, CapacityIsSane) {
    for (int i = 0; i < 1000; i++) {
        _v0.push_back(_e0);
    }
    ASSERT_GE(_v0.capacity(), 1000);
}

TEST_F(UnpackTest, MaxSizeIsSane) {
    ASSERT_GT(_v0.max_size(), 0);
}

TEST_F(UnpackTest, ResizeIncreasesElementsCorrectly) {
    _v0.resize(1000);
    ASSERT_EQ(_v0.size(), 1000);
}

TEST_F(UnpackTest, ResizeReducesElementsCorrectly) {
    for (int i = 0; i < 1000; i++) {
        _v0.push_back(_e0);
    }
    _v0.resize(500);
    ASSERT_EQ(_v0.size(), 500);
}

TEST_F(UnpackTest, ReserveIncreasesCapacity) {
    _v0.push_back(_e0);
    _v0.push_back(_e0);
    size_t new_size = _v0.capacity() + 1000;
    _v0.reserve(new_size);
    ASSERT_EQ(_v0.capacity(), new_size);
}

TEST_F(UnpackTest, ShrinkToFitReducesCapacityWhenAble) {
    _v0.reserve(1000);
    _v0.shrink_to_fit();
    ASSERT_EQ(_v0.capacity(), 0);
}

TEST_F(UnpackTest, BracketOperatorReturnsCorrectTuple) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    ASSERT_EQ(std::get<0>(_v0[1]), 123);
    ASSERT_TRUE(std::is_reference
            <decltype(std::get<1>(_v0[0]))>::value);
}

TEST_F(UnpackTest, AtAccessorCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    ASSERT_EQ(std::get<0>(_v0.at(1)), 123);
    ASSERT_TRUE(std::is_reference
            <decltype(std::get<1>(_v0.at(0)))>::value);
}

TEST_F(UnpackTest, AtAccessorThrowsExceptionWhenOutOfBounds) {
    _v0.push_back(_e0);
    try {
        _v0.at(100);
        FAIL();
    } catch (const std::out_of_range& expected) {
        // should be thrown/caught
    }
}

TEST_F(UnpackTest, FrontAccessorReturnsFirstElement) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    _v0.push_back(_e2);
    ASSERT_EQ(_v0.front(), _e0);
}

TEST_F(UnpackTest, BackAccessorReturnsLastElement) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    _v0.push_back(_e2);
    ASSERT_EQ(_v0.back(), _e2);
}

TEST_F(UnpackTest, CopyConstructorCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    vector<unpack<tuple<int, double>>> _v2(_v0); 
    ASSERT_EQ(_v0[0], _v2[0]);
    ASSERT_EQ(_v0.size(), _v2.size());
}

TEST_F(UnpackTest, AssignmentCorrectForLValRef) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    _v1 = _v0; 
    ASSERT_EQ(_v1[0], _v0[0]);
    ASSERT_EQ(_v1.size(), _v0.size());
}

TEST_F(UnpackTest, AssignmentCorrectForRValRef) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    _v1 = std::move(_v0); 
    ASSERT_EQ(_v1[0], _e0);
    ASSERT_EQ(_v1.size(), 2);
}

TEST_F(UnpackTest, AssignmentCorrectForIList) {
    _v0 = { _e0, _e1, _e2, _e3 };
    ASSERT_EQ(_v0[0], _e0);
    ASSERT_EQ(_v0[2], _e2);
}

TEST_F(UnpackTest, MoveConstructorCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    vector<unpack<tuple<int, double>>> _v2(std::move(_v0));
    ASSERT_EQ(_v2[0], _e0);
    ASSERT_EQ(_v2.size(), 2);
}

TEST_F(UnpackTest, IteratorBeginCorrect) {
    _v0.push_back(_e0);
    auto it = _v0.begin(); 
    ASSERT_EQ(*it, _e0);
}

TEST_F(UnpackTest, IteratorPrefixIncrCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    auto it = _v0.begin();
    ASSERT_EQ(*(++it), _e1); 
}

TEST_F(UnpackTest, IteratorPostfixIncrCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    auto it = _v0.begin();
    ASSERT_EQ(*(it++), _e0); 
}

TEST_F(UnpackTest, IteratorPrefixDecrCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    auto it = _v0.end();
    ASSERT_EQ(*(--it), _e1); 
}

TEST_F(UnpackTest, IteratorPostfixDecrCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    auto it = _v0.end();
    it--;
    ASSERT_EQ(*(it--), _e1); 
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
