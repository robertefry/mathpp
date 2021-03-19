
#include "gtest/gtest.h"
#include <mathpp/polyconv.hh>

TEST(MPP_POLYCONV, LIFETIME)
{
    {
        auto vec = std::vector<int>{0};
        auto poly = mpp::PolyConv<int,5>{};                     // default
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto vec = std::vector<int>{1,1,1,0,0};
        auto poly = mpp::PolyConv<int,5>(3,1);                  // fill smaller
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto vec = std::vector<int>{2,2,2,1,1};
        auto poly = mpp::PolyConv<int,5>(8,1);                  // fill bigger
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto vec = std::vector<int>{1,2,3,4,5};
        auto poly = mpp::PolyConv<int,5>{1,2,3,4,5};            // forward coeffs
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto vec = std::vector<int>{1,2,3,4,5};
        auto poly = mpp::PolyConv<int,5>{{1,2,3,4,5}};          // move coeffs
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto vec = std::vector<int>{1,2,3,4,5};
        auto poly = mpp::PolyConv<int,5>{vec};                  // copy coeffs
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto poly1 = mpp::PolyConv<int,5>{1,2,3,4,5};
        auto poly2 = mpp::PolyConv<int,5>{poly1};               // copy construct
        EXPECT_EQ(poly1.coeffs(), poly2.coeffs());
    }
    {
        auto poly1 = mpp::PolyConv<int,5>{1,2,3,4,5};
        auto poly2 = mpp::PolyConv<int,5>{};
        poly2 = poly1;                                          // copy assign
        EXPECT_EQ(poly1.coeffs(), poly2.coeffs());
    }
    {
        auto poly1 = mpp::PolyConv<int,5>{1,2,3,4,5};
        auto poly2 = mpp::PolyConv<int,5>{std::move(poly1)};    // move construct
        EXPECT_EQ(poly1.coeffs(), poly2.coeffs());
    }
    {
        auto poly1 = mpp::PolyConv<int,5>{1,2,3,4,5};
        auto poly2 = mpp::PolyConv<int,5>{};
        poly2 = std::move(poly1);                               // move assign
        EXPECT_EQ(poly1.coeffs(), poly2.coeffs());
    }
    {
        auto poly1 = mpp::PolyConv<int,5>{1,2,3,4,5};
        auto poly2 = mpp::PolyConv<float,7>{poly1};             // copy cast construct
        EXPECT_TRUE(poly1 == poly2);
    }
    {
        auto poly1 = mpp::PolyConv<int,5>{1,2,3,4,5};
        auto poly2 = mpp::PolyConv<float,7>{};
        poly2 = poly1;                                          // copy cast assign
        EXPECT_TRUE(poly1 == poly2);
    }
    {
        auto poly1 = mpp::PolyConv<int,5>{1,2,3,4,5};
        auto poly2 = mpp::PolyConv<float,7>{std::move(poly1)};  // move cast construct
        EXPECT_TRUE(poly1 == poly2);
    }
    {
        auto poly1 = mpp::PolyConv<int,5>{1,2,3,4,5};
        auto poly2 = mpp::PolyConv<float,7>{};
        poly2 = std::move(poly1);                               // move cast assign
        EXPECT_TRUE(poly1 == poly2);
    }
}

TEST(MPP_POLYCONV, OPERATORS)
{
    {
        auto poly = mpp::PolyConv<int,5>{1,2,3,4,5};
        EXPECT_EQ(poly.order(), 4);
        EXPECT_EQ(poly.size(), 5);
    }
    {
        auto poly = mpp::PolyConv<int,5>{1,2,3,0,0};
        EXPECT_EQ(poly.order(), 2);
        EXPECT_EQ(poly.size(), 5);
    }
    {
        auto poly = mpp::PolyConv<int,5>{1};
        EXPECT_EQ(poly.order(), 0);
        EXPECT_EQ(poly.size(), 5);
    }
    {
        auto poly = mpp::PolyConv<int,5>{0};
        EXPECT_EQ(poly.order(), 0);
        EXPECT_EQ(poly.size(), 5);
    }
    {
        auto poly = mpp::PolyConv<int,5>{1,2,3,4,5};
        EXPECT_EQ(poly[1], 2);
        EXPECT_EQ(poly[6], 2);
    }
}
