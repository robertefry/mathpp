
#include <type_traits>

/* ************************************************************************** */
// Definitions
/* ************************************************************************** */

namespace mpp
{

    /* Operation tags ******************************************************* */

    struct operation
    {};

    struct op_add
        : public operation
    {};

    struct op_mul
        : public operation
    {};

    /* Ordering tags ******************************************************** */

    // TODO Ordering tags

    /* Axiom tags *********************************************************** */

    template <typename Tp, typename Op>
    struct has_closure
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct has_identity
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct has_invertibility
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct has_associativity
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct has_commutativity
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct has_left_distributivity
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct has_right_distributivity
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct has_distributivity
        : public std::false_type
    {};

    /* Group-like structure tags ******************************************** */

    template <typename Tp, typename Op>
    struct is_semigroupoid
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct is_small_category
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct is_groupoid
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct is_magma
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct is_quasigroup
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct is_unary_magma
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct is_loop
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct is_semigroup
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct is_invertible_semigroup
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct is_monoid
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct is_commutative_monoid
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct is_group
        : public std::false_type
    {};

    template <typename Tp, typename Op>
    struct is_abelian_group
        : public std::false_type
    {};

    /* Ring-like structure tags ********************************************* */

    template <typename Tp, typename Op1, typename Op2>
    struct is_semiring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_left_near_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_right_near_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_near_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_pseudo_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_trivial_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_commutative_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_skew_field
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_field
        : public std::false_type
    {};

    /* Domain-like structure tags ******************************************* */

    template <typename Tp, typename Op1, typename Op2>
    struct is_integral_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_integrally_closed_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_gcd_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_unique_factorisation_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_principal_ideal_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_euclidean_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct is_algebraically_closed_domain
        : public std::false_type
    {};

} // namespace mpp

/* ************************************************************************** */
// Specialization
/* ************************************************************************** */

/* Axiom tags *************************************************************** */

template <typename Tp, typename Op>
    requires std::is_base_of<mpp::operation,Op>::value
        && std::is_arithmetic<Tp>::value
struct mpp::has_closure<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires std::is_base_of<mpp::operation,Op>::value
        && std::is_arithmetic<Tp>::value
struct mpp::has_identity<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires std::is_base_of<mpp::operation,Op>::value
        && std::is_arithmetic<Tp>::value && std::is_signed<Tp>::value
struct mpp::has_invertibility<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires std::is_base_of<mpp::operation,Op>::value
        && std::is_arithmetic<Tp>::value
struct mpp::has_associativity<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires std::is_base_of<mpp::operation,Op>::value
        && std::is_arithmetic<Tp>::value
struct mpp::has_commutativity<Tp,Op>
    : public std::true_type
{};

template <typename Tp>
    requires std::is_arithmetic<Tp>::value
struct mpp::has_left_distributivity<Tp,mpp::op_add,mpp::op_mul>
    : public std::true_type
{};

template <typename Tp>
    requires std::is_arithmetic<Tp>::value
struct mpp::has_right_distributivity<Tp,mpp::op_add,mpp::op_mul>
    : public std::true_type
{};

template <typename Tp>
    requires mpp::has_left_distributivity<Tp,mpp::op_add,mpp::op_mul>::value
        && mpp::has_right_distributivity<Tp,mpp::op_add,mpp::op_mul>::value
struct mpp::has_distributivity<Tp,mpp::op_add,mpp::op_mul>
    : public std::true_type
{};

/* Group-like structure tags ************************************************ */

// semigroup structure

template <typename Tp, typename Op>
    requires mpp::has_associativity<Tp,Op>::value
struct mpp::is_semigroupoid<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_semigroupoid<Tp,Op>::value
struct mpp::has_associativity<Tp,Op>
    : public std::true_type
{};

// small-category structure

template <typename Tp, typename Op>
    requires mpp::has_associativity<Tp,Op>::value
        && mpp::has_identity<Tp,Op>::value
struct mpp::is_small_category<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_small_category<Tp,Op>::value
struct mpp::has_associativity<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_small_category<Tp,Op>::value
struct mpp::has_identity<Tp,Op>
    : public std::true_type
{};

// groupid structure

template <typename Tp, typename Op>
    requires mpp::has_associativity<Tp,Op>::value
        && mpp::has_identity<Tp,Op>::value
        && mpp::has_invertibility<Tp,Op>::value
struct mpp::is_groupoid<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_groupoid<Tp,Op>::value
struct mpp::has_associativity<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_groupoid<Tp,Op>::value
struct mpp::has_invertibility<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_groupoid<Tp,Op>::value
struct mpp::has_identity<Tp,Op>
    : public std::true_type
{};

// magma structure

template <typename Tp, typename Op>
    requires mpp::has_closure<Tp,Op>::value
struct mpp::is_magma<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_magma<Tp,Op>::value
struct mpp::has_closure<Tp,Op>
    : public std::true_type
{};

// quasigroup structure

template <typename Tp, typename Op>
    requires mpp::has_closure<Tp,Op>::value
        && mpp::has_invertibility<Tp,Op>::value
struct mpp::is_quasigroup<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_quasigroup<Tp,Op>::value
struct mpp::has_closure<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_quasigroup<Tp,Op>::value
struct mpp::has_invertibility<Tp,Op>
    : public std::true_type
{};

// unary magma structure

template <typename Tp, typename Op>
    requires mpp::has_closure<Tp,Op>::value
        && mpp::has_identity<Tp,Op>::value
struct mpp::is_unary_magma<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_unary_magma<Tp,Op>::value
struct mpp::has_closure<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_unary_magma<Tp,Op>::value
struct mpp::has_identity<Tp,Op>
    : public std::true_type
{};

// loop structure

template <typename Tp, typename Op>
    requires mpp::has_closure<Tp,Op>::value
        && mpp::has_identity<Tp,Op>::value
        && mpp::has_invertibility<Tp,Op>::value
struct mpp::is_loop<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_loop<Tp,Op>::value
struct mpp::has_closure<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_loop<Tp,Op>::value
struct mpp::has_identity<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_loop<Tp,Op>::value
struct mpp::has_invertibility<Tp,Op>
    : public std::true_type
{};

// semigroup structure

template <typename Tp, typename Op>
    requires mpp::has_closure<Tp,Op>::value
        && mpp::has_associativity<Tp,Op>::value
struct mpp::is_semigroup<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_semigroup<Tp,Op>::value
struct mpp::has_closure<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_semigroup<Tp,Op>::value
struct mpp::has_associativity<Tp,Op>
    : public std::true_type
{};

// invertible semigroup structure

template <typename Tp, typename Op>
    requires mpp::has_closure<Tp,Op>::value
        && mpp::has_invertibility<Tp,Op>::value
        && mpp::has_associativity<Tp,Op>::value
struct mpp::is_invertible_semigroup<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_invertible_semigroup<Tp,Op>::value
struct mpp::has_closure<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_invertible_semigroup<Tp,Op>::value
struct mpp::has_invertibility<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_invertible_semigroup<Tp,Op>::value
struct mpp::has_associativity<Tp,Op>
    : public std::true_type
{};

// monoid structure

template <typename Tp, typename Op>
    requires mpp::has_closure<Tp,Op>::value
        && mpp::has_identity<Tp,Op>::value
        && mpp::has_associativity<Tp,Op>::value
struct mpp::is_monoid<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_monoid<Tp,Op>::value
struct mpp::has_closure<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_monoid<Tp,Op>::value
struct mpp::has_identity<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_monoid<Tp,Op>::value
struct mpp::has_associativity<Tp,Op>
    : public std::true_type
{};

// commutative monoid structure

template <typename Tp, typename Op>
    requires mpp::has_closure<Tp,Op>::value
        && mpp::has_identity<Tp,Op>::value
        && mpp::has_associativity<Tp,Op>::value
        && mpp::has_commutativity<Tp,Op>::value
struct mpp::is_commutative_monoid<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_commutative_monoid<Tp,Op>::value
struct mpp::has_closure<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_commutative_monoid<Tp,Op>::value
struct mpp::has_identity<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_commutative_monoid<Tp,Op>::value
struct mpp::has_associativity<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_commutative_monoid<Tp,Op>::value
struct mpp::has_commutativity<Tp,Op>
    : public std::true_type
{};

// group structure

template <typename Tp, typename Op>
    requires mpp::has_closure<Tp,Op>::value
        && mpp::has_identity<Tp,Op>::value
        && mpp::has_invertibility<Tp,Op>::value
        && mpp::has_associativity<Tp,Op>::value
struct mpp::is_group<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_group<Tp,Op>::value
struct mpp::has_closure<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_group<Tp,Op>::value
struct mpp::has_identity<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_group<Tp,Op>::value
struct mpp::has_invertibility<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_group<Tp,Op>::value
struct mpp::has_associativity<Tp,Op>
    : public std::true_type
{};

// abelian group structure

template <typename Tp, typename Op>
    requires mpp::has_closure<Tp,Op>::value
        && mpp::has_identity<Tp,Op>::value
        && mpp::has_invertibility<Tp,Op>::value
        && mpp::has_associativity<Tp,Op>::value
        && mpp::has_commutativity<Tp,Op>::value
struct mpp::is_abelian_group<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_abelian_group<Tp,Op>::value
struct mpp::has_closure<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_abelian_group<Tp,Op>::value
struct mpp::has_identity<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_abelian_group<Tp,Op>::value
struct mpp::has_invertibility<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_abelian_group<Tp,Op>::value
struct mpp::has_associativity<Tp,Op>
    : public std::true_type
{};

template <typename Tp, typename Op>
    requires mpp::is_abelian_group<Tp,Op>::value
struct mpp::has_commutativity<Tp,Op>
    : public std::true_type
{};

/* Ring-like structure tags ************************************************* */

// semiring structure

template <typename Tp, typename Op1, typename Op2>
    requires mpp::has_distributivity<Tp,Op1,Op2>::value
        && mpp::is_commutative_monoid<Tp,Op1>::value
        && mpp::is_monoid<Tp,Op2>::value
struct mpp::is_semiring<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_semiring<Tp,Op1,Op2>::value
struct mpp::has_distributivity<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_semiring<Tp,Op1,Op2>::value
struct mpp::is_commutative_monoid<Tp,Op1>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_semiring<Tp,Op1,Op2>::value
struct mpp::is_monoid<Tp,Op2>
    : public std::true_type
{};

// left-near-ring structure

template <typename Tp, typename Op1, typename Op2>
    requires mpp::has_left_distributivity<Tp,Op1,Op2>::value
        && mpp::is_group<Tp,Op1>::value
        && mpp::is_semigroup<Tp,Op2>::value
struct mpp::is_left_near_ring<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_left_near_ring<Tp,Op1,Op2>::value
struct mpp::has_left_distributivity<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_left_near_ring<Tp,Op1,Op2>::value
struct mpp::is_group<Tp,Op1>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_left_near_ring<Tp,Op1,Op2>::value
struct mpp::is_semigroup<Tp,Op2>
    : public std::true_type
{};

// right-near-ring structure

template <typename Tp, typename Op1, typename Op2>
    requires mpp::has_right_distributivity<Tp,Op1,Op2>::value
        && mpp::is_group<Tp,Op1>::value
        && mpp::is_semigroup<Tp,Op2>::value
struct mpp::is_right_near_ring<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_right_near_ring<Tp,Op1,Op2>::value
struct mpp::has_right_distributivity<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_right_near_ring<Tp,Op1,Op2>::value
struct mpp::is_group<Tp,Op1>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_right_near_ring<Tp,Op1,Op2>::value
struct mpp::is_semigroup<Tp,Op2>
    : public std::true_type
{};

// near-ring structure

template <typename Tp, typename Op1, typename Op2>
    requires mpp::has_distributivity<Tp,Op1,Op2>::value
        && mpp::is_group<Tp,Op1>::value
        && mpp::is_semigroup<Tp,Op2>::value
struct mpp::is_near_ring<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_near_ring<Tp,Op1,Op2>::value
struct mpp::has_distributivity<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_near_ring<Tp,Op1,Op2>::value
struct mpp::is_group<Tp,Op1>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_near_ring<Tp,Op1,Op2>::value
struct mpp::is_semigroup<Tp,Op2>
    : public std::true_type
{};

// pseudo-ring structure

template <typename Tp, typename Op1, typename Op2>
    requires mpp::has_distributivity<Tp,Op1,Op2>::value
        && mpp::is_abelian_group<Tp,Op1>::value
        && mpp::is_semigroup<Tp,Op2>::value
struct mpp::is_pseudo_ring<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_pseudo_ring<Tp,Op1,Op2>::value
struct mpp::has_distributivity<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_pseudo_ring<Tp,Op1,Op2>::value
struct mpp::is_abelian_group<Tp,Op1>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_pseudo_ring<Tp,Op1,Op2>::value
struct mpp::is_semigroup<Tp,Op2>
    : public std::true_type
{};

// ring structure

template <typename Tp, typename Op1, typename Op2>
    requires mpp::has_distributivity<Tp,Op1,Op2>::value
        && mpp::is_abelian_group<Tp,Op1>::value
        && mpp::is_monoid<Tp,Op2>::value
struct mpp::is_ring<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_ring<Tp,Op1,Op2>::value
struct mpp::has_distributivity<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_ring<Tp,Op1,Op2>::value
struct mpp::is_abelian_group<Tp,Op1>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_ring<Tp,Op1,Op2>::value
struct mpp::is_monoid<Tp,Op2>
    : public std::true_type
{};

// commutative-ring structure

template <typename Tp, typename Op1, typename Op2>
    requires mpp::has_distributivity<Tp,Op1,Op2>::value
        && mpp::is_abelian_group<Tp,Op1>::value
        && mpp::is_commutative_monoid<Tp,Op2>::value
struct mpp::is_commutative_ring<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_commutative_ring<Tp,Op1,Op2>::value
struct mpp::has_distributivity<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_commutative_ring<Tp,Op1,Op2>::value
struct mpp::is_abelian_group<Tp,Op1>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_commutative_ring<Tp,Op1,Op2>::value
struct mpp::is_commutative_monoid<Tp,Op2>
    : public std::true_type
{};

// skew-field structure

template <typename Tp, typename Op1, typename Op2>
    requires mpp::has_distributivity<Tp,Op1,Op2>::value
        && mpp::is_abelian_group<Tp,Op1>::value
        && mpp::is_group<Tp,Op2>::value
struct mpp::is_skew_field<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_skew_field<Tp,Op1,Op2>::value
struct mpp::has_distributivity<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_skew_field<Tp,Op1,Op2>::value
struct mpp::is_abelian_group<Tp,Op1>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_skew_field<Tp,Op1,Op2>::value
struct mpp::is_group<Tp,Op2>
    : public std::true_type
{};

// field structure

template <typename Tp, typename Op1, typename Op2>
    requires mpp::has_distributivity<Tp,Op1,Op2>::value
        && mpp::is_abelian_group<Tp,Op1>::value
        && mpp::is_abelian_group<Tp,Op2>::value
struct mpp::is_field<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_field<Tp,Op1,Op2>::value
struct mpp::has_distributivity<Tp,Op1,Op2>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_field<Tp,Op1,Op2>::value
struct mpp::is_abelian_group<Tp,Op1>
    : public std::true_type
{};

template <typename Tp, typename Op1, typename Op2>
    requires mpp::is_field<Tp,Op1,Op2>::value
struct mpp::is_abelian_group<Tp,Op2>
    : public std::true_type
{};

/* Domain-like structure tags *********************************************** */

// // integral-domain structure

// template <typename Tp, typename Op1, typename Op2>
//     requires std::is_arithmetic<Tp>::value && std::is_signed<Tp>::value
//         && std::is_base_of<mpp::operation,Op1>::value
//         && std::is_base_of<mpp::operation,Op2>::value
// struct mpp::is_integral_domain<Tp,Op1,Op2>
//     : public std::true_type
// {};

// template <typename Tp, typename Op1, typename Op2>
//     requires mpp::is_field<Tp,Op1,Op2>::value
// struct mpp::is_integral_domain<Tp,Op1,Op2>
//     : public std::true_type
// {};
