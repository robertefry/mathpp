
#include "gtest/gtest.h"

#include <mathpp/gcd.hh>
#include <mathpp/poly.hh>

TEST(GCD, INTEGERS)
{
    int d = mpp::gcd<int>(123,45);
    EXPECT_TRUE(d == 3);
}

TEST(GCD, POLYNOMIALS)
{
    auto poly1 = mpp::Poly<int>{1,1} * mpp::Poly<int>{1,2}; // (1+x)(1+2x)
    auto poly2 = mpp::Poly<int>{1,1} * mpp::Poly<int>{2,3}; // (1+x)(2+3x)
    auto poly3 = mpp::Poly<int>{1,1};
    auto gcd = mpp::gcd(poly1,poly2);
    EXPECT_TRUE(gcd == poly3);  // gcd of (1+x)(1+2x) and (1+x)(2+3x) is (1+x)
}
