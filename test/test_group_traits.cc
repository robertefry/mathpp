
#include "gtest/gtest.h"
#include <mathpp/group_traits.hh>

struct TestType
{};

template <>
struct mpp::is_abelian_group<TestType,mpp::op_add>
    : public std::true_type
{};

TEST(GROUP_TRAITS, GROUP_TERMINATION)
{
    auto is_group = mpp::is_group<TestType,mpp::op_add>::value;
    EXPECT_TRUE(is_group);
    auto has_closure = mpp::has_closure<TestType,mpp::op_add>::value;
    EXPECT_TRUE(has_closure);
    auto has_identity = mpp::has_identity<TestType,mpp::op_add>::value;
    EXPECT_TRUE(has_identity);
    auto has_invertibility = mpp::has_invertibility<TestType,mpp::op_add>::value;
    EXPECT_TRUE(has_invertibility);
    auto has_associativity = mpp::has_associativity<TestType,mpp::op_add>::value;
    EXPECT_TRUE(has_associativity);
    auto has_commutativity = mpp::has_commutativity<TestType,mpp::op_add>::value;
    EXPECT_TRUE(has_commutativity);
}

template <>
struct mpp::is_abelian_group<TestType,mpp::op_mul>
    : public std::true_type
{};

template <>
struct mpp::has_distributivity<TestType,mpp::op_add,mpp::op_mul>
    : public std::true_type
{};

TEST(GROUP_TRAITS, FIELD_TERMINATION)
{
    auto is_field = mpp::is_field<TestType,mpp::op_add,mpp::op_mul>::value;
    EXPECT_TRUE(is_field);
}
