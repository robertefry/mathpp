
#include "gtest/gtest.h"
#include <mathpp/mathpp.hh>

using mpp::op_add;  using mpp::op_mul;

TEST(MATHPP, PRIMITIVE_IDENTITY)
{
    {
        using Tp = unsigned;
        using identity = mpp::identity<Tp,op_add>;

        auto has = identity::has();
        EXPECT_TRUE(has);

        auto ident = identity::get();
        EXPECT_EQ(ident,0);
    }
    {
        using Tp = signed;
        using identity = mpp::identity<Tp,op_add>;

        auto has = identity::has();
        EXPECT_TRUE(has);

        auto ident = identity::get();
        EXPECT_EQ(ident,0);
    }
    {
        using Tp = float;
        using identity = mpp::identity<Tp,op_add>;

        auto has = identity::has();
        EXPECT_TRUE(has);

        auto ident = identity::get();
        EXPECT_EQ(ident,0);
    }
    {
        using Tp = unsigned;
        using identity = mpp::identity<Tp,op_mul>;

        auto has = identity::has();
        EXPECT_TRUE(has);

        auto ident = identity::get();
        EXPECT_EQ(ident,1);
    }
    {
        using Tp = signed;
        using identity = mpp::identity<Tp,op_mul>;

        auto has = identity::has();
        EXPECT_TRUE(has);

        auto ident = identity::get();
        EXPECT_EQ(ident,1);
    }
    {
        using Tp = float;
        using identity = mpp::identity<Tp,op_mul>;

        auto has = identity::has();
        EXPECT_TRUE(has);

        auto ident = identity::get();
        EXPECT_EQ(ident,1);
    }
}

TEST(MATHPP, PRIMITIVE_INVERSE)
{
    {
        // The unsigned integers do not have additive inverses
        using Tp = unsigned;
        using inverse = mpp::inverse<Tp,op_add>;

        auto has = inverse::has();
        EXPECT_FALSE(has);
    }
    {
        using Tp = signed;
        using inverse = mpp::inverse<Tp,op_add>;
        Tp value = 10;

        auto has = inverse::has();
        EXPECT_TRUE(has);

        inverse::make(value);
        EXPECT_EQ(value,-10);

        inverse::make(value);
        EXPECT_EQ(value,10);
    }
    {
        using Tp = float;
        using inverse = mpp::inverse<Tp,op_add>;
        Tp value = 10;

        auto has = inverse::has();
        EXPECT_TRUE(has);

        inverse::make(value);
        EXPECT_EQ(value,-10);

        inverse::make(value);
        EXPECT_EQ(value,10);
    }
    {
        // The unsigned integers do not have multiplicative inverses
        using Tp = unsigned;
        using inverse = mpp::inverse<Tp,op_mul>;

        auto has = inverse::has();
        EXPECT_FALSE(has);
    }
    {
        // The signed integers do not have multiplicative inverses
        using Tp = signed;
        using inverse = mpp::inverse<Tp,op_mul>;

        auto has = inverse::has();
        EXPECT_FALSE(has);
    }
    {
        using Tp = float;
        using inverse = mpp::inverse<Tp,op_mul>;
        Tp value = 10;

        auto has = inverse::has();
        EXPECT_TRUE(has);

        inverse::make(value);
        EXPECT_EQ(value,0.1f);

        inverse::make(value);
        EXPECT_EQ(value,10);
    }
}

TEST(MATHPP, PRIMITIVE_ABSOLUTE)
{
    {
        // The unsigned integers do not have additive inverses
        using Tp = unsigned;
        using absolute = mpp::absolute<Tp,op_add>;

        auto has = absolute::has();
        EXPECT_FALSE(has);
    }
    {
        using Tp = signed;
        using absolute = mpp::absolute<Tp,op_add>;
        Tp value = -10;

        auto has = absolute::has();
        EXPECT_TRUE(has);

        absolute::make(value);
        EXPECT_EQ(value,10);

        absolute::make(value);
        EXPECT_EQ(value,10);
    }
    {
        using Tp = float;
        using absolute = mpp::absolute<Tp,op_add>;
        Tp value = -10;

        auto has = absolute::has();
        EXPECT_TRUE(has);

        absolute::make(value);
        EXPECT_EQ(value,10);

        absolute::make(value);
        EXPECT_EQ(value,10);
    }
    {
        /* The unsigned integers do not have multiplicative inverses */
        using Tp = unsigned;
        using absolute = mpp::absolute<Tp,op_mul>;

        auto has = absolute::has();
        EXPECT_FALSE(has);
    }
    {
        /* The signed integers do not have multiplicative inverses */
        using Tp = signed;
        using absolute = mpp::absolute<Tp,op_mul>;

        auto has = absolute::has();
        EXPECT_FALSE(has);
    }
    {
        using Tp = float;
        using absolute = mpp::absolute<Tp,op_mul>;
        Tp value = 0.1;

        auto has = absolute::has();
        EXPECT_TRUE(has);

        absolute::make(value);
        EXPECT_EQ(value,10);

        absolute::make(value);
        EXPECT_EQ(value,10);
    }
}

TEST(MATHPP, PRIMITIVE_MODULO)
{
    {
        using Tp = unsigned;
        using modulo = mpp::modulo<Tp>;
        Tp value = 10, mod = 7;

        auto has = modulo::has();
        EXPECT_TRUE(has);

        modulo::make(value,mod);
        EXPECT_EQ(value,3);
    }
    {
        using Tp = signed;
        using modulo = mpp::modulo<Tp>;
        Tp value = 10, mod = 7;

        auto has = modulo::has();
        EXPECT_TRUE(has);

        modulo::make(value,mod);
        EXPECT_EQ(value,3);
    }
    {
        using Tp = signed;
        using modulo = mpp::modulo<Tp>;
        Tp value = -10, mod = 7;

        auto has = modulo::has();
        EXPECT_TRUE(has);

        modulo::make(value,mod);
        EXPECT_EQ(value,4);
    }
    {
        using Tp = float;
        using modulo = mpp::modulo<Tp>;
        Tp value = 10, mod = 7;

        auto has = modulo::has();
        EXPECT_TRUE(has);

        modulo::make(value,mod);
        EXPECT_EQ(value,3);
    }
    {
        using Tp = float;
        using modulo = mpp::modulo<Tp>;
        Tp value = -10, mod = 7;

        auto has = modulo::has();
        EXPECT_TRUE(has);

        modulo::make(value,mod);
        EXPECT_EQ(value,4);
    }
}

TEST(MATHPP, PRIMITIVE_DIVISION)
{
    {
        auto [q,r] = mpp::division<int,int>::get(13,5);
        EXPECT_EQ(q,2);
        EXPECT_EQ(r,3);
    }
    {
        auto [q,r] = mpp::division<int,float>::get(13,5);
        EXPECT_EQ(q,2.6f);
        EXPECT_EQ(r,0.0f);
    }
    {
        auto [q,r] = mpp::division<float,int>::get(13,5);
        EXPECT_EQ(q,2.6f);
        EXPECT_EQ(r,0.0f);
    }
    {
        auto [q,r] = mpp::division<float,float>::get(13,5);
        EXPECT_EQ(q,2.6f);
        EXPECT_EQ(r,0.0f);
    }
}
