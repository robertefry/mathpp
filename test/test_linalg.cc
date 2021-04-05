
#include "gtest/gtest.h"

#include <mathpp/linalg.hh>
using namespace mpp;

TEST(MPP_LINEAR_ALGEBRA, LINEAR_MAP)
{
    auto const matrix = Matrix<float,3,2>{1,2,3,4,5,6};
    auto const vector = Vector<float,2>{1,2};
    auto const covector = CoVector<float,3>{1,2,3};
    {
        auto const expected = Vector<float,3>{5,11,17};
        auto result = matrix * vector;
        EXPECT_TRUE(result == expected);
    }
    {
        auto const expected = CoVector<float,2>{22,28};
        auto result = covector * matrix;
        EXPECT_TRUE(result == expected);
    }
}

TEST(MPP_LINEAR_ALGEBRA, VECTORS)
{
    auto const vec = Vector<float,3>{1,2,3};
    auto const covec = CoVector<float,3>{1,2,3};
    {
        auto const expected = 14;
        auto result = covec * vec;
        EXPECT_TRUE(result == expected);
    }
    {
        auto const expected = Matrix<float,3,3>{1,2,3,2,4,6,3,6,9};
        auto result = vec * covec;
        EXPECT_TRUE(result == expected);
    }
}
