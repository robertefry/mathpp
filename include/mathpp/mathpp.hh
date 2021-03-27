
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
     * All helpers should have the following functions, where appropriate.
     *  `has`   - Determines if possible for the template parameters.
     *  `can`   - Determines if possible for an argument of template type.
     *  `get`   - Returns the property.
     *  `make`  - Sets an argument to the property.
     */

    template <typename Tp, typename Op>
    struct identity;

    template <typename Tp, typename Op>
    struct inverse;

    template <typename Tp, typename Op>
    struct absolute;

    template <typename Tp, typename Tq>
    struct modulo;

    template <typename Tp, typename Tq>
    struct division;

} // namespace mpp

/* ************************************************************************** */
// Math Helper Defaults
/* ************************************************************************** */

namespace mpp
{

    // identity

    template <typename Tp, typename Op>
    struct identity
    {
        constexpr static bool has()
        {
            return false;
        }
    };

    // inverse

    template <typename Tp, typename Op>
    struct inverse
    {
        constexpr static bool has()
        {
            return false;
        }
        constexpr static bool can(Tp const&)
        {
            return has();
        }
    };

    // absolute

    template <typename Tp, typename Op>
    struct absolute
    {
        constexpr static bool has()
        {
            return mpp::identity<Tp,Op>::has() && mpp::inverse<Tp,Op>::has();
        }
        constexpr static bool can(Tp const&)
        {
            return has();
        }
        static Tp get(Tp e)
        {
            if (e < mpp::identity<Tp,Op>::get())
            {
                return mpp::inverse<Tp,Op>::get(e);
            }
            return e;
        }
        static Tp& make(Tp& e)
        {
            if (e < mpp::identity<Tp,Op>::get())
            {
                return mpp::inverse<Tp,Op>::make(e);
            }
            return e;
        }
    };

    // modulo

    template <typename Tp, typename Tq>
    struct modulo
    {
        constexpr static bool has()
        {
            return false;
        }
        constexpr static bool can(Tp const&, Tq const&)
        {
            return false;
        }
    };

    // division

    template <typename Tp, typename Tq>
    struct division
    {
        constexpr static bool has()
        {
            return true;
        }
        constexpr static bool can(Tp const&, Tq const&)
        {
            return has();
        }
        static auto get(Tp const& dividend, Tq const& divisor)
        {
            auto quotient = dividend / divisor;
            auto remainder = dividend - quotient * divisor;
            return std::make_tuple(quotient,remainder);
        }
    };

} // namespace mpp

/* ************************************************************************** */
// Math Helper Specialization for Arithmetic Types
/* ************************************************************************** */

namespace mpp
{

    // identity

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct identity<Tp,op_add>
    {
        constexpr static bool has()
        {
            return true;
        }
        static Tp get()
        {
            return Tp{0};
        }
        static Tp& make(Tp& e)
        {
            return e = get();
        }
    };

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct identity<Tp,op_mul>
    {
        constexpr static bool has()
        {
            return true;
        }
        static Tp get()
        {
            return Tp{1};
        }
        static Tp& make(Tp& e)
        {
            return e = get();
        }
    };

    // inverse

    template <typename Tp>
        requires std::is_signed<Tp>::value
    struct inverse<Tp,op_add>
    {
        constexpr static bool has()
        {
            return true;
        }
        constexpr static bool can(Tp)
        {
            return has();
        }
        static Tp get(Tp e)
        {
            return -e;
        }
        static Tp& make(Tp& e)
        {
            return e = get(e);
        }
    };

    template <typename Tp>
        requires std::is_floating_point<Tp>::value
    struct inverse<Tp,op_mul>
    {
        constexpr static bool has()
        {
            return true;
        }
        constexpr static bool can(Tp)
        {
            return has();
        }
        static Tp get(Tp e)
        {
            return 1/e;
        }
        static Tp& make(Tp& e)
        {
            return e = get(e);
        }
    };

    // modulo

    template <typename Tp, typename Tq>
        requires std::is_arithmetic<Tp>::value
            && std::is_arithmetic<Tq>::value
    struct modulo<Tp,Tq>
    {
        constexpr static bool has()
        {
            return true;
        }
        constexpr static bool can(Tp, Tq)
        {
            return has();
        }
        static Tp get(Tp e, Tq n)
        {
            return (e < 0) ? n - std::fmod(-e,n) : std::fmod(e,n);
        }
        static Tp& make(Tp& e, Tq n)
        {
            return e = get(e,n);
        }
    };

} // namespace mpp

#endif /* __HH_MPP_MATHPP */
