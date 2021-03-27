
#include "gtest/gtest.h"
#include <mathpp/poly.hh>

TEST(MPP_POLY, LIFETIME)
{
    {
        auto vec = std::vector<int>{0};
        auto poly = mpp::Poly<int>{};                       // default
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto vec = std::vector<int>{1,1,1};
        auto poly = mpp::Poly<int>(3,1);                    // fill
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto vec = std::vector<int>{1,2,3,4,5};
        auto poly = mpp::Poly<int>{1,2,3,4,5};              // forward coeffs
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto vec = std::vector<int>{1,2,3,4,5};
        auto poly = mpp::Poly<int>{vec};                    // copy coeffs
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto vec = std::vector<int>{1,2,3,4,5};
        auto poly = mpp::Poly<int>{{1,2,3,4,5}};            // move coeffs
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto vec = std::vector<int>{1,2,3,4,5};
        auto poly = mpp::Poly<int>{};
        poly = {1,2,3,4,5};                                 // assign initializer_list
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto vec = std::vector<int>{1,2,3,4,5};
        auto poly = mpp::Poly<int>{};
        poly = vec;                                         // assign copy vector
        EXPECT_EQ(poly.coeffs(), vec);
    }
    {
        auto vec1 = std::vector<int>{1,2,3,4,5};
        auto vec2 = std::vector<int>{1,2,3,4,5};
        auto poly = mpp::Poly<int>{};
        poly = std::move(vec1);                             // assign move vector
        EXPECT_EQ(poly.coeffs(), vec2);
    }
    {
        auto poly1 = mpp::Poly<int>{1,2,3,4,5};
        auto poly2 = mpp::Poly<int>{poly1};                 // copy construct
        EXPECT_EQ(poly1.coeffs(), poly2.coeffs());
    }
    {
        auto vec = std::vector<int>{1,2,3,4,5};
        auto poly1 = mpp::Poly<int>{vec};
        auto poly2 = mpp::Poly<int>{std::move(poly1)};      // move construct
        EXPECT_EQ(poly2.coeffs(), vec);
    }
    {
        auto poly1 = mpp::Poly<int>{1,2,3,4,5};
        auto poly2 = mpp::Poly<int>{};
        poly2 = poly1;                                      // copy assign
        EXPECT_EQ(poly1.coeffs(), poly2.coeffs());
    }
    {
        auto vec = std::vector<int>{1,2,3,4,5};
        auto poly1 = mpp::Poly<int>{vec};
        auto poly2 = mpp::Poly<int>{};
        poly2 = std::move(poly1);                           // move assign
        EXPECT_EQ(poly2.coeffs(), vec);
    }
    {
        auto poly1 = mpp::Poly<int>{1,2,3,4,5};
        auto poly2 = mpp::Poly<float>{poly1};               // copy cast construct
        EXPECT_TRUE(poly1 == poly2);
    }
    {
        auto poly1 = mpp::Poly<int>{1,2,3,4,5};
        auto poly2 = mpp::Poly<int>{poly1};
        auto poly3 = mpp::Poly<float>{std::move(poly2)};    // move cast construct
        EXPECT_TRUE(poly1 == poly3);
    }
    {
        auto poly1 = mpp::Poly<int>{1,2,3,4,5};
        auto poly2 = mpp::Poly<float>{};
        poly2 = poly1;                                      // copy cast assign
        EXPECT_TRUE(poly1 == poly2);
    }
    {
        auto poly1 = mpp::Poly<int>{1,2,3,4,5};
        auto poly2 = mpp::Poly<int>{poly1};
        auto poly3 = mpp::Poly<float>{};
        poly3 = std::move(poly2);                           // move cast assign
        EXPECT_TRUE(poly1 == poly3);
    }
}

TEST(MPP_POLY, MATHPP)
{
    using mpp::op_add;  using mpp::op_mul;
    {
        using identity = mpp::identity<mpp::Poly<int>,mpp::op_add>;
        auto has = identity::has();
        EXPECT_TRUE(has);

        auto const poly = mpp::Poly<int>{0};

        auto poly1 = identity::get();
        EXPECT_TRUE(poly1 == poly);

        auto poly2 = mpp::Poly<int>{1,2,3};
        identity::make(poly2);
        EXPECT_TRUE(poly2 == poly);
    }
    {
        using identity = mpp::identity<mpp::Poly<int>,mpp::op_mul>;
        auto has = identity::has();
        EXPECT_TRUE(has);

        auto const poly = mpp::Poly<int>{1};

        auto poly1 = identity::get();
        EXPECT_TRUE(poly1 == poly);

        auto poly2 = mpp::Poly<int>{1,2,3};
        identity::make(poly2);
        EXPECT_TRUE(poly2 == poly);
    }
    {
        using inverse = mpp::inverse<mpp::Poly<int>,mpp::op_add>;
        auto has = inverse::has();
        EXPECT_TRUE(has);

        auto const poly = mpp::Poly<int>{-1,-2,-3};

        auto poly1 = mpp::Poly<int>{1,2,3};
        auto can = inverse::can(poly1);
        EXPECT_TRUE(can);

        auto poly2 = inverse::get(poly1);
        EXPECT_TRUE(poly2 == poly);

        inverse::make(poly1);
        EXPECT_TRUE(poly1 == poly);
    }
    {
        // Polynomials do not have multiplicative inverses
        using inverse = mpp::inverse<mpp::Poly<int>,mpp::op_mul>;
        auto has = inverse::has();
        EXPECT_FALSE(has);
    }
    {
        using absolute = mpp::absolute<mpp::Poly<int>,op_add>;
        auto has = absolute::has();
        EXPECT_TRUE(has);

        auto const poly = mpp::Poly<int>{1,2,3};

        auto poly1 = mpp::Poly<int>{-1,-2,-3};
        auto can = absolute::can(poly1);
        EXPECT_TRUE(can);

        auto poly2 = absolute::get(poly1);
        EXPECT_TRUE(poly2 == poly);

        absolute::make(poly1);
        EXPECT_TRUE(poly1 == poly);
    }
    {
        // Polynomials do not have multiplicative inverses
        using absolute = mpp::absolute<mpp::Poly<int>,mpp::op_mul>;
        auto has = absolute::has();
        EXPECT_FALSE(has);
    }
    {
        // TODO: test mathpp modulo of polynomials
    }
    {
        using division = mpp::division<mpp::Poly<int>,mpp::Poly<int>>;
        auto has = division::has();
        EXPECT_TRUE(has);

        auto const poly_a = mpp::Poly<int>{2,5,7,6};
        auto const poly_b = mpp::Poly<int>{1,2,3};
        auto can = division::can(poly_a,poly_b);
        EXPECT_TRUE(can);

        auto const poly_q = mpp::Poly<int>{1,2};
        auto const poly_r = mpp::Poly<int>{1,1};
        auto [q,r] = division::get(poly_a,poly_b);
        EXPECT_TRUE(q == poly_q);
        EXPECT_TRUE(r == poly_r);
    }
}

TEST(MPP_POLY, OPERATORS)
{
    {
        auto poly1 = mpp::Poly<int>{1,2,3};
        auto poly2 = mpp::Poly<int>{2,3};
        EXPECT_TRUE(poly1 != poly2);
        EXPECT_TRUE(poly1 >= poly2);
        EXPECT_TRUE(poly1 > poly2);
        EXPECT_FALSE(poly1 == poly2);
        EXPECT_FALSE(poly1 <= poly2);
        EXPECT_FALSE(poly1 < poly2);
    }
    {
        auto poly1 = mpp::Poly<int>{1,2,-3};
        auto poly2 = mpp::Poly<int>{2,3};
        EXPECT_TRUE(poly1 != poly2);
        EXPECT_TRUE(poly1 <= poly2);
        EXPECT_TRUE(poly1 < poly2);
        EXPECT_FALSE(poly1 == poly2);
        EXPECT_FALSE(poly1 >= poly2);
        EXPECT_FALSE(poly1 > poly2);
    }
    {
        auto poly1 = mpp::Poly<int>{1,2,3};
        auto poly2 = mpp::Poly<float>{1.0f,2.0f,3.0f};
        EXPECT_TRUE(poly1 == poly2);
        EXPECT_TRUE(poly1 >= poly2);
        EXPECT_TRUE(poly1 <= poly2);
        EXPECT_FALSE(poly1 != poly2);
        EXPECT_FALSE(poly1 < poly2);
        EXPECT_FALSE(poly1 > poly2);
    }
    {
        auto poly1 = mpp::Poly<int>{1,2,3};
        auto poly2 = mpp::Poly<float>{1.0f,1.0f,1.0f};
        EXPECT_TRUE(poly1 != poly2);
        EXPECT_TRUE(poly1 >= poly2);
        EXPECT_TRUE(poly1 > poly2);
        EXPECT_FALSE(poly1 == poly2);
        EXPECT_FALSE(poly1 <= poly2);
        EXPECT_FALSE(poly1 < poly2);
    }
    {
        auto poly1 = mpp::Poly<float>{1.0f,2.0f,3.5f};
        auto poly2 = mpp::Poly<float>{1.0f,2.5f,3.0f};
        EXPECT_TRUE(poly1 != poly2);
        EXPECT_TRUE(poly1 >= poly2);
        EXPECT_TRUE(poly1 > poly2);
        EXPECT_FALSE(poly1 == poly2);
        EXPECT_FALSE(poly1 <= poly2);
        EXPECT_FALSE(poly1 < poly2);
    }
    {
        auto vec1 = std::vector<int>{1,2,3}, vec2 = std::vector<int>{4,5,6};
        auto poly1 = mpp::Poly<int>{vec1}, poly2 = mpp::Poly<int>{vec2};

        std::swap(poly1,poly2);
        EXPECT_EQ(poly1.coeffs(), vec2);
        EXPECT_EQ(poly2.coeffs(), vec1);

        poly1.swap(poly2);
        EXPECT_EQ(poly1.coeffs(), vec1);
        EXPECT_EQ(poly2.coeffs(), vec2);
    }
}

TEST(MPP_POLY, MATH_SELF)
{
    {
        auto poly1 = mpp::Poly<int>{1,2,3,4,5};

        auto poly2 = mpp::Poly<int>{3,4,5};
        EXPECT_TRUE((poly1 << 2) == poly2);
        EXPECT_TRUE((poly1 <<= 2) == poly2);

        auto poly3 = mpp::Poly<int>{0,0,3,4,5};
        EXPECT_TRUE((poly1 >> 2) == poly3);
        EXPECT_TRUE((poly1 >>= 2) == poly3);
    }
    {
        auto poly1 = mpp::Poly<int>{5,1};           // 5+x
        poly1 += mpp::Poly<float>{0.0f,2.0f,1.0f};  // 2x+x^2
        auto poly2 = mpp::Poly<int>{5,3,1};         // 5+3x+x^2
        EXPECT_TRUE(poly1 == poly2);
    }
    {
        auto poly1 = mpp::Poly<int>{5,1};           // 5+x
        poly1 -= mpp::Poly<float>{0.0f,2.0f,1.0f};  // 2x+x^2
        auto poly2 = mpp::Poly<int>{5,-1,-1};       // 5-x-x^2
        EXPECT_TRUE(poly1 == poly2);
    }
    {
        auto poly1 = mpp::Poly<int>{5,1};           // 5+x
        poly1 *= mpp::Poly<float>{0.0f,2.0f,1.0f};  // 2x+x^2
        auto poly2 = mpp::Poly<int>{0,10,7,1};      // 10x+7x^2+x^3
        EXPECT_TRUE(poly1 == poly2);
    }
    {
        auto poly1 = mpp::Poly<int>{2,5,7,6};
        auto poly2 = mpp::Poly<int>{1,2,3};
        auto poly3 = mpp::Poly<int>{1,2};
        EXPECT_TRUE((poly1 /= poly2) == poly3);
    }
    {
        auto poly1 = mpp::Poly<int>{2,5,7,6};
        auto poly2 = mpp::Poly<int>{1,2,3};
        auto poly3 = mpp::Poly<int>{1,1};
        EXPECT_TRUE((poly1 %= poly2) == poly3);
    }
}

TEST(MPP_POLY, MATH_EXTRA)
{
    {
        auto poly1 = mpp::Poly<int>{1,2,3};

        auto poly2 = mpp::Poly<int>{-1,-2,-3};
        EXPECT_TRUE(-poly1 == poly2);

        auto poly3 = mpp::Poly<int>{+1,+2,+3};
        EXPECT_TRUE(+poly1 == poly3);
    }
    {
        auto poly1 = mpp::Poly<int>{5,1};                       // 5+x
        auto poly2 = mpp::Poly<float>{0.0f,2.0f,1.0f};          // 2x+x^2
        auto poly3 = mpp::Poly<float>{5.0f,3.0f,1.0f};          // 5+3x+x^2
        auto poly4 = poly1 + poly2;
        EXPECT_TRUE(poly3 == poly4);
    }
    {
        auto poly1 = mpp::Poly<int>{5,1};                       // 5+x
        auto poly2 = mpp::Poly<float>{0.0f,2.0f,1.0f};          // 2x+x^2
        auto poly3 = mpp::Poly<float>{5.0f,-1.0f,-1.0f};        // 5-x-x^2
        auto poly4 = poly1 - poly2;
        EXPECT_TRUE(poly3 == poly4);
    }
    {
        auto poly1 = mpp::Poly<int>{5,1};                       // 5+x
        auto poly2 = mpp::Poly<float>{0.0f,2.0f,1.0f};          // 2x+x^2
        auto poly3 = mpp::Poly<float>{0.0f,10.0f,7.0f,1.0f};    // 10x+7x^2+x^3
        auto poly4 = poly1 * poly2;
        EXPECT_TRUE(poly3 == poly4);
    }
    {
        auto poly1 = mpp::Poly<int>{2,5,7,6};
        auto poly2 = mpp::Poly<int>{1,2,3};
        auto poly3 = mpp::Poly<int>{1,2};
        EXPECT_TRUE((poly1 / poly2) == poly3);
    }
    {
        auto poly1 = mpp::Poly<int>{2,5,7,6};
        auto poly2 = mpp::Poly<int>{1,2,3};
        auto poly3 = mpp::Poly<int>{1,1};
        EXPECT_TRUE((poly1 % poly2) == poly3);
    }
}
