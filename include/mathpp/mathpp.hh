
#ifndef __HH_MPP_MATHPP
#define __HH_MPP_MATHPP

#include <type_traits>
#include <tuple>
#include <cmath>

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
    struct identity
    {
        constexpr static bool has() { return false; }
    //     static Tp get();
    //     static Tp& make(Tp&);
    };

    template <typename Tp, typename Op>
    struct inverse
    {
        constexpr static bool has(Tp const&) { return false; }
    //     static Tp get(Tp const&);
    //     static Tp& make(Tp&);
    };

    template <typename Tp, typename Op>
    struct absolute
    {
        constexpr static bool has(Tp const&) { return false; }
    //     static Tp get(Tp const&);
    //     static Tp& make(Tp&);
    };

    template <typename Tp>
    struct modulo
    {
        constexpr static bool has(Tp const&) { return false; }
    //     static Tp get(Tp const&);
    //     static Tp& make(Tp&);
    };

    template <typename Tp, typename Tq>
    struct division;
    // {
    //     static auto get(Tp const&, Tq const&);
    // };

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
        constexpr static bool has(Tp e) { return true; }
        static Tp get() { return Tp{0}; }
        static Tp& make(Tp& e) { return e = get(); }
    };

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct identity<Tp,op_mul>
    {
        constexpr static bool has(Tp e) { return true; }
        static Tp get() { return Tp{1}; }
        static Tp& make(Tp& e) { return e = get(); }
    };

    // inverse

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct inverse<Tp,op_add>
    {
        constexpr static bool has(Tp e) { return true; }
        static Tp get(Tp e) { return -e; }
        static Tp& make(Tp& e) { return e = -e; }
    };

    template <typename Tp>
        requires std::is_floating_point<Tp>::value
    struct inverse<Tp,op_mul>
    {
        constexpr static bool has(Tp e) { return true; }
        static Tp get(Tp e) { return 1/e; }
        static Tp& make(Tp& e) { return e = 1/e; }
    };

    // absolute

    template <typename Tp, typename Op>
        requires std::is_arithmetic<Tp>::value
    struct absolute<Tp,Op>
    {
        constexpr static bool has(Tp e)
        {
            return true;
        }
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

    // modulo

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct modulo<Tp>
    {
        constexpr static bool has(Tp e)
        {
            return true;
        }
        static Tp get(Tp const& e, Tp n)
        {
            return (e < 0) ? std::fmod(-e,n) : std::fmod(e,n);
        }
        static Tp& make(Tp& e, Tp n)
        {
            return e = get(e,n);
        }
    };

    // division

    template <typename Tp, typename Tq>
        requires std::is_arithmetic<Tp>::value
            && std::is_arithmetic<Tp>::value
    struct division<Tp,Tq>
    {
        static auto get(Tp const& dividend, Tq const& divisor)
        {
            auto quotient = std::floor(dividend/divisor);
            auto remainder = dividend - quotient * divisor;
            return std::make_tuple(quotient,remainder);
        }
    };

} // namespace mpp

#endif /* __HH_MPP_MATHPP */
