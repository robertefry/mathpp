
#include "gtest/gtest.h"

#include <mathpp/gcd.hh>

TEST(MPP_GCD, INTEGERS)
{
    {
        auto d = mpp::gcd<int>(123,45);
        EXPECT_EQ(d, 3);
    }
    {
        auto d = mpp::gcd<int>(45,123);
        EXPECT_EQ(d, 3);
    }
}

TEST(MPP_GCD_EXTENDED, INTEGERS)
{
    {
        auto [x,y] = mpp::gcd_extended<int>(123,45);
        EXPECT_EQ(x, -4);
        EXPECT_EQ(y, 11);
    }
    {
        auto [y,x] = mpp::gcd_extended<int>(45,123);
        EXPECT_EQ(x, -4);
        EXPECT_EQ(y, 11);
    }
}
