
#ifndef __HH_MPP_GROUP_TRAITS
#define __HH_MPP_GROUP_TRAITS

#include <type_traits>

/* ************************************************************************** */
// Definitions
/* ************************************************************************** */

namespace mpp
{

    /* Helper metatemplates ************************************************* */

    template <typename Tp>
    struct is_not
        : public std::integral_constant<bool,!bool(Tp::value)>
    {};

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
    // (integrate into structure tags)

    /* Tag helpers ********************************************************** */

    template <typename Tp, typename Op>
    struct identity {
        // static Tp get();             <- expected in specializations
    };

    template <typename Tp, typename Op>
    struct inverse {
        // static Tp get(Tp const&);    <- expected in specializations
    };

    /* Group-like axiom tags ************************************************ */

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

    /* Group-like structure tags ******************************************** */

    template <typename Tp, typename Op>
    struct is_trivial_grouplike
        : public std::false_type
    {};

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

    /* Ring-like axiom tags ************************************************* */

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

    /* Ring-like structure tags ********************************************* */

    template <typename Tp, typename Op1, typename Op2>
    struct is_trivial_ringlike
        : public std::false_type
    {};

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

    /* Domain-like axiom tags *********************************************** */

    template <typename Tp, typename Op>
    struct has_nonzero_products
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct has_gcd
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct has_unique_factorisation
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
    struct has_principal_ideals
        : public std::false_type
    {};

    /* Domain-like structure tags ******************************************* */

    template <typename Tp, typename Op1, typename Op2>
    struct is_integral_domain
        : public std::false_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    // struct is_integrally_closed_domain
    //     : public std::false_type
    // {};

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

    // template <typename Tp, typename Op1, typename Op2>
    // struct is_euclidean_domain
    //     : public std::false_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    // struct is_algebraically_closed_domain
    //     : public std::false_type
    // {};

} // namespace mpp

/* ************************************************************************** */
// Specialization
/* ************************************************************************** */

namespace mpp
{

    /* Tag helpers ********************************************************** */

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
            && has_identity<Tp,op_add>::value
    struct identity<Tp,op_add>
    {
        static Tp get() { return Tp{0}; }
    };

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
            && has_identity<Tp,op_mul>::value
    struct identity<Tp,op_mul>
    {
        static Tp get() { return Tp{1}; }
    };

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
            && has_invertibility<Tp,op_add>::value
    struct inverse<Tp,op_add>
    {
        static Tp get(Tp e) { return -e; }
    };

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
            && has_invertibility<Tp,op_mul>::value
    struct inverse<Tp,op_mul>
    {
        static Tp get(Tp e) { return 1/e; }
    };

    /* Group-like axiom tags ************************************************ */

    template <typename Tp, typename Op>
        requires std::is_arithmetic<Tp>::value
    struct has_closure<Tp,Op>
        : public std::true_type
    {};

    template <typename Tp, typename Op>
        requires std::is_arithmetic<Tp>::value
    struct has_identity<Tp,Op>
        : public std::true_type
    {};

    template <typename Tp, typename Op>
        requires std::is_signed<Tp>::value
    struct has_invertibility<Tp,Op>
        : public std::true_type
    {};

    template <typename Tp, typename Op>
        requires std::is_arithmetic<Tp>::value
    struct has_associativity<Tp,Op>
        : public std::true_type
    {};

    template <typename Tp, typename Op>
        requires std::is_arithmetic<Tp>::value
    struct has_commutativity<Tp,Op>
        : public std::true_type
    {};

    /* Group-like structure tags ******************************************** */

    // semigroup structure

    template <typename Tp, typename Op>
        requires has_associativity<Tp,Op>::value
    struct is_semigroupoid<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_semigroupoid<Tp,Op>::value
    // struct has_associativity<Tp,Op>
    //     : public std::true_type
    // {};

    // small-category structure

    template <typename Tp, typename Op>
        requires has_associativity<Tp,Op>::value
            && has_identity<Tp,Op>::value
    struct is_small_category<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_small_category<Tp,Op>::value
    // struct has_associativity<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_small_category<Tp,Op>::value
    // struct has_identity<Tp,Op>
    //     : public std::true_type
    // {};

    // groupid structure

    template <typename Tp, typename Op>
        requires has_associativity<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
    struct is_groupoid<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_groupoid<Tp,Op>::value
    // struct has_associativity<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_groupoid<Tp,Op>::value
    // struct has_invertibility<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_groupoid<Tp,Op>::value
    // struct has_identity<Tp,Op>
    //     : public std::true_type
    // {};

    // magma structure

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
    struct is_magma<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_magma<Tp,Op>::value
    // struct has_closure<Tp,Op>
    //     : public std::true_type
    // {};

    // quasigroup structure

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
    struct is_quasigroup<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_quasigroup<Tp,Op>::value
    // struct has_closure<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_quasigroup<Tp,Op>::value
    // struct has_invertibility<Tp,Op>
    //     : public std::true_type
    // {};

    // unary magma structure

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
    struct is_unary_magma<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_unary_magma<Tp,Op>::value
    // struct has_closure<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_unary_magma<Tp,Op>::value
    // struct has_identity<Tp,Op>
    //     : public std::true_type
    // {};

    // loop structure

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
    struct is_loop<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_loop<Tp,Op>::value
    // struct has_closure<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_loop<Tp,Op>::value
    // struct has_identity<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_loop<Tp,Op>::value
    // struct has_invertibility<Tp,Op>
    //     : public std::true_type
    // {};

    // semigroup structure

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_semigroup<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_semigroup<Tp,Op>::value
    // struct has_closure<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_semigroup<Tp,Op>::value
    // struct has_associativity<Tp,Op>
    //     : public std::true_type
    // {};

    // invertible semigroup structure

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_invertible_semigroup<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_invertible_semigroup<Tp,Op>::value
    // struct has_closure<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_invertible_semigroup<Tp,Op>::value
    // struct has_invertibility<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_invertible_semigroup<Tp,Op>::value
    // struct has_associativity<Tp,Op>
    //     : public std::true_type
    // {};

    // monoid structure

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_monoid<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_monoid<Tp,Op>::value
    // struct has_closure<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_monoid<Tp,Op>::value
    // struct has_identity<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_monoid<Tp,Op>::value
    // struct has_associativity<Tp,Op>
    //     : public std::true_type
    // {};

    // commutative monoid structure

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_associativity<Tp,Op>::value
            && has_commutativity<Tp,Op>::value
    struct is_commutative_monoid<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_commutative_monoid<Tp,Op>::value
    // struct has_closure<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_commutative_monoid<Tp,Op>::value
    // struct has_identity<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_commutative_monoid<Tp,Op>::value
    // struct has_associativity<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_commutative_monoid<Tp,Op>::value
    // struct has_commutativity<Tp,Op>
    //     : public std::true_type
    // {};

    // group structure

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_group<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_group<Tp,Op>::value
    // struct has_closure<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_group<Tp,Op>::value
    // struct has_identity<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_group<Tp,Op>::value
    // struct has_invertibility<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_group<Tp,Op>::value
    // struct has_associativity<Tp,Op>
    //     : public std::true_type
    // {};

    // abelian group structure

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
            && has_associativity<Tp,Op>::value
            && has_commutativity<Tp,Op>::value
    struct is_abelian_group<Tp,Op>
        : public std::true_type
    {};

    // template <typename Tp, typename Op>
    //     requires is_abelian_group<Tp,Op>::value
    // struct has_closure<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_abelian_group<Tp,Op>::value
    // struct has_identity<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_abelian_group<Tp,Op>::value
    // struct has_invertibility<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_abelian_group<Tp,Op>::value
    // struct has_associativity<Tp,Op>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op>
    //     requires is_abelian_group<Tp,Op>::value
    // struct has_commutativity<Tp,Op>
    //     : public std::true_type
    // {};

    /* Ring-like axiom tags ************************************************* */

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct has_left_distributivity<Tp,op_add,op_mul>
        : public std::true_type
    {};

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct has_right_distributivity<Tp,op_add,op_mul>
        : public std::true_type
    {};

    template <typename Tp>
        requires has_left_distributivity<Tp,op_add,op_mul>::value
            && has_right_distributivity<Tp,op_add,op_mul>::value
    struct has_distributivity<Tp,op_add,op_mul>
        : public std::true_type
    {};

    /* Ring-like structure tags ********************************************* */

    // semiring structure

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_commutative_monoid<Tp,Op1>::value
            && is_monoid<Tp,Op2>::value
    struct is_semiring<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_semiring<Tp,Op1,Op2>::value
    // struct has_distributivity<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_semiring<Tp,Op1,Op2>::value
    // struct is_commutative_monoid<Tp,Op1>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_semiring<Tp,Op1,Op2>::value
    // struct is_monoid<Tp,Op2>
    //     : public std::true_type
    // {};

    // left-near-ring structure

    template <typename Tp, typename Op1, typename Op2>
        requires has_left_distributivity<Tp,Op1,Op2>::value
            && is_group<Tp,Op1>::value
            && is_semigroup<Tp,Op2>::value
    struct is_left_near_ring<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_left_near_ring<Tp,Op1,Op2>::value
    // struct has_left_distributivity<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_left_near_ring<Tp,Op1,Op2>::value
    // struct is_group<Tp,Op1>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_left_near_ring<Tp,Op1,Op2>::value
    // struct is_semigroup<Tp,Op2>
    //     : public std::true_type
    // {};

    // right-near-ring structure

    template <typename Tp, typename Op1, typename Op2>
        requires has_right_distributivity<Tp,Op1,Op2>::value
            && is_group<Tp,Op1>::value
            && is_semigroup<Tp,Op2>::value
    struct is_right_near_ring<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_right_near_ring<Tp,Op1,Op2>::value
    // struct has_right_distributivity<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_right_near_ring<Tp,Op1,Op2>::value
    // struct is_group<Tp,Op1>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_right_near_ring<Tp,Op1,Op2>::value
    // struct is_semigroup<Tp,Op2>
    //     : public std::true_type
    // {};

    // near-ring structure

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_group<Tp,Op1>::value
            && is_semigroup<Tp,Op2>::value
    struct is_near_ring<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_near_ring<Tp,Op1,Op2>::value
    // struct has_distributivity<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_near_ring<Tp,Op1,Op2>::value
    // struct is_group<Tp,Op1>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_near_ring<Tp,Op1,Op2>::value
    // struct is_semigroup<Tp,Op2>
    //     : public std::true_type
    // {};

    // pseudo-ring structure

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_semigroup<Tp,Op2>::value
    struct is_pseudo_ring<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_pseudo_ring<Tp,Op1,Op2>::value
    // struct has_distributivity<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_pseudo_ring<Tp,Op1,Op2>::value
    // struct is_abelian_group<Tp,Op1>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_pseudo_ring<Tp,Op1,Op2>::value
    // struct is_semigroup<Tp,Op2>
    //     : public std::true_type
    // {};

    // ring structure

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_monoid<Tp,Op2>::value
    struct is_ring<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_ring<Tp,Op1,Op2>::value
    // struct has_distributivity<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_ring<Tp,Op1,Op2>::value
    // struct is_abelian_group<Tp,Op1>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_ring<Tp,Op1,Op2>::value
    // struct is_monoid<Tp,Op2>
    //     : public std::true_type
    // {};

    // commutative-ring structure

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_commutative_monoid<Tp,Op2>::value
    struct is_commutative_ring<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_commutative_ring<Tp,Op1,Op2>::value
    // struct has_distributivity<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_commutative_ring<Tp,Op1,Op2>::value
    // struct is_abelian_group<Tp,Op1>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_commutative_ring<Tp,Op1,Op2>::value
    // struct is_commutative_monoid<Tp,Op2>
    //     : public std::true_type
    // {};

    // skew-field structure

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_group<Tp,Op2>::value
    struct is_skew_field<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_skew_field<Tp,Op1,Op2>::value
    // struct has_distributivity<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_skew_field<Tp,Op1,Op2>::value
    // struct is_abelian_group<Tp,Op1>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_skew_field<Tp,Op1,Op2>::value
    // struct is_group<Tp,Op2>
    //     : public std::true_type
    // {};

    // field structure

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_abelian_group<Tp,Op2>::value
    struct is_field<Tp,Op1,Op2>
        : public std::true_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires is_field<Tp,Op1,Op2>::value
    struct has_distributivity<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_field<Tp,Op1,Op2>::value
    // struct is_abelian_group<Tp,Op1>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_field<Tp,Op1,Op2>::value
    // struct is_abelian_group<Tp,Op2>
    //     : public std::true_type
    // {};

    /* Domain-like axiom tags *********************************************** */

    template <typename Tp>
        requires std::is_signed<Tp>::value
    struct has_nonzero_products<Tp,op_add>
        : public std::true_type
    {};

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct has_nonzero_products<Tp,op_mul>
        : public std::true_type
    {};

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct has_gcd<Tp,op_add,op_mul>
        : public std::true_type
    {};

    template <typename Tp>
        requires std::is_integral<Tp>::value
    struct has_unique_factorisation<Tp,op_add,op_mul>
        : public std::true_type
    {};

    template <typename Tp>
        requires std::is_integral<Tp>::value
    struct has_principal_ideals<Tp,op_add,op_mul>
        : public std::true_type
    {};

    /* Domain-like structure tags ******************************************* */

    // integral-domain structure

    template <typename Tp, typename Op1, typename Op2>
        requires is_commutative_ring<Tp,Op1,Op2>::value
            && is_not<is_trivial_ringlike<Tp,Op1,Op2>>::value
            && has_nonzero_products<Tp,Op2>::value
    struct is_integral_domain<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_integral_domain<Tp,Op1,Op2>::value
    // struct is_commutative_ring<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_integral_domain<Tp,Op1,Op2>::value
    // struct has_nonzero_products<Tp,Op2>
    //     : public std::true_type
    // {};

    // gcd-domain structure

    template <typename Tp, typename Op1, typename Op2>
        requires is_integral_domain<Tp,Op1,Op2>::value
            && has_gcd<Tp,Op1,Op2>::value
    struct is_gcd_domain<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_gcd_domain<Tp,Op1,Op2>::value
    // struct is_integral_domain<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_gcd_domain<Tp,Op1,Op2>::value
    // struct has_gcd<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // unique-factorisation-domain structure

    template <typename Tp, typename Op1, typename Op2>
        requires is_gcd_domain<Tp,Op1,Op2>::value
            && has_unique_factorisation<Tp,Op1,Op2>::value
    struct is_unique_factorisation_domain<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_unique_factorisation_domain<Tp,Op1,Op2>::value
    // struct is_gcd_domain<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_unique_factorisation_domain<Tp,Op1,Op2>::value
    // struct has_unique_factorisation<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // principal-ideal-domain structure

    template <typename Tp, typename Op1, typename Op2>
        requires is_unique_factorisation_domain<Tp,Op1,Op2>::value
            && has_principal_ideals<Tp,Op1,Op2>::value
    struct is_principal_ideal_domain<Tp,Op1,Op2>
        : public std::true_type
    {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_principal_ideal_domain<Tp,Op1,Op2>::value
    // struct is_unique_factorisation_domain<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

    // template <typename Tp, typename Op1, typename Op2>
    //     requires is_principal_ideal_domain<Tp,Op1,Op2>::value
    // struct has_principal_ideals<Tp,Op1,Op2>
    //     : public std::true_type
    // {};

} // namespace mpp

#endif /* __HH_MPP_GROUP_TRAITS */
