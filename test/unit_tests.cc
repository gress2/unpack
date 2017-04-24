#include <iostream>
#include <vector>
#include <type_traits>
#include <utility>
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

TEST_F(UnpackTest, PushBackCorrectlyInsertsElements) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    auto it = _v0.cbegin();
    ASSERT_EQ(*it, _e0);
    ASSERT_EQ(*(++it), _e1);
}

TEST_F(UnpackTest, PopBackRemovesLastElement) {
    _v0 = { _e0, _e1, _e2 };
    _v0.pop_back();
    ASSERT_EQ(_v0.size(), 2);
    ASSERT_EQ(_v0[1], _e1);
}

TEST_F(UnpackTest, SwapCorrectlySwapsElements) {
    _v0 = { _e0, _e1 };
    _v1 = { _e2, _e3 };
    _v0.swap(_v1);
    ASSERT_EQ(_v0[0], _e2);
    ASSERT_EQ(_v0[1], _e3); 
}

TEST_F(UnpackTest, StdSwapCorrectlySwapsElements) {
    _v0 = { _e0, _e1 };
    _v1 = { _e2, _e3 };
    using std::swap;
    swap(_v0, _v1);
    ASSERT_EQ(_v0[0], _e2);
    ASSERT_EQ(_v0[1], _e3);
}

TEST_F(UnpackTest, ClearRemovesAllElements) {
    _v0 = { _e0, _e1, _e2 };
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

TEST_F(UnpackTest, EmptyIsCorrect) {
    ASSERT_TRUE(_v0.empty());
    _v0.push_back(_e0);
    ASSERT_FALSE(_v0.empty());
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
    _v0 = { _e0, _e0 }; 
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
    _v0 = { _e0, _e1 };
    ASSERT_EQ(std::get<0>(_v0[1]), 123);
    ASSERT_TRUE(std::is_reference
            <decltype(std::get<1>(_v0[0]))>::value);
}

TEST_F(UnpackTest, ConstBracketOperatorReturnsCorrectTuple) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    ASSERT_EQ(std::get<0>(cv[1]), 123);
    ASSERT_TRUE(std::is_reference
            <decltype(std::get<1>(cv[0]))>::value);
}

TEST_F(UnpackTest, AtAccessorCorrect) {
    _v0 = { _e0, _e1 };
    ASSERT_EQ(std::get<0>(_v0.at(1)), 123);
    ASSERT_TRUE(std::is_reference
            <decltype(std::get<1>(_v0.at(0)))>::value);
}

TEST_F(UnpackTest, ConstAtAccessorCorrect) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    ASSERT_EQ(std::get<0>(cv.at(1)), 123);
    ASSERT_TRUE(std::is_reference
            <decltype(std::get<1>(cv.at(0)))>::value);
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

TEST_F(UnpackTest, ConstAtAccessorThrowsExceptionWhenOutOfBounds) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    try {
        cv.at(100);
        FAIL();
    } catch (const std::out_of_range& expected) {
        // should be thrown/caught
    }
}

TEST_F(UnpackTest, FrontAccessorReturnsFirstElement) {
    _v0 = { _e0, _e1, _e2 };
    ASSERT_EQ(_v0.front(), _e0);
}

TEST_F(UnpackTest, ConstFrontAccessorReturnsFirstElement) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    ASSERT_EQ(cv.front(), _e0);
}

TEST_F(UnpackTest, BackAccessorReturnsLastElement) {
    _v0 = { _e0, _e1, _e2 };
    ASSERT_EQ(_v0.back(), _e2);
}

TEST_F(UnpackTest, ConstBackAccessorReturnsFirstElement) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    ASSERT_EQ(cv.back(), _e2);
}

TEST_F(UnpackTest, CopyConstructorCorrect) {
    _v0 = { _e0, _e1 };
    vector<unpack<tuple<int, double>>> _v2(_v0); 
    ASSERT_EQ(_v0[0], _v2[0]);
    ASSERT_EQ(_v0.size(), _v2.size());
}

TEST_F(UnpackTest, AssignmentCorrectForLValRef) {
    _v0 = { _e0, _e1 };
    _v1 = _v0; 
    ASSERT_EQ(_v1[0], _v0[0]);
    ASSERT_EQ(_v1.size(), _v0.size());
}

TEST_F(UnpackTest, AssignmentCorrectForRValRef) {
    _v0 = { _e0, _e1 };
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
    _v0 = { _e0, _e1 };
    vector<unpack<tuple<int, double>>> _v2(std::move(_v0));
    ASSERT_EQ(_v2[0], _e0);
    ASSERT_EQ(_v2.size(), 2);
}

TEST_F(UnpackTest, IteratorBeginCorrect) {
    _v0.push_back(_e0);
    auto it = _v0.begin(); 
    ASSERT_EQ(*it, _e0);
}

TEST_F(UnpackTest, ConstIteratorBeginCorrect) {
     const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
     auto it = cv.begin();
     ASSERT_EQ(*it, _e0);
}

TEST_F(UnpackTest, ConstIteratorCbeginCorrect) {
     const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
     auto it = cv.cbegin();
     ASSERT_EQ(*it, _e0);
}

TEST_F(UnpackTest, IteratorPrefixIncrCorrect) {
    _v0 = { _e0, _e1 };
    auto it = _v0.begin();
    ASSERT_EQ(*(++it), _e1); 
}

TEST_F(UnpackTest, ConstIteratorPrefixIncrCorrect) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    auto it = cv.begin();
    ++it;
    ASSERT_EQ(*(++it), _e2); 
}

TEST_F(UnpackTest, IteratorPostfixIncrCorrect) {
    _v0.push_back(_e0);
    _v0.push_back(_e1);
    auto it = _v0.begin();
    ASSERT_EQ(*(it++), _e0); 
}

TEST_F(UnpackTest, ConstIteratorPostfixIncrCorrect) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    auto it = cv.begin();
    it++;
    ASSERT_EQ(*(it++), _e1); 
}

TEST_F(UnpackTest, IteratorPrefixDecrCorrect) {
    _v0 = { _e0, _e1 };
    auto it = _v0.end();
    ASSERT_EQ(*(--it), _e1); 
}

TEST_F(UnpackTest, ConstIteratorPrefixDecrCorrect) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    auto it = cv.end();
    ASSERT_EQ(*(--it), _e2); 
}

TEST_F(UnpackTest, IteratorPostfixDecrCorrect) {
    _v0 = { _e0, _e1 };
    auto it = _v0.end();
    it--;
    ASSERT_EQ(*(it--), _e1); 
}

TEST_F(UnpackTest, ConstIteratorPostfixDecrCorrect) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    auto it = cv.end();
    it--;
    ASSERT_EQ(*(it--), _e2); 
}

TEST_F(UnpackTest, IteratorEqualityCorrect) {
    _v0 = { _e0, _e1, _e2 };
    auto it0 = _v0.begin();
    auto it1 = _v0.begin(); 
    ASSERT_TRUE(it0 == it1);
    it0++;
    it1++;
    ASSERT_TRUE(it0 == it1);
    it1++;
    ASSERT_FALSE(it0 == it1);
}

TEST_F(UnpackTest, ConstIteratorEqualityCorrect) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    auto it0 = cv.begin();
    auto it1 = cv.begin(); 
    ASSERT_TRUE(it0 == it1);
    it0++;
    it1++;
    ASSERT_TRUE(it0 == it1);
    it1++;
    ASSERT_FALSE(it0 == it1);
}

TEST_F(UnpackTest, IteratorInequalityCorrect) {
    _v0 = { _e0, _e1, _e2 };
    auto it0 = _v0.begin();
    auto it1 = _v0.begin(); 
    ASSERT_FALSE(it0 != it1);
    it0++;
    it1++;
    ASSERT_FALSE(it0 != it1);
    it1++;
    ASSERT_TRUE(it0 != it1);
}

TEST_F(UnpackTest, ConstIteratorInequalityCorrect) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    auto it0 = cv.begin();
    auto it1 = cv.begin(); 
    ASSERT_FALSE(it0 != it1);
    it0++;
    it1++;
    ASSERT_FALSE(it0 != it1);
    it1++;
    ASSERT_TRUE(it0 != it1);
}

TEST_F(UnpackTest, IteratorEndCorrect) {
    _v0 = { _e0, _e1, _e2 };
    auto it0 = _v0.begin();
    auto it1 = _v0.end();
    ASSERT_FALSE(it0 == it1);
    it0 = _v0.end();
    ASSERT_TRUE(it0 == it1);
}

TEST_F(UnpackTest, ConstIteratorEndCorrect) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    auto it0 = cv.begin();
    auto it1 = cv.end();
    ASSERT_FALSE(it0 == it1);
    it0 = cv.end();
    ASSERT_TRUE(it0 == it1);
}

TEST_F(UnpackTest, ConstIteratorCendCorrect) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    auto it0 = cv.begin();
    auto it1 = cv.cend();
    ASSERT_FALSE(it0 == it1);
    it0 = cv.cend();
    ASSERT_TRUE(it0 == it1);
}

TEST_F(UnpackTest, IteratorDistanceCorrect) {
    _v0 = { _e0, _e1, _e2 };
    auto it0 = _v0.begin();
    auto it1 = _v0.begin();
    ASSERT_EQ(std::distance(it0, it1), 0);
    std::advance(it1, 2);
    ASSERT_EQ(std::distance(it0, it1), 2);
}

TEST_F(UnpackTest, ConstIteratorDistanceCorrect) {
    const vector<unpack<tuple<int, double>>> cv = { _e0, _e1, _e2 };
    auto it0 = cv.begin();
    auto it1 = cv.begin();
    ASSERT_EQ(std::distance(it0, it1), 0);
    std::advance(it1, 2);
    ASSERT_EQ(std::distance(it0, it1), 2);
}

TEST_F(UnpackTest, IteratedTuplesMutable) {
    _v0 = { _e0, _e1, _e2 };
    auto it = _v0.begin();
    *it = _e2;
    ASSERT_EQ(*it, _e2);
    *(++it) = _e0;
    ASSERT_EQ(*it, _e0);
    ASSERT_EQ(*(_v0.begin()), _e2);
}

TEST_F(UnpackTest, InsertPlacesTupleInVectorAndReturnsIter) {
    _v0 = { _e0, _e1, _e2 };
    auto it = _v0.cbegin();
    it++;
    auto pos = _v0.insert(it, _e3);
    ASSERT_EQ(*pos, _e3);
    ASSERT_EQ(*pos, *it);
    ASSERT_EQ(*(--pos), _e0);

    it = _v0.cbegin();
    std::advance(it, 2);
    pos = _v0.insert(it, _e4);
    ASSERT_EQ(*pos, _e4);
    ASSERT_EQ(*(--pos), _e3);
    std::advance(pos, 2);
    ASSERT_EQ(*pos, _e1); 
}

TEST_F(UnpackTest, RValInsertPlacesTupleInVectorAndReturnsIter) {
    _v0 = { _e0, _e1, _e2 };
    auto it = _v0.cbegin();
    it++;
    auto pos = _v0.insert(it, tuple<int, double>(8, 98.4));
    ASSERT_EQ(*pos, _e3);
    ASSERT_EQ(*(--pos), _e0);
    pos++;
    ASSERT_EQ(*(++pos), _e1);
    ASSERT_EQ(_v0.size(), 4);
    it = _v0.cend();
    it--;
    pos = _v0.insert(it, tuple<int, double>(7, 2));
    ASSERT_EQ(*pos, (tuple<int, double>(7, 2)));
    ASSERT_EQ(_v0.size(), 5);
}

TEST_F(UnpackTest, InsertCountCorrect) {
    _v0 = { _e0, _e1, _e2 };
    auto it = _v0.cbegin();
    it++;
    auto pos = _v0.insert(it, 3, _e3);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(*(pos++), _e3);
    }
    ASSERT_EQ(*pos, _e1);
    ASSERT_EQ(_v0.size(), 6);
}

TEST_F(UnpackTest, InsertRangeCorrect) {
    _v0 = { _e0, _e1, _e2, _e3 };
    _v1 = { _e0, _e1 };

    auto _v0_target = _v0.cbegin();
    _v0_target++; 
    auto res_it = _v0.insert(_v0_target, _v1.begin(), _v1.end());
    ASSERT_EQ(*res_it, *_v1.begin());
    ASSERT_EQ(*res_it, _e0);
    ASSERT_EQ(_v0.size(), 6);
    ASSERT_EQ(*(--res_it), _e0);
    ASSERT_EQ((std::get<0>(*res_it)), 3);
    ASSERT_EQ((std::get<1>(*res_it)), 5); 

    _v0.clear();
    _v1.clear();
    _v0 = { _e0, _e1, _e2, _e3 };
    _v1 = { _e0, _e1 };

    auto _v1_target = _v1.cbegin();
    _v1_target++;
    auto _v0_start = ++(_v0.begin());
    res_it = _v1.insert(_v1_target, _v0_start, _v0.end());
    ASSERT_EQ(_v1.size(), 5);
    res_it--;
    ASSERT_EQ(*res_it, _e0);
}

TEST_F(UnpackTest, InsertIListCorrect) {
    _v0 = { _e0, _e1, _e2 };
    auto _v0_target = ++(_v0.cbegin());
    auto res_it = _v0.insert(_v0_target, { _e0, _e1 });
    ASSERT_EQ(_v0.size(), 5);
    ASSERT_EQ(*res_it, _e0);
    ASSERT_EQ(*(--res_it), _e0);
    std::advance(res_it, 2);
    ASSERT_EQ(*res_it, _e1);
    ASSERT_EQ(*(++res_it), _e1);

    _v0.clear();
    _v0 = { _e0, _e1, _e2 };
    res_it = _v0.insert(_v0.cbegin(), { _e4 });
    ASSERT_EQ(_v0.size(), 4);
    ASSERT_EQ(*res_it, _e4);
    ASSERT_EQ(*(++res_it), _e0);
    ASSERT_EQ(*(++res_it), _e1);
}

TEST_F(UnpackTest, EmplaceBackCorrect) {
    _v0 = { _e0 };
    auto ref = _v0.emplace_back(6, 7);
    ASSERT_EQ(std::get<0>(ref), 6);
    ASSERT_EQ(std::get<1>(ref), 7);
    ref = _v0.emplace_back(0, 10000);
    ASSERT_EQ(std::get<0>(ref), 0);
    ASSERT_EQ(std::get<1>(ref), 10000);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
