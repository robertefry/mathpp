
#include "gtest/gtest.h"
#include <mathpp/group_traits.hh>

TEST(GROUP_TRAITS, PRIMATIVES)
{
    using mpp::op_add;  using mpp::op_mul;
    {
        auto float_add_identity = mpp::identity<float,op_add>::get();
        EXPECT_EQ(float_add_identity,0);

        auto float_mul_identity = mpp::identity<float,op_mul>::get();
        EXPECT_EQ(float_mul_identity,1);

        auto float_add_inverse_10 = mpp::inverse<float,op_add>::get(10);
        EXPECT_EQ(float_add_inverse_10,-10);

        auto float_mul_inverse_10 = mpp::inverse<float,op_mul>::get(10);
        EXPECT_EQ(float_mul_inverse_10,0.1f);
    }
    {
        auto int_add_is_abelian_group = mpp::is_abelian_group<int,op_add>::value;
        EXPECT_TRUE(int_add_is_abelian_group);

        auto int_mul_is_commutative_monoid = mpp::is_commutative_monoid<int,op_mul>::value;
        EXPECT_TRUE(int_mul_is_commutative_monoid);

        auto float_add_is_abelian_group = mpp::is_abelian_group<float,op_add>::value;
        EXPECT_TRUE(float_add_is_abelian_group);

        auto float_mul_is_abelian_group = mpp::is_abelian_group<float,op_mul>::value;
        EXPECT_TRUE(float_mul_is_abelian_group);
    }
}
