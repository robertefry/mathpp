
#include "gtest/gtest.h"
#include <mathpp/mathpp.hh>

#include <cstdint>

using mpp::op_add;  using mpp::op_mul;

TEST(MATHPP, PRIMITIVE_IDENTITY)
{
    {
        using Tp = uint32_t;
        using identity = mpp::identity<Tp,op_add>;

        auto has = identity::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto ident = identity::get();
        EXPECT_EQ(ident,0);
    }
    {
        using Tp = int32_t;
        using identity = mpp::identity<Tp,op_add>;

        auto has = identity::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto ident = identity::get();
        EXPECT_EQ(ident,0);
    }
    {
        using Tp = float;
        using identity = mpp::identity<Tp,op_add>;

        auto has = identity::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto ident = identity::get();
        EXPECT_EQ(ident,0);
    }
    {
        using Tp = uint32_t;
        using identity = mpp::identity<Tp,op_mul>;

        auto has = identity::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto ident = identity::get();
        EXPECT_EQ(ident,1);
    }
    {
        using Tp = int32_t;
        using identity = mpp::identity<Tp,op_mul>;

        auto has = identity::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto ident = identity::get();
        EXPECT_EQ(ident,1);
    }
    {
        using Tp = float;
        using identity = mpp::identity<Tp,op_mul>;

        auto has = identity::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto ident = identity::get();
        EXPECT_EQ(ident,1);
    }
}

TEST(MATHPP, PRIMITIVE_INVERSE)
{
    {
        // The unsigned integers do not have additive inverses
        using Tp = uint32_t;
        using inverse = mpp::inverse<Tp,op_add>;

        auto has = inverse::has();
        EXPECT_TRUE(has == mpp::logic::none);
    }
    {
        using Tp = int32_t;
        using inverse = mpp::inverse<Tp,op_add>;
        Tp value = -10;

        auto has = inverse::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = inverse::can(Tp{});
        EXPECT_TRUE(can);

        auto value1 = inverse::get(value);
        EXPECT_EQ(value1,10);

        inverse::make(value);
        EXPECT_EQ(value,10);
    }
    {
        using Tp = float;
        using inverse = mpp::inverse<Tp,op_add>;
        Tp value = -10;

        auto has = inverse::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = inverse::can(Tp{});
        EXPECT_TRUE(can);

        auto value1 = inverse::get(value);
        EXPECT_EQ(value1,10);

        inverse::make(value);
        EXPECT_EQ(value,10);
    }
    {
        // The unsigned integers do not have multiplicative inverses
        using Tp = uint32_t;
        using inverse = mpp::inverse<Tp,op_mul>;

        auto has = inverse::has();
        EXPECT_TRUE(has == mpp::logic::none);
    }
    {
        // The signed integers do not have multiplicative inverses
        using Tp = int32_t;
        using inverse = mpp::inverse<Tp,op_mul>;

        auto has = inverse::has();
        EXPECT_TRUE(has == mpp::logic::none);
    }
    {
        using Tp = float;
        using inverse = mpp::inverse<Tp,op_mul>;
        Tp value = -10;

        auto has = inverse::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = inverse::can(Tp{});
        EXPECT_TRUE(can);

        auto value1 = inverse::get(value);
        EXPECT_EQ(value1,-0.1f);

        inverse::make(value);
        EXPECT_EQ(value,-0.1f);
    }
}

TEST(MATHPP, PRIMITIVE_ABSOLUTE)
{
    {
        // The unsigned integers are their own additive absolutes
        using Tp = uint32_t;
        using absolute = mpp::absolute<Tp,op_add>;
        Tp value = 0xFFFFFFF5; // -10

        auto has = absolute::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = absolute::can(Tp{});
        EXPECT_TRUE(can);

        auto value1 = absolute::get(value);
        EXPECT_EQ(value1,0xFFFFFFF5);

        absolute::make(value);
        EXPECT_EQ(value,0xFFFFFFF5);
    }
    {
        using Tp = int32_t;
        using absolute = mpp::absolute<Tp,op_add>;
        Tp value = -10;

        auto has = absolute::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = absolute::can(Tp{});
        EXPECT_TRUE(can);

        auto value1 = absolute::get(value);
        EXPECT_EQ(value1,10);

        absolute::make(value);
        EXPECT_EQ(value,10);
    }
    {
        using Tp = float;
        using absolute = mpp::absolute<Tp,op_add>;
        Tp value = -10;

        auto has = absolute::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = absolute::can(Tp{});
        EXPECT_TRUE(can);

        auto value1 = absolute::get(value);
        EXPECT_EQ(value1,10);

        absolute::make(value);
        EXPECT_EQ(value,10);
    }
    {
        // The unsigned integers are their own multiplicative absolutes
        using Tp = uint32_t;
        using absolute = mpp::absolute<Tp,op_mul>;
        Tp value = 0xFFFFFFF5; // -10

        auto has = absolute::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = absolute::can(Tp{});
        EXPECT_TRUE(can);

        auto value1 = absolute::get(value);
        EXPECT_EQ(value1,0xFFFFFFF5);

        absolute::make(value);
        EXPECT_EQ(value,0xFFFFFFF5);
    }
    {
        // The signed integers are their own multiplicative absolutes
        using Tp = uint32_t;
        using absolute = mpp::absolute<Tp,op_mul>;
        Tp value = -10;

        auto has = absolute::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = absolute::can(Tp{});
        EXPECT_TRUE(can);

        auto value1 = absolute::get(value);
        EXPECT_EQ(value1,-10);

        absolute::make(value);
        EXPECT_EQ(value,-10);
    }
    {
        using Tp = float;
        using absolute = mpp::absolute<Tp,op_mul>;
        Tp value = -0.1f;

        auto has = absolute::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = absolute::can(Tp{});
        EXPECT_TRUE(can);

        auto value1 = absolute::get(value);
        EXPECT_EQ(value1,-10);

        absolute::make(value);
        EXPECT_EQ(value,-10);
    }
}

TEST(MATHPP, PRIMITIVE_MODULO)
{
    {
        using Tp = uint32_t;
        using modulo = mpp::modulo<Tp,Tp>;
        Tp value = 10, mod = 7;

        auto has = modulo::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = modulo::can(Tp{},Tp{});
        EXPECT_TRUE(can);

        modulo::make(value,mod);
        EXPECT_EQ(value,3);
    }
    {
        using Tp = int32_t;
        using modulo = mpp::modulo<Tp,Tp>;
        Tp value = 10, mod = 7;

        auto has = modulo::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = modulo::can(Tp{},Tp{});
        EXPECT_TRUE(can);

        modulo::make(value,mod);
        EXPECT_EQ(value,3);
    }
    {
        using Tp = int32_t;
        using modulo = mpp::modulo<Tp,Tp>;
        Tp value = -10, mod = 7;

        auto has = modulo::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = modulo::can(Tp{},Tp{});
        EXPECT_TRUE(can);

        modulo::make(value,mod);
        EXPECT_EQ(value,4);
    }
    {
        using Tp = float;
        using modulo = mpp::modulo<Tp,Tp>;
        Tp value = 10, mod = 7;

        auto has = modulo::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = modulo::can(Tp{},Tp{});
        EXPECT_TRUE(can);

        modulo::make(value,mod);
        EXPECT_EQ(value,3);
    }
    {
        using Tp = float;
        using modulo = mpp::modulo<Tp,Tp>;
        Tp value = -10, mod = 7;

        auto has = modulo::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto can = modulo::can(Tp{},Tp{});
        EXPECT_TRUE(can);

        modulo::make(value,mod);
        EXPECT_EQ(value,4);
    }
}

TEST(MATHPP, PRIMITIVE_DIVISION)
{
    {
        auto [r,s] = mpp::division<int,int>::get(13,5);
        EXPECT_EQ(r,3);
        EXPECT_EQ(s,2);
    }
    {
        auto [r,s] = mpp::division<int,float>::get(13,5);
        EXPECT_EQ(r,0.0f);
        EXPECT_EQ(s,2.6f);
    }
    {
        auto [r,s] = mpp::division<float,int>::get(13,5);
        EXPECT_EQ(r,0.0f);
        EXPECT_EQ(s,2.6f);
    }
    {
        auto [r,s] = mpp::division<float,float>::get(13,5);
        EXPECT_EQ(r,0.0f);
        EXPECT_EQ(s,2.6f);
    }
}
