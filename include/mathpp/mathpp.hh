
#ifndef __HH_MPP_MATHPP
#define __HH_MPP_MATHPP

#include <type_traits>
#include <tuple>

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
// Math Helper Definitions
/* ************************************************************************** */

namespace mpp
{

    /*
     * Math Helpers should have both a `get` and `make` function,
     * unless nonsensical.
     */

    template <typename Tp, typename Op>
    struct identity;
    // {
    //     static Tp get();
    //     static Tp& make(Tp&);
    // };

    template <typename Tp, typename Op>
    struct inverse;
    // {
    //     static Tp get(Tp const&);
    //     static Tp& make(Tp&);
    // };

    template <typename Tp, typename Op>
    struct absolute;
    // {
    //     static Tp get(Tp const&);
    //     static Tp& make(Tp&);
    // };

    template <typename Tp, typename Tq>
    struct division;
    // {
    //     static auto get(Tp const&, Tq const&);
    // };

} // namespace mpp

/* ************************************************************************** */
// Math Helper Default Implementation
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp, typename Op>
    struct absolute
    {
        static Tp get(Tp e)
        {
            return (e < mpp::identity<Tp,Op>::get()) ? mpp::inverse<Tp,Op>::get(e) : e;
        }
        static Tp& make(Tp& e)
        {
            if (e < mpp::identity<Tp,Op>::get()) e = mpp::inverse<Tp,Op>::get(e);
            return e;
        }
    };

    template <typename Tp, typename Tq>
    struct division
    {
        static auto get(Tp const& dividend, Tp const& divisor)
        {
            return std::make_tuple(dividend/divisor,dividend%divisor);
        }
    };

} // namespace mpp

/* ************************************************************************** */
// Math Helper Specialization
/* ************************************************************************** */

namespace mpp
{

    // identity

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct identity<Tp,op_add>
    {
        static Tp get() { return Tp{0}; }
        static Tp& make(Tp& e) { return e = get(); }
    };

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct identity<Tp,op_mul>
    {
        static Tp get() { return Tp{1}; }
        static Tp& make(Tp& e) { return e = get(); }
    };

    // inverse

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct inverse<Tp,op_add>
    {
        static Tp get(Tp e) { return -e; }
        static Tp& make(Tp& e) { return e = -e; }
    };

    template <typename Tp>
        requires std::is_floating_point<Tp>::value
    struct inverse<Tp,op_mul>
    {
        static Tp get(Tp e) { return 1/e; }
        static Tp& make(Tp& e) { return e = 1/e; }
    };

} // namespace mpp

#endif /* __HH_MPP_MATHPP */
