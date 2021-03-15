
#include "gtest/gtest.h"

#include <mathpp/gcd.hh>
#include <mathpp/poly.hh>

TEST(GCD, INTEGERS)
{
    {
        auto d = mpp::gcd<int>(123,45);
        EXPECT_EQ(d, 3);
    }
    {
        auto d = mpp::gcd<int>(45,123);
        EXPECT_EQ(d, 3);
    }
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

TEST(GCD, POLYNOMIALS)
{
    {
        auto poly1 = mpp::Poly<int>{1,1} * mpp::Poly<int>{1,2}; // (1+x)(1+2x)
        auto poly2 = mpp::Poly<int>{1,1} * mpp::Poly<int>{2,3}; // (1+x)(2+3x)
        auto poly3 = mpp::Poly<int>{1,1};
        auto gcd = mpp::gcd(poly1,poly2);
        EXPECT_EQ(gcd.coeffs(), poly3.coeffs());  // gcd of (1+x)(1+2x) and (1+x)(2+3x) is (1+x)
    }
    {
        auto poly1 = mpp::Poly<int>{1,1} * mpp::Poly<int>{1,2}; // (1+x)(1+2x)
        auto poly2 = mpp::Poly<int>{1,1} * mpp::Poly<int>{2,3}; // (1+x)(2+3x)
        auto poly3 = mpp::Poly<int>{1,1};
        auto gcd = mpp::gcd(poly2,poly1);
        EXPECT_EQ(gcd.coeffs(), poly3.coeffs());  // gcd of (1+x)(1+2x) and (1+x)(2+3x) is (1+x)
    }
    {
        auto poly1 = mpp::Poly<int>{1,1} * mpp::Poly<int>{1,2}; // (1+x)(1+2x)
        auto poly2 = mpp::Poly<int>{1,1} * mpp::Poly<int>{2,3}; // (1+x)(2+3x)
        auto poly3 = mpp::Poly<int>{-3};
        auto poly4 = mpp::Poly<int>{2};
        auto [x,y] = mpp::gcd_extended(poly2,poly1);
        EXPECT_EQ(x.coeffs(), poly3.coeffs());
        EXPECT_EQ(y.coeffs(), poly4.coeffs());
    }
    {
        auto poly1 = mpp::Poly<int>{1,1} * mpp::Poly<int>{1,2}; // (1+x)(1+2x)
        auto poly2 = mpp::Poly<int>{1,1} * mpp::Poly<int>{2,3}; // (1+x)(2+3x)
        auto poly3 = mpp::Poly<int>{-3};
        auto poly4 = mpp::Poly<int>{2};
        auto [y,x] = mpp::gcd_extended(poly1,poly2);
        EXPECT_EQ(x.coeffs(), poly3.coeffs());
        EXPECT_EQ(y.coeffs(), poly4.coeffs());
    }
}
