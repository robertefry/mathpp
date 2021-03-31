
#include "gtest/gtest.h"

#include <mathpp/logic.hh>
using namespace mpp;

TEST(MPP_LOGIC, TRISTATE)
{
    {
        auto value = logic::all;
        EXPECT_TRUE(value == logic::all);
        EXPECT_FALSE(value == logic::some);
        EXPECT_FALSE(value == logic::none);
    }
    {
        auto value = logic::some;
        EXPECT_FALSE(value == logic::all);
        EXPECT_TRUE(value == logic::some);
        EXPECT_FALSE(value == logic::none);
    }
    {
        auto value = logic::none;
        EXPECT_FALSE(value == logic::all);
        EXPECT_FALSE(value == logic::some);
        EXPECT_TRUE(value == logic::none);
    }
    {
        auto a = logic::all;
        auto b = logic::some;
        auto c = logic::none;

        EXPECT_TRUE(a == a);
        EXPECT_TRUE(b == b);
        EXPECT_TRUE(c == c);
        EXPECT_TRUE(a != b);
        EXPECT_TRUE(a != c);
        EXPECT_TRUE(b != c);

        EXPECT_TRUE(a > b);
        EXPECT_TRUE(a > c);
        EXPECT_TRUE(b > c);
        EXPECT_FALSE(a < b);
        EXPECT_FALSE(a < c);
        EXPECT_FALSE(b < c);

        EXPECT_TRUE(a >= b);
        EXPECT_TRUE(a >= c);
        EXPECT_TRUE(b >= c);
        EXPECT_FALSE(a <= b);
        EXPECT_FALSE(a <= c);
        EXPECT_FALSE(b <= c);
    }
    {
        EXPECT_TRUE(~logic::all  == logic::none);
        EXPECT_TRUE(~logic::some == logic::some);
        EXPECT_TRUE(~logic::none == logic::all );

        EXPECT_TRUE((logic::all  & logic::all ) == logic::all );
        EXPECT_TRUE((logic::all  & logic::some) == logic::some);
        EXPECT_TRUE((logic::all  & logic::none) == logic::none);
        EXPECT_TRUE((logic::some & logic::all ) == logic::some);
        EXPECT_TRUE((logic::some & logic::some) == logic::some);
        EXPECT_TRUE((logic::some & logic::none) == logic::none);
        EXPECT_TRUE((logic::none & logic::all ) == logic::none);
        EXPECT_TRUE((logic::none & logic::some) == logic::none);
        EXPECT_TRUE((logic::none & logic::none) == logic::none);

        EXPECT_TRUE((logic::all  | logic::all ) == logic::all );
        EXPECT_TRUE((logic::all  | logic::some) == logic::all );
        EXPECT_TRUE((logic::all  | logic::none) == logic::all );
        EXPECT_TRUE((logic::some | logic::all ) == logic::all );
        EXPECT_TRUE((logic::some | logic::some) == logic::some);
        EXPECT_TRUE((logic::some | logic::none) == logic::some);
        EXPECT_TRUE((logic::none | logic::all ) == logic::all );
        EXPECT_TRUE((logic::none | logic::some) == logic::some);
        EXPECT_TRUE((logic::none | logic::none) == logic::none);

        EXPECT_TRUE((logic::all  ^ logic::all ) == logic::none);
        EXPECT_TRUE((logic::all  ^ logic::some) == logic::some);
        EXPECT_TRUE((logic::all  ^ logic::none) == logic::all );
        EXPECT_TRUE((logic::some ^ logic::all ) == logic::some);
        EXPECT_TRUE((logic::some ^ logic::some) == logic::some);
        EXPECT_TRUE((logic::some ^ logic::none) == logic::some);
        EXPECT_TRUE((logic::none ^ logic::all ) == logic::all );
        EXPECT_TRUE((logic::none ^ logic::some) == logic::some);
        EXPECT_TRUE((logic::none ^ logic::none) == logic::none);

        EXPECT_TRUE((logic::all  << 0) == logic::all );
        EXPECT_TRUE((logic::all  << 1) == logic::all );
        EXPECT_TRUE((logic::all  << 2) == logic::all );
        EXPECT_TRUE((logic::some << 0) == logic::some);
        EXPECT_TRUE((logic::some << 1) == logic::all );
        EXPECT_TRUE((logic::some << 2) == logic::all );
        EXPECT_TRUE((logic::none << 0) == logic::none);
        EXPECT_TRUE((logic::none << 1) == logic::some);
        EXPECT_TRUE((logic::none << 2) == logic::all );

        EXPECT_TRUE((logic::all  >> 0) == logic::all );
        EXPECT_TRUE((logic::all  >> 1) == logic::some);
        EXPECT_TRUE((logic::all  >> 2) == logic::none);
        EXPECT_TRUE((logic::some >> 0) == logic::some);
        EXPECT_TRUE((logic::some >> 1) == logic::none);
        EXPECT_TRUE((logic::some >> 2) == logic::none);
        EXPECT_TRUE((logic::none >> 0) == logic::none);
        EXPECT_TRUE((logic::none >> 1) == logic::none);
        EXPECT_TRUE((logic::none >> 2) == logic::none);
    }
}
