
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

    /* Axiom tags *********************************************************** */

    // closure axiom

    template <typename Tp, typename Op>
    struct has_closure
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires std::is_base_of<operation,Op>::value
            && std::is_arithmetic<Tp>::value
    struct has_closure
        : public std::true_type
    {};

    // identity axiom

    template <typename Tp, typename Op>
    struct has_identity
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires std::is_base_of<operation,Op>::value
            && std::is_arithmetic<Tp>::value
    struct has_identity
        : public std::true_type
    {};

    // invertibility axiom

    template <typename Tp, typename Op>
    struct has_invertibility
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires std::is_base_of<operation,Op>::value
            && std::is_arithmetic<Tp>::value && std::is_signed<Tp>::value
    struct has_invertibility
        : public std::true_type
    {};

    // associativity axiom

    template <typename Tp, typename Op>
    struct has_associativity
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires std::is_base_of<operation,Op>::value
            && std::is_arithmetic<Tp>::value
    struct has_associativity
        : public std::true_type
    {};

    // commutativity axiom

    template <typename Tp, typename Op>
    struct has_commutativity
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires std::is_base_of<operation,Op>::value
            && std::is_arithmetic<Tp>::value
    struct has_commutativity
        : public std::true_type
    {};

    // left-distributivity axiom

    template <typename Tp, typename Op1, typename Op2>
    struct has_left_distributivity
        : public std::false_type
    {};

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct has_left_distributivity<Tp,op_add,op_mul>
        : public std::true_type
    {};

    // right-distributivity axiom

    template <typename Tp, typename Op1, typename Op2>
    struct has_right_distributivity
        : public std::false_type
    {};

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct has_right_distributivity<Tp,op_add,op_mul>
        : public std::true_type
    {};

    // distributivity axiom

    template <typename Tp, typename Op1, typename Op2>
    struct has_distributivity
        : public std::false_type
    {};

    template <typename Tp>
        requires has_left_distributivity<Tp,op_add,op_mul>::value
            && has_right_distributivity<Tp,op_add,op_mul>::value
    struct has_distributivity<Tp,op_add,op_mul>
        : public std::true_type
    {};

    /* Group-like structure tags ******************************************** */

    // semigroupoid structure

    template <typename Tp, typename Op>
    struct is_semigroupoid
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_associativity<Tp,Op>::value
    struct is_semigroupoid
        : public std::true_type
    {};

    // small-category structure

    template <typename Tp, typename Op>
    struct is_small_category
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_associativity<Tp,Op>::value
            && has_identity<Tp,Op>::value
    struct is_small_category
        : public std::true_type
    {};

    // groupoid structure

    template <typename Tp, typename Op>
    struct is_groupoid
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_associativity<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
    struct is_groupoid
        : public std::true_type
    {};

    // magma structure

    template <typename Tp, typename Op>
    struct is_magma
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
    struct is_magma
        : public std::true_type
    {};

    // quasigroup structure

    template <typename Tp, typename Op>
    struct is_quasigroup
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
    struct is_quasigroup
        : public std::true_type
    {};

    // unary-magma structure

    template <typename Tp, typename Op>
    struct is_unary_magma
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
    struct is_unary_magma
        : public std::true_type
    {};

    // loop structure

    template <typename Tp, typename Op>
    struct is_loop
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
    struct is_loop
        : public std::true_type
    {};

    // semigroup structure

    template <typename Tp, typename Op>
    struct is_semigroup
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_semigroup
        : public std::true_type
    {};

    // invertible-semigroup structure

    template <typename Tp, typename Op>
    struct is_invertible_semigroup
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_invertible_semigroup
        : public std::true_type
    {};

    // monoid structure

    template <typename Tp, typename Op>
    struct is_monoid
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_monoid
        : public std::true_type
    {};

    // commutative-monoid structure

    template <typename Tp, typename Op>
    struct is_commutative_monoid
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_associativity<Tp,Op>::value
            && has_commutativity<Tp,Op>::value
    struct is_commutative_monoid
        : public std::true_type
    {};

    // group structure

    template <typename Tp, typename Op>
    struct is_group
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_group
        : public std::true_type
    {};

    // abelian-group structure

    template <typename Tp, typename Op>
    struct is_abelian_group
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
            && has_associativity<Tp,Op>::value
            && has_commutativity<Tp,Op>::value
    struct is_abelian_group
        : public std::true_type
    {};

    /* Ring-like structure tags ********************************************* */

    // semiring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_semiring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_commutative_monoid<Tp,Op1>::value
            && is_monoid<Tp,Op2>::value
    struct is_semiring
        : public std::true_type
    {};

    // left-near-ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_left_near_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_left_distributivity<Tp,Op1,Op2>::value
            && is_group<Tp,Op1>::value
            && is_semigroup<Tp,Op2>::value
    struct is_left_near_ring
        : public std::true_type
    {};

    // right-near-ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_right_near_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_right_distributivity<Tp,Op1,Op2>::value
            && is_group<Tp,Op1>::value
            && is_semigroup<Tp,Op2>::value
    struct is_right_near_ring
        : public std::true_type
    {};

    // near-ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_near_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_group<Tp,Op1>::value
            && is_semigroup<Tp,Op2>::value
    struct is_near_ring
        : public std::true_type
    {};

    // pseudo-ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_pseudo_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_semigroup<Tp,Op2>::value
    struct is_pseudo_ring
        : public std::true_type
    {};

    // ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_monoid<Tp,Op2>::value
    struct is_ring
        : public std::true_type
    {};

    // trivial-ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_trivial_ring
        : public std::false_type
    {};

    // commutative-ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_commutative_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_commutative_monoid<Tp,Op2>::value
    struct is_commutative_ring
        : public std::true_type
    {};

    // integral-domain structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_integral_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires std::is_arithmetic<Tp>::value && std::is_signed<Tp>::value
            && std::is_base_of<operation,Op1>::value
            && std::is_base_of<operation,Op2>::value
    struct is_integral_domain
        : public std::false_type
    {};

    // skew-field structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_skew_field
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_group<Tp,Op2>::value
    struct is_skew_field
        : public std::true_type
    {};

    // field structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_field
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_abelian_group<Tp,Op2>::value
    struct is_field
        : public std::true_type
    {};

} // namespace mpp
