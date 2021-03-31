
#include "gtest/gtest.h"

#include <mathpp/vector.hh>
using namespace mpp;

TEST(MPP_VECTOR, LIFETIME)
{
    {
        auto elems = std::vector<int>{0,0,0};
        auto vec = Vector<int,3>{};                         // default
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{1,1,1};
        auto vec = Vector<int,3>(1);                        // fill
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{1,2,3};
        auto vec = Vector<int,3>{1,2,3};                    // forward elements
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{1,2,3};
        auto vec = Vector<int,3>{};
        vec = {1,2,3};                                      // assign initializer_list
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto vec1 = Vector<int,3>{1,2,3};
        auto vec2 = Vector<int,3>{vec1};                    // copy construct
        EXPECT_EQ(vec1.elements(), vec2.elements());
    }
    {
        auto vec = Vector<int,3>{1,2,3};
        auto vec1 = Vector<int,3>{vec};
        auto vec2 = Vector<int,3>{std::move(vec)};          // move construct
        EXPECT_EQ(vec1.elements(), vec2.elements());
    }
    {
        auto vec1 = Vector<int,3>{1,2,3};
        auto vec2 = Vector<int,3>{};
        vec2 = vec1;                                        // copy assign
        EXPECT_EQ(vec1.elements(), vec2.elements());
    }
    {
        auto vec = Vector<int,3>{1,2,3};
        auto vec1 = Vector<int,3>{vec};
        auto vec2 = Vector<int,3>{};
        vec2 = std::move(vec);                              // move assign
        EXPECT_EQ(vec1.elements(), vec2.elements());
    }
    {
        auto vec1 = Vector<int,3>{1,2,3};
        auto vec2 = Vector<float,3>{vec1};                  // copy cast construct
        EXPECT_TRUE(vec1 == vec2);
    }
    {
        auto vec = Vector<int,3>{1,2,3};
        auto vec1 = Vector<int,3>{vec};
        auto vec2 = Vector<float,3>{std::move(vec)};        // move cast construct
        EXPECT_TRUE(vec1 == vec2);
    }
    {
        auto vec1 = Vector<int,3>{1,2,3};
        auto vec2 = Vector<float,3>{};
        vec2 = vec1;                                        // copy cast assign
        EXPECT_TRUE(vec1 == vec2);
    }
    {
        auto vec = Vector<int,3>{1,2,3};
        auto vec1 = Vector<int,3>{vec};
        auto vec2 = Vector<float,3>{};
        vec2 = std::move(vec);                              // move cast assign
        EXPECT_TRUE(vec1 == vec2);
    }
}

TEST(MPP_VECTOR, MATHPP)
{
    {
        using identity = identity<Vector<int,3>,op_add>;
        auto has = identity::has();
        EXPECT_TRUE(has == logic::all);

        auto const vec = Vector<int,3>{0,0,0};

        auto vec1 = identity::get();
        EXPECT_TRUE(vec1 == vec);

        auto vec2 = Vector<int,3>{1,2,3};
        identity::make(vec2);
        EXPECT_TRUE(vec2 == vec);
    }
    {
        // Vectors do not have multiplicative identities
        using identity = identity<Vector<int,3>,op_mul>;
        auto has = identity::has();
        EXPECT_TRUE(has == logic::none);
    }
    {
        using inverse = inverse<Vector<int,3>,op_add>;
        auto has = inverse::has();
        EXPECT_TRUE(has == logic::all);

        auto const vec = Vector<int,3>{-1,-2,-3};

        auto vec1 = Vector<int,3>{1,2,3};
        auto can = inverse::can(vec1);
        EXPECT_TRUE(can);

        auto vec2 = inverse::get(vec1);
        EXPECT_TRUE(vec2 == vec);

        inverse::make(vec1);
        EXPECT_TRUE(vec1 == vec);
    }
    {
        // Vectors do not have multiplicative inverses
        using inverse = inverse<Vector<int,3>,op_mul>;
        auto has = inverse::has();
        EXPECT_TRUE(has == logic::none);
    }
    {
        using absolute = absolute<Vector<int,3>,op_add>;
        auto has = absolute::has();
        EXPECT_TRUE(has == logic::none);
    }
    {
        // Vectors do not have multiplicative absolutes
        using absolute = absolute<Vector<int,3>,op_mul>;
        auto has = absolute::has();
        EXPECT_TRUE(has == logic::none);
    }
    {
        // Vectors cannot modulo
        using modulo = modulo<Vector<int,3>,Vector<int,3>>;
        auto has = modulo::has();
        EXPECT_TRUE(has == logic::none);
    }
    {
        // Vectors cannot divide
        using division = division<Vector<int,3>,Vector<int,3>>;
        auto has = division::has();
        EXPECT_TRUE(has == logic::none);
    }
}

TEST(MPP_VECTOR, OPERATORS)
{
    {
        auto vec1 = Vector<int,3>{1,2,3};
        auto vec2 = Vector<float,3>{1.0f,2.0f,3.0f};
        EXPECT_TRUE(vec1 == vec2);
        EXPECT_FALSE(vec1 != vec2);
    }
    {
        auto vec1 = Vector<int,3>{1,2,3};
        auto vec2 = Vector<float,3>{3.0f,2.0f,1.0f};
        EXPECT_FALSE(vec1 == vec2);
        EXPECT_TRUE(vec1 != vec2);
    }
    {
        auto elems1 = std::vector<int>{1,2,3}, elems2 = std::vector<int>{4,5,6};
        auto vec1 = Vector<int,3>{1,2,3}, vec2 = Vector<int,3>{4,5,6};

        std::swap(vec1,vec2);
        EXPECT_EQ(vec1.elements(), elems2);
        EXPECT_EQ(vec2.elements(), elems1);

        std::swap(vec1,vec2);
        EXPECT_EQ(vec1.elements(), elems1);
        EXPECT_EQ(vec2.elements(), elems2);
    }
}

TEST(MPP_VECTOR, MATH_SELF)
{
    auto const vec1 = Vector<int,3>{1,2,3};
    auto const vec2 = Vector<int,3>{2,4,6};
    auto const scalar = 2.0f;
    {
        auto elems = std::vector<int>{3,6,9};
        auto vec = Vector<int,3>{vec2};
        vec += vec1;
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{1,2,3};
        auto vec = Vector<int,3>{vec2};
        vec -= vec1;
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{2,4,6};
        auto vec = Vector<int,3>{vec1};
        vec *= scalar;
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{1,2,3};
        auto vec = Vector<int,3>{vec2};
        vec /= scalar;
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{1,0,1};
        auto vec = Vector<int,3>{vec1};
        vec %= scalar;
        EXPECT_EQ(vec.elements(), elems);
    }
}

TEST(MPP_VECTOR, MATH_EXTRA)
{
    auto const vec1 = Vector<int,3>{1,2,3};
    auto const vec2 = Vector<int,3>{2,4,6};
    auto const scalar = 2.0f;
    {
        auto elems = std::vector<int>{1,2,3};
        auto vec = +vec1;
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{-1,-2,-3};
        auto vec = -vec1;
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{3,6,9};
        auto vec = vec2 + vec1;
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{1,2,3};
        auto vec = vec2 - vec1;
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{2,4,6};
        auto vec = vec1 * scalar;
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{1,2,3};
        auto vec = vec2 / scalar;
        EXPECT_EQ(vec.elements(), elems);
    }
    {
        auto elems = std::vector<int>{1,0,1};
        auto vec = vec1 % scalar;
        EXPECT_EQ(vec.elements(), elems);
    }
}
