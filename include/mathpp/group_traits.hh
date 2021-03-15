
#ifndef __HH_MPP_GROUP_TRAITS
#define __HH_MPP_GROUP_TRAITS

#include <type_traits>

/* ************************************************************************** */
// Operation Definitions
/* ************************************************************************** */

namespace mpp
{

    struct operation
    {};

    struct op_add
        : public operation
    {};

    struct op_mul
        : public operation
    {};

} // namespace mpp

/* ************************************************************************** */
// Helper Definitions
/* ************************************************************************** */

namespace mpp
{

    /* Helper metatemplates ************************************************* */

    template <typename Tp>
    struct is_not
        : public std::integral_constant<bool,!bool(Tp::value)>
    {};

    /* Tag helpers ********************************************************** */

    template <typename Tp, typename Op>
    struct identity
    {
        // static Tp get();             <- expected in specializations
    };

    template <typename Tp, typename Op>
    struct inverse
    {
        // static Tp get(Tp const&);    <- expected in specializations
        // static Tp& make(Tp&);        <- expected in specializations
    };

    template <typename Tp, typename Op>
    struct absolute
    {
        // static Tp get(Tp const&);    <- expected in specializations
        // static Tp& make(Tp&);        <- expected in specializations
    };

} // namespace mpp

/* ************************************************************************** */
// Tag Definitions
/* ************************************************************************** */

namespace mpp
{

    /* Ordering tags ******************************************************** */

    // TODO Ordering tags
    // (integrate into structure tags)

    /* Group-like axiom tags ************************************************ */

    // closure axiom

    template <typename Tp, typename Op>
    struct has_closure
        : public std::false_type
    {};

    #define MPP_GIVE_CLOSURE(TP,OP)\
        template <>\
        struct has_closure<TP,OP> : public std::true_type {};\

    #define MPP_GIVE_CLOSURE_COND(SPEC,TP,OP,COND)\
        template <SPEC> requires COND\
        struct has_closure<TP,OP> : public std::true_type {};\

    // identity axiom

    template <typename Tp, typename Op>
    struct has_identity
        : public std::false_type
    {};

    #define MPP_GIVE_IDENTITY(TP,OP)\
        template <>\
        struct has_identity<TP,OP> : public std::true_type {};\

    #define MPP_GIVE_IDENTITY_COND(SPEC,TP,OP,COND)\
        template <SPEC> requires COND\
        struct has_identity<TP,OP> : public std::true_type {};\

    // invertibility axiom

    template <typename Tp, typename Op>
    struct has_invertibility
        : public std::false_type
    {};

    #define MPP_GIVE_INVERTIBILITY(TP,OP)\
        template <>\
        struct has_invertibility<TP,OP> : public std::true_type {};\

    #define MPP_GIVE_INVERTIBILITY_COND(SPEC,TP,OP,COND)\
        template <SPEC> requires COND\
        struct has_invertibility<TP,OP> : public std::true_type {};\

    // associativity axiom

    template <typename Tp, typename Op>
    struct has_associativity
        : public std::false_type
    {};

    #define MPP_GIVE_ASSOCIATIVITY(TP,OP)\
        template <>\
        struct has_associativity<TP,OP> : public std::true_type {};\

    #define MPP_GIVE_ASSOCIATIVITY_COND(SPEC,TP,OP,COND)\
        template <SPEC> requires COND\
        struct has_associativity<TP,OP> : public std::true_type {};\

    // commutativity axiom

    template <typename Tp, typename Op>
    struct has_commutativity
        : public std::false_type
    {};

    #define MPP_GIVE_COMMUTATIVITY(TP,OP)\
        template <>\
        struct has_commutativity<TP,OP> : public std::true_type {};\

    #define MPP_GIVE_COMMUTATIVITY_COND(SPEC,TP,OP,COND)\
        template <SPEC> requires COND\
        struct has_commutativity<TP,OP> : public std::true_type {};\

    /* Group-like structure tags ******************************************** */

    // trivial-grouplike structure

    template <typename Tp, typename Op>
    struct is_trivial_grouplike
        : public std::false_type
    {};

    #define MPP_DEFINE_TRIVIAL_GROUP(TP,OP)\
        template <>\
        struct is_trivial_grouplike<TP,OP> : public std::true_type {};\

    #define MPP_REQUIRE_TRIVIAL_GROUP_COND(SPEC,TP,OP,COND)\
        template <SPEC> requires COND\
        struct is_trivial_grouplike<TP,OP> : public std::true_type {};\

    // semigroupoid structure

    template <typename Tp, typename Op>
    struct is_semigroupoid
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_associativity<Tp,Op>::value
    struct is_semigroupoid<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_SEMIGROUPOID(TP,OP)\
        MPP_GIVE_ASSOCIATIVITY(TP,OP);\

    #define MPP_REQUIRE_SEMIGROUPOID(SPEC,TP,OP,COND)\
        MPP_GIVE_ASSOCIATIVITY_COND(SPEC,TP,OP,COND);\

    // small-category structure

    template <typename Tp, typename Op>
    struct is_small_category
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_identity<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_small_category<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_SMALL_CATEGORY(TP,OP)\
        MPP_GIVE_IDENTITY(TP,OP);\
        MPP_GIVE_ASSOCIATIVITY(TP,OP);\

    #define MPP_REQUIRE_SMALL_CATEGORY(SPEC,TP,OP,COND)\
        MPP_GIVE_IDENTITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_ASSOCIATIVITY_COND(SPEC,TP,OP,COND);\

    // groupoid structure

    template <typename Tp, typename Op>
    struct is_groupoid
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_identity<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_groupoid<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_GROUPOID(TP,OP)\
        MPP_GIVE_IDENTITY(TP,OP);\
        MPP_GIVE_INVERTIBILITY(TP,OP);\
        MPP_GIVE_ASSOCIATIVITY(TP,OP);\

    #define MPP_REQUIRE_GROUPOID(SPEC,TP,OP,COND)\
        MPP_GIVE_IDENTITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_INVERTIBILITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_ASSOCIATIVITY_COND(SPEC,TP,OP,COND);\

    // magma structure

    template <typename Tp, typename Op>
    struct is_magma
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
    struct is_magma<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_MAGMA(TP,OP)\
        MPP_GIVE_CLOSURE(TP,OP);\

    #define MPP_REQUIRE_MAGMA(SPEC,TP,OP,COND)\
        MPP_GIVE_CLOSURE_COND(SPEC,TP,OP,COND);\

    // unary-magma structure

    template <typename Tp, typename Op>
    struct is_unary_magma
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
    struct is_unary_magma<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_UNARY_MAGMA(TP,OP)\
        MPP_GIVE_CLOSURE(TP,OP);\
        MPP_GIVE_IDENTITY(TP,OP);\

    #define MPP_REQUIRE_UNARY_MAGMA(SPEC,TP,OP,COND)\
        MPP_GIVE_CLOSURE_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_IDENTITY_COND(SPEC,TP,OP,COND);\

    // quasigroup structure

    template <typename Tp, typename Op>
    struct is_quasigroup
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
    struct is_quasigroup<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_QUASIGROUP(TP,OP)\
        MPP_GIVE_CLOSURE(TP,OP);\
        MPP_GIVE_INVERTIBILITY(TP,OP);\

    #define MPP_REQUIRE_QUASIGROUP(SPEC,TP,OP,COND)\
        MPP_GIVE_CLOSURE_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_INVERTIBILITY_COND(SPEC,TP,OP,COND);\

    // loop structure

    template <typename Tp, typename Op>
    struct is_loop
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
    struct is_loop<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_LOOP(TP,OP)\
        MPP_GIVE_CLOSURE(TP,OP);\
        MPP_GIVE_IDENTITY(TP,OP);\
        MPP_GIVE_INVERTIBILITY(TP,OP);\

    #define MPP_REQUIRE_LOOP(SPEC,TP,OP,COND)\
        MPP_GIVE_CLOSURE_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_IDENTITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_INVERTIBILITY_COND(SPEC,TP,OP,COND);\

    // semigroup structure

    template <typename Tp, typename Op>
    struct is_semigroup
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_semigroup<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_SEMIGROUP(TP,OP)\
        MPP_GIVE_CLOSURE(TP,OP);\
        MPP_GIVE_ASSOCIATIVITY(TP,OP);\

    #define MPP_REQUIRE_SEMIGROUP(SPEC,TP,OP,COND)\
        MPP_GIVE_CLOSURE_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_ASSOCIATIVITY_COND(SPEC,TP,OP,COND);\

    // invertible-semigroup structure

    template <typename Tp, typename Op>
    struct is_invertible_semigroup
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_invertible_semigroup<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_INVERTIBLE_SEMIGROUP(TP,OP)\
        MPP_GIVE_CLOSURE(TP,OP);\
        MPP_GIVE_INVERTIBILITY(TP,OP);\
        MPP_GIVE_ASSOCIATIVITY(TP,OP);\

    #define MPP_REQUIRE_INVERTIBLE_SEMIGROUP(SPEC,TP,OP,COND)\
        MPP_GIVE_CLOSURE_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_INVERTIBILITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_ASSOCIATIVITY_COND(SPEC,TP,OP,COND);\

    // monoid structure

    template <typename Tp, typename Op>
    struct is_monoid
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_monoid<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_MONOID(TP,OP)\
        MPP_GIVE_CLOSURE(TP,OP);\
        MPP_GIVE_IDENTITY(TP,OP);\
        MPP_GIVE_ASSOCIATIVITY(TP,OP);\

    #define MPP_REQUIRE_MONOID(SPEC,TP,OP,COND)\
        MPP_GIVE_CLOSURE_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_IDENTITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_ASSOCIATIVITY_COND(SPEC,TP,OP,COND);\

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
    struct is_commutative_monoid<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_COMMUTATIVE_MONOID(TP,OP)\
        MPP_GIVE_CLOSURE(TP,OP);\
        MPP_GIVE_IDENTITY(TP,OP);\
        MPP_GIVE_ASSOCIATIVITY(TP,OP);\
        MPP_GIVE_COMMUTATIVITY(TP,OP);\

    #define MPP_REQUIRE_COMMUTATIVE_MONOID(SPEC,TP,OP,COND)\
        MPP_GIVE_CLOSURE_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_IDENTITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_ASSOCIATIVITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_COMMUTATIVITY_COND(SPEC,TP,OP,COND);\

    // group strucure

    template <typename Tp, typename Op>
    struct is_group
        : public std::false_type
    {};

    template <typename Tp, typename Op>
        requires has_closure<Tp,Op>::value
            && has_identity<Tp,Op>::value
            && has_invertibility<Tp,Op>::value
            && has_associativity<Tp,Op>::value
    struct is_group<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_GROUP(TP,OP)\
        MPP_GIVE_CLOSURE(TP,OP);\
        MPP_GIVE_IDENTITY(TP,OP);\
        MPP_GIVE_INVERTIBILITY(TP,OP);\
        MPP_GIVE_ASSOCIATIVITY(TP,OP);\

    #define MPP_REQUIRE_GROUP(SPEC,TP,OP,COND)\
        MPP_GIVE_CLOSURE_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_IDENTITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_INVERTIBILITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_ASSOCIATIVITY_COND(SPEC,TP,OP,COND);\

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
    struct is_abelian_group<Tp,Op>
        : public std::true_type
    {};

    #define MPP_DEFINE_ABELIAN_GROUP(TP,OP)\
        MPP_GIVE_CLOSURE(TP,OP);\
        MPP_GIVE_IDENTITY(TP,OP);\
        MPP_GIVE_INVERTIBILITY(TP,OP);\
        MPP_GIVE_ASSOCIATIVITY(TP,OP);\
        MPP_GIVE_COMMUTATIVITY(TP,OP);\

    #define MPP_REQUIRE_ABELIAN_GROUP(SPEC,TP,OP,COND)\
        MPP_GIVE_CLOSURE_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_IDENTITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_INVERTIBILITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_ASSOCIATIVITY_COND(SPEC,TP,OP,COND);\
        MPP_GIVE_COMMUTATIVITY_COND(SPEC,TP,OP,COND);\

    /* Ring-like axiom tags ************************************************* */

    // left-distributivity axiom

    template <typename Tp, typename Op1, typename Op2>
    struct has_left_distributivity
        : public std::false_type
    {};

    #define MPP_GIVE_LEFT_DISTRIBUTIVITY(TP,OP1,OP2)\
        template <>\
        struct has_left_distributivity<TP,OP1,OP2> : public std::true_type {};\

    #define MPP_GIVE_LEFT_DISTRIBUTIVITY_COND(SPEC,TP,OP1,OP2,COND)\
        template <SPEC> requires COND\
        struct has_left_distributivity<TP,OP1,OP2> : public std::true_type {};\

    // right-distributivity axiom

    template <typename Tp, typename Op1, typename Op2>
    struct has_right_distributivity
        : public std::false_type
    {};

    #define MPP_GIVE_RIGHT_DISTRIBUTIVITY(TP,OP1,OP2)\
        template <>\
        struct has_right_distributivity<TP,OP1,OP2> : public std::true_type {};\

    #define MPP_GIVE_RIGHT_DISTRIBUTIVITY_COND(SPEC,TP,OP1,OP2,COND)\
        template <SPEC> requires COND\
        struct has_right_distributivity<TP,OP1,OP2> : public std::true_type {};\

    // distributivity axiom

    template <typename Tp, typename Op1, typename Op2>
    struct has_distributivity
        : public std::false_type
    {};

    #define MPP_GIVE_DISTRIBUTIVITY(TP,OP1,OP2)\
        template <>\
        struct has_distributivity<TP,OP1,OP2> : public std::true_type {};\

    #define MPP_GIVE_DISTRIBUTIVITY_COND(SPEC,TP,OP1,OP2,COND)\
        template <SPEC> requires COND\
        struct has_distributivity<TP,OP1,OP2> : public std::true_type {};\

    /* Ring-like structure tags ********************************************* */

    // trivial-ringlike structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_trivial_ringlike
        : public std::false_type
    {};

    #define MPP_DEFINE_TRIVIAL_RINGLIKE(TP,OP1,OP2)\
        template <>\
        struct is_trivial_ringlike<TP,OP1,OP2> : public std::true_type {};\

    #define MPP_REQUIRE_TRIVIAL_RINGLIKE(SPEC,TP,OP1,OP2,COND)\
        template <SPEC> requires COND\
        struct is_trivial_ringlike<TP,OP1,OP2> : public std::true_type {};\

    // semiring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_semiring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_commutative_monoid<Tp,Op1>::value
            && is_monoid<Tp,Op2>::value
    struct is_semiring<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_SEMIRING(TP,OP1,OP2)\
        MPP_GIVE_DISTRIBUTIVITY(TP,OP1,OP2);\
        MPP_DEFINE_COMMUTATIVE_MONOID(TP,OP1);\
        MPP_DEFINE_MONOID(TP,OP2);\

    #define MPP_REQUIRE_SEMIRING(SPEC,TP,OP1,OP2,COND)\
        MPP_GIVE_DISTRIBUTIVITY_COND(SPEC,TP,OP1,OP2,COND);\
        MPP_REQUIRE_COMMUTATIVE_MONOID(SPEC,TP,OP1,COND);\
        MPP_REQUIRE_MONOID(SPEC,TP,OP2,COND);\

    // left-near-ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_left_near_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_left_distributivity<Tp,Op1,Op2>::value
            && is_group<Tp,Op1>::value
            && is_semigroup<Tp,Op2>::value
    struct is_left_near_ring<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_LEFT_NEAR_RING(TP,OP1,OP2)\
        MPP_GIVE_LEFT_DISTRIBUTIVITY(TP,OP1,OP2);\
        MPP_DEFINE_GROUP(TP,OP1);\
        MPP_DEFINE_SEMIGROUP(TP,OP2);\

    #define MPP_REQUIRE_LEFT_NEAR_RING(SPEC,TP,OP1,OP2,COND)\
        MPP_GIVE_LEFT_DISTRIBUTIVITY_COND(SPEC,TP,OP1,OP2,COND);\
        MPP_REQUIRE_GROUP(SPEC,TP,OP1,COND);\
        MPP_REQUIRE_SEMIGROUP(SPEC,TP,OP2,COND);\

    // right-near-ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_right_near_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_right_distributivity<Tp,Op1,Op2>::value
            && is_group<Tp,Op1>::value
            && is_semigroup<Tp,Op2>::value
    struct is_right_near_ring<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_RIGHT_NEAR_RING(TP,OP1,OP2)\
        MPP_GIVE_RIGHT_DISTRIBUTIVITY(TP,OP1,OP2);\
        MPP_DEFINE_GROUP(TP,OP1);\
        MPP_DEFINE_SEMIGROUP(TP,OP2);\

    #define MPP_REQUIRE_RIGHT_NEAR_RING(SPEC,TP,OP1,OP2,COND)\
        MPP_GIVE_RIGHT_DISTRIBUTIVITY_COND(SPEC,TP,OP1,OP2,COND);\
        MPP_REQUIRE_GROUP(SPEC,TP,OP1,COND);\
        MPP_REQUIRE_SEMIGROUP(SPEC,TP,OP2,COND);\

    // near-ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_near_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_group<Tp,Op1>::value
            && is_semigroup<Tp,Op2>::value
    struct is_near_ring<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_NEAR_RING(TP,OP1,OP2)\
        MPP_GIVE_DISTRIBUTIVITY(TP,OP1,OP2);\
        MPP_DEFINE_GROUP(TP,OP1);\
        MPP_DEFINE_SEMIGROUP(TP,OP2);\

    #define MPP_REQUIRE_NEAR_RING(SPEC,TP,OP1,OP2,COND)\
        MPP_GIVE_DISTRIBUTIVITY_COND(SPEC,TP,OP1,OP2,COND);\
        MPP_REQUIRE_GROUP(SPEC,TP,OP1,COND);\
        MPP_REQUIRE_SEMIGROUP(SPEC,TP,OP2,COND);\

    // pseudo-ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_pseudo_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_semigroup<Tp,Op2>::value
    struct is_pseudo_ring<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_PSEUDO_RING(TP,OP1,OP2)\
        MPP_GIVE_DISTRIBUTIVITY(TP,OP1,OP2);\
        MPP_DEFINE_ABELIAN_GROUP(TP,OP1);\
        MPP_DEFINE_SEMIGROUP(TP,OP2);\

    #define MPP_REQUIRE_PSEUDO_RING(SPEC,TP,OP1,OP2,COND)\
        MPP_GIVE_DISTRIBUTIVITY_COND(SPEC,TP,OP1,OP2,COND);\
        MPP_REQUIRE_ABELIAN_GROUP(SPEC,TP,OP1,COND);\
        MPP_REQUIRE_SEMIGROUP(SPEC,TP,OP2,COND);\

    // ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_monoid<Tp,Op2>::value
    struct is_ring<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_RING(TP,OP1,OP2)\
        MPP_GIVE_DISTRIBUTIVITY(TP,OP1,OP2);\
        MPP_DEFINE_ABELIAN_GROUP(TP,OP1);\
        MPP_DEFINE_MONOID(TP,OP2);\

    #define MPP_REQUIRE_RING(SPEC,TP,OP1,OP2,COND)\
        MPP_GIVE_DISTRIBUTIVITY_COND(SPEC,TP,OP1,OP2,COND);\
        MPP_REQUIRE_ABELIAN_GROUP(SPEC,TP,OP1,COND);\
        MPP_REQUIRE_MONOID(SPEC,TP,OP2,COND);\

    // commutative-ring structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_commutative_ring
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_commutative_monoid<Tp,Op2>::value
    struct is_commutative_ring<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_COMMUTATIVE_RING(TP,OP1,OP2)\
        MPP_GIVE_DISTRIBUTIVITY(TP,OP1,OP2);\
        MPP_DEFINE_ABELIAN_GROUP(TP,OP1);\
        MPP_DEFINE_COMMUTATIVE_MONOID(TP,OP2);\

    #define MPP_REQUIRE_COMMUTATIVE_RING(SPEC,TP,OP1,OP2,COND)\
        MPP_GIVE_DISTRIBUTIVITY_COND(SPEC,TP,OP1,OP2,COND);\
        MPP_REQUIRE_ABELIAN_GROUP(SPEC,TP,OP1,COND);\
        MPP_REQUIRE_COMMUTATIVE_MONOID(SPEC,TP,OP2,COND);\

    /* Domain-like axiom tags *********************************************** */

    // nonzero-products axiom (tristate zero-divisors)

    template <typename Tp, typename Op>
    struct has_nonzero_products
        : public std::false_type
    {};

    #define MPP_GIVE_NONZERO_PRODUCTS(TP,OP)\
        template <>\
        struct has_nonzero_products<TP,OP> : public std::true_type {};\

    #define MPP_GIVE_NONZERO_PRODUCTS_COND(SPEC,TP,OP,COND)\
        template <SPEC> requires COND\
        struct has_nonzero_products<TP,OP> : public std::true_type {};\

    // zero-divisors axiom (tristate nonzero-products)

    template <typename Tp, typename Op>
    struct has_zero_divisors
        : public std::false_type
    {};

    #define MPP_GIVE_ZERO_DIVISORS(TP,OP)\
        template <>\
        struct has_zero_divisors<TP,OP> : public std::true_type {};\

    #define MPP_GIVE_ZERO_DIVISORS_COND(SPEC,TP,OP,COND)\
        template <SPEC> requires COND\
        struct has_zero_divisors<TP,OP> : public std::true_type {};\

    // has-gcd axiom

    template <typename Tp, typename Op1, typename Op2>
    struct has_gcd
        : public std::false_type
    {};

    #define MPP_GIVE_GCD(TP,OP1,OP2)\
        template <>\
        struct has_gcd<TP,OP1,OP2> : public std::true_type {};\

    #define MPP_GIVE_GCD_COND(SPEC,TP,OP1,OP2,COND)\
        template <SPEC> requires COND\
        struct has_gcd<TP,OP1,OP2> : public std::true_type {};\

    // has-unique-factorisation axiom

    template <typename Tp, typename Op1, typename Op2>
    struct has_unique_factorisation
        : public std::false_type
    {};

    #define MPP_GIVE_UNIQUE_FACTORISATION(TP,OP1,OP2)\
        template <>\
        struct has_unique_factorisation<TP,OP1,OP2> : public std::true_type {};\

    #define MPP_GIVE_UNIQUE_FACTORISATION_COND(SPEC,TP,OP1,OP2,COND)\
        template <SPEC> requires COND\
        struct has_unique_factorisation<TP,OP1,OP2> : public std::true_type {};\

    // has-principal-ideals axiom

    template <typename Tp, typename Op1, typename Op2>
    struct has_principal_ideals
        : public std::false_type
    {};

    #define MPP_GIVE_PRINCIPAL_IDEALS(TP,OP1,OP2)\
        template <>\
        struct has_principal_ideals<TP,OP1,OP2> : public std::true_type {};\

    #define MPP_GIVE_PRINCIPAL_IDEALS_COND(SPEC,TP,OP1,OP2,COND)\
        template <SPEC> requires COND\
        struct has_principal_ideals<TP,OP1,OP2> : public std::true_type {};\

    /* Domain-like structure tags ******************************************* */

    // integral-domain structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_integral_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires is_commutative_ring<Tp,Op1,Op2>::value
            && is_not<is_trivial_ringlike<Tp,Op1,Op2>>::value
            && has_nonzero_products<Tp,Op2>::value
            && is_not<has_zero_divisors<Tp,Op2>>::value
    struct is_integral_domain<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_INTEGRAL_DOMAIN(TP,OP1,OP2)\
        MPP_DEFINE_COMMUTATIVE_RING(TP,OP1,OP2);\
        MPP_GIVE_NONZERO_PRODUCTS(TP,OP2);\

    #define MPP_REQUIRE_INTEGRAL_DOMAIN(SPEC,TP,OP1,OP2,COND)\
        MPP_REQUIRE_COMMUTATIVE_RING(SPEC,TP,OP1,OP2,COND);\
        MPP_GIVE_NONZERO_PRODUCTS_COND(SPEC,TP,OP2,COND);\

    // intargrally-closed-domain structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_integrally_closed_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires is_integral_domain<Tp,Op1,Op2>::value
    struct is_integrally_closed_domain<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_INTEGRALLY_CLOSED_DOMAIN(TP,OP1,OP2)\
        MPP_DEFINE_INTEGRAL_DOMAIN(TP,OP1,OP2);\

    #define MPP_REQUIRE_INTEGRALLY_CLOSED_DOMAIN(SPEC,TP,OP1,OP2,COND)\
        MPP_REQUIRE_INTEGRAL_DOMAIN(SPEC,TP,OP1,OP2,COND);\

    // gcd-domain structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_gcd_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires is_integral_domain<Tp,Op1,Op2>::value
            && has_gcd<Tp,Op1,Op2>::value
    struct is_gcd_domain<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_GCD_DOMAIN(TP,OP1,OP2)\
        MPP_DEFINE_INTEGRALLY_CLOSED_DOMAIN(TP,OP1,OP2);\
        MPP_GIVE_GCD(TP,OP1,OP2);\

    #define MPP_REQUIRE_GCD_DOMAIN(SPEC,TP,OP1,OP2,COND)\
        MPP_REQUIRE_INTEGRALLY_CLOSED_DOMAIN(SPEC,TP,OP1,OP2,COND);\
        MPP_GIVE_GCD_COND(SPEC,TP,OP1,OP2,COND);\

    // unique-factorisation-domain structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_unique_factorisation_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires is_gcd_domain<Tp,Op1,Op2>::value
            && has_unique_factorisation<Tp,Op1,Op2>::value
    struct is_unique_factorisation_domain<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_UNIQUE_FACTORISATION_DOMAIN(TP,OP1,OP2)\
        MPP_DEFINE_GCD_DOMAIN(TP,OP1,OP2);\
        MPP_GIVE_UNIQUE_FACTORISATION(TP,OP1,OP2);\

    #define MPP_REQUIRE_UNIQUE_FACTORISATION_DOMAIN(SPEC,TP,OP1,OP2,COND)\
        MPP_REQUIRE_GCD_DOMAIN(SPEC,TP,OP1,OP2,COND);\
        MPP_GIVE_UNIQUE_FACTORISATION_COND(SPEC,TP,OP1,OP2,COND);\

    // principal-ideal-domain structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_principal_ideal_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires is_unique_factorisation_domain<Tp,Op1,Op2>::value
            && has_principal_ideals<Tp,Op1,Op2>::value
    struct is_principal_ideal_domain<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_PRINCIPAL_IDEAL_DOMAIN(TP,OP1,OP2)\
        MPP_DEFINE_UNIQUE_FACTORISATION_DOMAIN(TP,OP1,OP2);\
        MPP_GIVE_PRINCIPAL_IDEALS(TP,OP1,OP2);\

    #define MPP_REQUIRE_PRINCIPAL_IDEAL_DOMAIN(SPEC,TP,OP1,OP2,COND)\
        MPP_REQUIRE_UNIQUE_FACTORISATION_DOMAIN(SPEC,TP,OP1,OP2,COND);\
        MPP_GIVE_PRINCIPAL_IDEALS_COND(SPEC,TP,OP1,OP2,COND);\

    // euclidean-domain structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_euclidean_domain
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires is_principal_ideal_domain<Tp,Op1,Op2>::value
    struct is_euclidean_domain<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_EUCLIDEAN_DOMAIN(TP,OP1,OP2)\
        MPP_DEFINE_PRINCIPAL_IDEAL_DOMAIN(TP,OP1,OP2);\

    #define MPP_REQUIRE_EUCLIDEAN_DOMAIN(SPEC,TP,OP1,OP2,COND)\
        MPP_REQUIRE_PRINCIPAL_IDEAL_DOMAIN(SPEC,TP,OP1,OP2,COND);\

    /* Field-like structure tags ******************************************** */

    // skew-field structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_skew_field
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_group<Tp,Op2>::value
    struct is_skew_field<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_SKEW_FIELD(TP,OP1,OP2)\
        MPP_GIVE_DISTRIBUTIVITY(TP,OP1,OP2);\
        MPP_DEFINE_ABELIAN_GROUP(TP,OP1);\
        MPP_DEFINE_GROUP(TP,OP2);\

    #define MPP_REQUIRE_SKEW_FIELD(SPEC,TP,OP1,OP2,COND)\
        MPP_GIVE_DISTRIBUTIVITY_COND(SPEC,TP,OP1,OP2,COND);\
        MPP_REQUIRE_ABELIAN_GROUP(SPEC,TP,OP1,COND);\
        MPP_REQUIRE_GROUP(SPEC,TP,OP2,COND);\

    // field structure

    template <typename Tp, typename Op1, typename Op2>
    struct is_field
        : public std::false_type
    {};

    template <typename Tp, typename Op1, typename Op2>
        requires has_distributivity<Tp,Op1,Op2>::value
            && is_abelian_group<Tp,Op1>::value
            && is_abelian_group<Tp,Op2>::value
    struct is_field<Tp,Op1,Op2>
        : public std::true_type
    {};

    #define MPP_DEFINE_FIELD(TP,OP1,OP2)\
        MPP_GIVE_DISTRIBUTIVITY(TP,OP1,OP2);\
        MPP_DEFINE_ABELIAN_GROUP(TP,OP1);\
        MPP_DEFINE_ABELIAN_GROUP(TP,OP2);\
        /* MPP_DEFINE_EUCLIDEAN_DOMAIN(TP,OP1,OP2); */ \
        MPP_GIVE_PRINCIPAL_IDEALS(TP,OP1,OP2);\
        MPP_GIVE_UNIQUE_FACTORISATION(TP,OP1,OP2);\
        MPP_GIVE_GCD(TP,OP1,OP2);\
        MPP_GIVE_NONZERO_PRODUCTS(TP,OP2);\

    #define MPP_REQUIRE_FIELD(SPEC,TP,OP1,OP2,COND)\
        MPP_GIVE_DISTRIBUTIVITY_COND(SPEC,TP,OP1,OP2,COND);\
        MPP_REQUIRE_ABELIAN_GROUP(SPEC,TP,OP1,COND);\
        MPP_REQUIRE_ABELIAN_GROUP(SPEC,TP,OP2,COND);\
        /* MPP_REQUIRE_EUCLIDEAN_DOMAIN(SPEC,TP,OP1,OP2,COND); */ \
        MPP_GIVE_PRINCIPAL_IDEALS_COND(SPEC,TP,OP1,OP2,COND);\
        MPP_GIVE_UNIQUE_FACTORISATION_COND(SPEC,TP,OP1,OP2,COND);\
        MPP_GIVE_GCD_COND(SPEC,TP,OP1,OP2,COND);\
        MPP_GIVE_NONZERO_PRODUCTS_COND(SPEC,TP,OP2,COND);\

    // algebraically-closed-field structure

    // template <typename Tp, typename Op1, typename Op2>
    // struct is_algebraically_closed_domain
    //     : public std::false_type
    // {};

} // namespace mpp

/* ************************************************************************** */
// Helper Specialization
/* ************************************************************************** */

namespace mpp
{

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
        static Tp& make(Tp& e) { return e = -e; }
    };

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
            && has_invertibility<Tp,op_mul>::value
    struct inverse<Tp,op_mul>
    {
        static Tp get(Tp e) { return 1/e; }
        static Tp& make(Tp& e) { return e = 1/e; }
    };

    template <typename Tp, typename Op>
        requires std::is_arithmetic<Tp>::value
    struct absolute<Tp,Op>
    {
        static Tp get(Tp e) { return (e < mpp::identity<Tp,Op>::get()) ? mpp::inverse<Tp,Op>::get(e) : e; }
        static Tp& make(Tp& e) { return e = get(e); }
    };

} // namespace mpp

/* ************************************************************************** */
// Tag Specialization for Primitive Types
/* ************************************************************************** */

namespace mpp
{
    MPP_REQUIRE_FIELD(typename Tp,Tp,op_add,op_mul,std::is_floating_point<Tp>::value);
    MPP_REQUIRE_GCD_DOMAIN(typename Tp,Tp,op_add,op_mul,std::is_integral<Tp>::value);

} // namespace mpp

#endif /* __HH_MPP_GROUP_TRAITS */
