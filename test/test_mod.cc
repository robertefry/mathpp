
#include "gtest/gtest.h"
#include <mathpp/mod.hh>

TEST(MPP_MOD, LIFETIME)
{
    {
        auto mod = mpp::Mod<int>{7};                        // default
        EXPECT_EQ(mod.value(), 0);
    }
    {
        auto mod = mpp::Mod<int>{7,4};                      // construct smaller
        EXPECT_EQ(mod.value(), 4);
    }
    {
        auto mod = mpp::Mod<int>{7,11};                     // construct bigger
        EXPECT_EQ(mod.value(), 4);
    }
    {
        auto mod = mpp::Mod<int>{7};
        mod = 4;                                            // assign smaller
        EXPECT_EQ(mod.value(), 4);
    }
    {
        auto mod = mpp::Mod<int>{7};
        mod = 11;                                           // assign bigger
        EXPECT_EQ(mod.value(), 4);
    }
    {
        auto mod1 = mpp::Mod<int>{7,6};
        auto mod2 = mpp::Mod<int>{5,mod1};                  // copy construct
        EXPECT_EQ(mod2.value(), 1);
    }
    {
        auto mod1 = mpp::Mod<int>{7,6};
        auto mod2 = mpp::Mod<int>{5,std::move(mod1)};       // move construct
        EXPECT_EQ(mod2.value(), 1);
    }
    {
        auto mod1 = mpp::Mod<int>{7,6};
        auto mod2 = mpp::Mod<int>{5};
        mod2 = mod1;                                        // copy assign
        EXPECT_EQ(mod2.value(), 1);
    }
    {
        auto mod1 = mpp::Mod<int>{7,6};
        auto mod2 = mpp::Mod<int>{5};
        mod2 = std::move(mod1);                             // move assign
        EXPECT_EQ(mod2.value(), 1);
    }
    {
        auto mod1 = mpp::Mod<int>{7,6};
        auto mod2 = mpp::Mod<float>{5,mod1};                // copy cast construct
        EXPECT_EQ(mod2.value(), 1.0f);
    }
    {
        auto mod1 = mpp::Mod<int>{7,6};
        auto mod2 = mpp::Mod<float>{5,std::move(mod1)};     // move cast construct
        EXPECT_EQ(mod2.value(), 1.0f);
    }
    {
        auto mod1 = mpp::Mod<int>{7,6};
        auto mod2 = mpp::Mod<float>{5};
        mod2 = mod1;                                        // copy cast assign
        EXPECT_EQ(mod2.value(), 1.0f);
    }
    {
        auto mod1 = mpp::Mod<int>{7,6};
        auto mod2 = mpp::Mod<float>{5};
        mod2 = std::move(mod1);                             // move cast assign
        EXPECT_EQ(mod2.value(), 1.0f);
    }
}

TEST(MPP_MOD, MATHPP)
{
    using mpp::op_add;  using mpp::op_mul;
    {
        using identity = mpp::identity<mpp::Mod<int>,op_add>;
        auto has = identity::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto mod1 = identity::get(7); // 0  (mod 7)
        EXPECT_EQ(mod1.value(), 0);

        auto mod2 = mpp::Mod<int>{7,3};
        identity::make(mod2);
        EXPECT_EQ(mod2.value(), 0);
    }
    {
        using identity = mpp::identity<mpp::Mod<int>,op_mul>;
        auto has = identity::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto mod1 = identity::get(7); // 1  (mod 7)
        EXPECT_EQ(mod1.value(), 1);

        auto mod2 = mpp::Mod<int>{7,3};
        identity::make(mod2);
        EXPECT_EQ(mod2.value(), 1);
    }
    {
        using inverse = mpp::inverse<mpp::Mod<int>,op_add>;
        auto has = inverse::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto mod1 = mpp::Mod<int>{7,5};
        auto can = inverse::can(mod1);
        EXPECT_TRUE(can);

        auto mod2 = inverse::get(mod1);
        EXPECT_EQ(mod2.value(), 2);

        inverse::make(mod1);
        EXPECT_EQ(mod1.value(), 2);
    }
    {
        using inverse = mpp::inverse<mpp::Mod<int>,op_mul>;
        auto has = inverse::has();
        EXPECT_TRUE(has == mpp::logic::some);

        // 4 (mod 8) doesn't have a multiplicative inverse because `gcd(4,8)!=1`
        auto mod1 = mpp::Mod<int>{8,4};
        auto can1 = inverse::can(mod1);
        EXPECT_FALSE(can1);

        // 4 (mod 7) does have a multiplicative inverse because `gcd(4,7)==1`
        auto mod2 = mpp::Mod<int>{7,4};
        auto can2 = inverse::can(mod2);
        EXPECT_TRUE(can2);

        auto mod3 = inverse::get(mod2);
        EXPECT_EQ(mod3.value(), 2);

        inverse::make(mod2);
        EXPECT_EQ(mod2.value(), 2);
    }
    {
        // The modular numbers are their own additive absolutes
        using absolute = mpp::absolute<mpp::Mod<int>,op_add>;
        auto has = absolute::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto mod1 = mpp::Mod<int>{7,4};
        auto can = absolute::can(mod1);
        EXPECT_TRUE(can);

        auto mod2 = absolute::get(mod1);
        EXPECT_EQ(mod2.value(), 4);

        absolute::make(mod1);
        EXPECT_EQ(mod1.value(), 4);
    }
    {
        using absolute = mpp::absolute<mpp::Mod<float>,op_mul>;
        auto has = absolute::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto mod1 = mpp::Mod<float>{7,0.25f};
        auto can = absolute::can(mod1);
        EXPECT_TRUE(can);

        auto mod2 = absolute::get(mod1);
        EXPECT_EQ(mod2.value(), 4.0f);

        absolute::make(mod1);
        EXPECT_EQ(mod1.value(), 4.0f);
    }
    {
        // TODO: test mathpp modulo of modulo
    }
    {
        using division = mpp::division<mpp::Mod<int>,mpp::Mod<int>>;
        auto has = division::has();
        EXPECT_TRUE(has == mpp::logic::all);

        auto const mod_a = mpp::Mod<int>{12,11};
        auto const mod_b = mpp::Mod<int>{8,2};
        auto can = division::can(mod_a,mod_b);
        EXPECT_TRUE(can);

        // `gcd(12,8) == 4` and `11 == 1 + 5 * 2 == 1 + 1 * 2  (mod 4)`
        auto const mod_r = mpp::Mod<int>{4,1};
        auto const mod_s = mpp::Mod<int>{4,5};
        auto [r,s] = division::get(mod_a,mod_b);
        EXPECT_TRUE(mod_r.modulus() == r.modulus());
        EXPECT_TRUE(mod_s.modulus() == s.modulus());
        EXPECT_TRUE(mod_r.value() == r.value());
        EXPECT_TRUE(mod_s.value() == s.value());
    }
}

TEST(MPP_MOD, OPERATORS)
{
    {
        auto mod1 = mpp::Mod<int>{7,3};
        auto mod2 = mpp::Mod<int>{7,5};
        EXPECT_TRUE(mod1 != mod2);
        EXPECT_TRUE(mod1 <= mod2);
        EXPECT_TRUE(mod1 < mod2);
        EXPECT_FALSE(mod1 == mod2);
        EXPECT_FALSE(mod1 >= mod2);
        EXPECT_FALSE(mod1 > mod2);
    }
    {
        auto mod1 = mpp::Mod<int>{7,3};
        auto mod2 = mpp::Mod<float>{7,5};
        EXPECT_TRUE(mod1 != mod2);
        EXPECT_TRUE(mod1 <= mod2);
        EXPECT_TRUE(mod1 < mod2);
        EXPECT_FALSE(mod1 == mod2);
        EXPECT_FALSE(mod1 >= mod2);
        EXPECT_FALSE(mod1 > mod2);
    }
    {
        int val1 = 3, val2 = 5;
        auto mod1 = mpp::Mod<int>{7,val1}, mod2 = mpp::Mod<int>{7,val2};

        std::swap(mod1,mod2);
        EXPECT_EQ(mod1.value(), val2);
        EXPECT_EQ(mod2.value(), val1);

        std::swap(mod1,mod2);
        EXPECT_EQ(mod1.value(), val1);
        EXPECT_EQ(mod2.value(), val2);
    }
}

TEST(MPP_MOD, MATH_SELF)
{
    {
        auto mod1 = mpp::Mod<int>{7,5};

        auto mod2 = mpp::Mod<int>{7,3};
        EXPECT_TRUE((mod1 << 1) == mod2);
        EXPECT_TRUE((mod1 <<= 1) == mod2);

        auto mod3 = mpp::Mod<int>{7,1};
        EXPECT_TRUE((mod1 >> 1) == mod3);
        EXPECT_TRUE((mod1 >>= 1) == mod3);
    }
    {
        auto mod1 = mpp::Mod<int>{7,3};

        auto mod2 = ++mod1;
        EXPECT_EQ(mod2.value(), 4);
        EXPECT_EQ(mod1.value(), 4);

        auto mod3 = mod1++;
        EXPECT_EQ(mod3.value(), 4);
        EXPECT_EQ(mod1.value(), 5);

        auto mod4 = --mod1;
        EXPECT_EQ(mod4.value(), 4);
        EXPECT_EQ(mod1.value(), 4);

        auto mod5 = mod1--;
        EXPECT_EQ(mod5.value(), 4);
        EXPECT_EQ(mod1.value(), 3);
    }
    {
        auto mod1 = mpp::Mod<int>{7,5};
        mod1 += mpp::Mod<int>{7,5};
        EXPECT_EQ(mod1.value(), 3);

        auto mod2 = mpp::Mod<int>{7,3};
        mod2 += mpp::Mod<int>{7,3};
        EXPECT_EQ(mod2.value(), 6);
    }
    {
        auto mod1 = mpp::Mod<int>{7,5};
        mod1 += mpp::Mod<float>{7,5};
        EXPECT_EQ(mod1.value(), 3);

        auto mod2 = mpp::Mod<int>{7,3};
        mod2 += mpp::Mod<float>{7,3};
        EXPECT_EQ(mod2.value(), 6);
    }
    {
        auto mod1 = mpp::Mod<int>{7,3};
        mod1 -= mpp::Mod<int>{7,5};
        EXPECT_EQ(mod1.value(), 5);

        auto mod2 = mpp::Mod<int>{7,5};
        mod2 -= mpp::Mod<int>{7,3};
        EXPECT_EQ(mod2.value(), 2);
    }
    {
        auto mod1 = mpp::Mod<int>{7,3};
        mod1 -= mpp::Mod<float>{7,5};
        EXPECT_EQ(mod1.value(), 5);

        auto mod2 = mpp::Mod<int>{7,5};
        mod2 -= mpp::Mod<float>{7,3};
        EXPECT_EQ(mod2.value(), 2);
    }
    {
        auto mod1 = mpp::Mod<int>{7,5};
        mod1 *= mpp::Mod<int>{7,3};
        EXPECT_EQ(mod1.value(), 1);

        auto mod2 = mpp::Mod<int>{7,3};
        mod2 *= mpp::Mod<int>{7,2};
        EXPECT_EQ(mod2.value(), 6);
    }
    {
        auto mod1 = mpp::Mod<int>{7,5};
        mod1 *= mpp::Mod<float>{7,3};
        EXPECT_EQ(mod1.value(), 1);

        auto mod2 = mpp::Mod<int>{7,3};
        mod2 *= mpp::Mod<float>{7,2};
        EXPECT_EQ(mod2.value(), 6);
    }
    {
        auto mod1 = mpp::Mod<int>{7,6};
        mod1 /= mpp::Mod<int>{7,3};
        EXPECT_EQ(mod1.value(), 2);
    }
    {
        auto mod1 = mpp::Mod<int>{7,6};
        mod1 /= mpp::Mod<float>{7,3};
        EXPECT_EQ(mod1.value(), 2);
    }
    {
        auto mod1 = mpp::Mod<int>{7,5};
        mod1 %= mpp::Mod<int>{7,3};
        EXPECT_EQ(mod1.value(), 2);
    }
    {
        auto mod1 = mpp::Mod<int>{7,5};
        mod1 %= mpp::Mod<float>{7,3};
        EXPECT_EQ(mod1.value(), 2);
    }
}

TEST(MPP_MOD, MATH_EXTRA)
{
    {
        auto mod1 = mpp::Mod<int>{7,5};
        EXPECT_TRUE(+mod1 == mod1);

        auto mod2 = mpp::Mod<int>{7,2};
        EXPECT_TRUE(-mod1 == mod2);
    }
}
