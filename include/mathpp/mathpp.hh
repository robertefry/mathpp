
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
// Logic Definitions
/* ************************************************************************** */

namespace mpp
{

    enum struct tristate
    {
        all, some, none
    };

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
        constexpr static tristate has()
        {
            return tristate::none;
        }
    };

    // inverse

    template <typename Tp, typename Op>
    struct inverse
    {
        constexpr static tristate has()
        {
            return tristate::none;
        }
        constexpr static bool can(Tp const&)
        {
            return false;
        }
    };

    // absolute

    template <typename Tp, typename Op>
    struct absolute
    {
        constexpr static tristate has()
        {
            return tristate::none;
        }
        constexpr static bool can(Tp const&)
        {
            return false;
        }
    };

    // modulo

    template <typename Tp, typename Tq>
    struct modulo
    {
        constexpr static tristate has()
        {
            return tristate::none;
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
        constexpr static tristate has()
        {
            return tristate::none;
        }
        constexpr static bool can(Tp const&, Tq const&)
        {
            return false;
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
        constexpr static tristate has()
        {
            return tristate::all;
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
        constexpr static tristate has()
        {
            return tristate::all;
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
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(Tp const&)
        {
            return true;
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
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(Tp const&)
        {
            return true;
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

    // absolute

    template <typename Tp>
        requires std::is_arithmetic<Tp>::value
    struct absolute<Tp,op_add>
    {
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(Tp const&)
        {
            return true;
        }
        static Tp get(Tp e)
        {
            return (e < 0) ? -e : e;
        }
        static Tp& make(Tp& e)
        {
            return (e < 0) ? e *= -1 : e;
        }
    };

    template <typename Tp>
        requires std::is_floating_point<Tp>::value
    struct absolute<Tp,op_mul>
    {
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(Tp const&)
        {
            return true;
        }
        static Tp get(Tp e)
        {
            return (absolute<Tp,op_add>::get(e) < 1) ? 1/e : e;
        }
        static Tp& make(Tp& e)
        {
            return (absolute<Tp,op_add>::get(e) < 1) ? e = 1/e : e;
        }
    };

    // modulo

    template <typename Tp, typename Tq>
        requires std::is_arithmetic<Tp>::value
            && std::is_arithmetic<Tq>::value
    struct modulo<Tp,Tq>
    {
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(Tp const&, Tq const&)
        {
            return true;
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

    // division

    template <typename Tp, typename Tq>
        requires std::is_arithmetic<Tp>::value
            && std::is_arithmetic<Tq>::value
    struct division<Tp,Tq>
    {
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(Tp const&, Tq const&)
        {
            return true;
        }
        static auto get(Tp const& dividend, Tq const& divisor)
        {
            auto s = dividend / divisor;
            auto r = dividend - s * divisor;
            return std::make_tuple(r,s);
        }
    };

} // namespace mpp

#endif /* __HH_MPP_MATHPP */
