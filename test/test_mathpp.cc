
#include "gtest/gtest.h"
#include <mathpp/mathpp.hh>

using mpp::op_add;  using mpp::op_mul;

TEST(MATHPP, PRIMATIVES)
{
    {
        auto int_add_identity = mpp::identity<int,op_add>::get();
        EXPECT_EQ(int_add_identity,0);

        auto int_mul_identity = mpp::identity<int,op_mul>::get();
        EXPECT_EQ(int_mul_identity,1);

        auto float_add_identity = mpp::identity<float,op_add>::get();
        EXPECT_EQ(float_add_identity,0);

        auto float_mul_identity = mpp::identity<float,op_mul>::get();
        EXPECT_EQ(float_mul_identity,1);
    }
    {
        auto int_add_inverse_10 = mpp::inverse<int,op_add>::get(10);
        EXPECT_EQ(int_add_inverse_10,-10);

        // auto int_mul_inverse_10 = mpp::inverse<int,op_mul>::get(10);
        // EXPECT_EQ(int_mul_inverse_10,0.1f);

        auto float_add_inverse_10 = mpp::inverse<float,op_add>::get(10);
        EXPECT_EQ(float_add_inverse_10,-10);

        auto float_mul_inverse_10 = mpp::inverse<float,op_mul>::get(10);
        EXPECT_EQ(float_mul_inverse_10,0.1f);
    }
}
