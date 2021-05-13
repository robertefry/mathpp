
#ifndef __HH_MPP_MATHPP
#define __HH_MPP_MATHPP

#include "mathpp/logic.hh"

#include <type_traits>
#include <concepts>
#include <tuple>
#include <cmath>

/* ************************************************************************** */
// Operation Definitions
/* ************************************************************************** */

namespace mpp
{

    struct operation
    {};

    struct op_add : public operation
    {
        template <typename Tp, typename Tq>
            requires requires (Tp a, Tq b) { a + b; }
        static auto operate(Tp const& a, Tp const& b) { return a + b; }

        template <typename Tp, typename Tq>
            requires requires (Tp a, Tq b) { a + b; }
        struct result {
            using type = std::result_of<decltype(operate<Tp,Tq>)&(Tp,Tq)>::type;
        };
    };

    struct op_mul : public operation
    {
        template <typename Tp, typename Tq>
            requires requires (Tp a, Tq b) { a * b; }
        static auto operate(Tp const& a, Tp const& b) { return a * b; }

        template <typename Tp, typename Tq>
            requires requires (Tp a, Tq b) { a * b; }
        struct result {
            using type = std::result_of<decltype(operate<Tp,Tq>)&(Tp,Tq)>::type;
        };
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
     *  `get`   - Returns the property, if & when appropriate, and throws.
     *  `make`  - Sets to the property, if & when appropriate, and throws.
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
            return logic::none;
        }
    };

    // inverse

    template <typename Tp, typename Op>
    struct inverse
    {
        constexpr static tristate has()
        {
            return logic::none;
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
            return logic::none;
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
            return logic::none;
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
            return logic::none;
        }
        constexpr static bool can(Tp const&, Tq const&)
        {
            return false;
        }
    };

} // namespace mpp

/* ************************************************************************** */
// Math Helper Specialization for Algorithmic Calculation
/* ************************************************************************** */

namespace mpp
{

    // absolute

    template <typename Tp, typename Op>
        requires std::totally_ordered<Tp>
    struct absolute<Tp,Op>
    {
        constexpr static tristate has()
        {
            if constexpr (inverse<Tp,Op>::has() != logic::none)
            {
                return identity<Tp,Op>::has();
            }
            return logic::all;
        }
        constexpr static bool can(Tp const&)
        {
            return has() != logic::none;
        }
        constexpr static Tp get(Tp const& e)
        {
            if constexpr (inverse<Tp,Op>::has() != logic::none) {
                if (e < identity<Tp,Op>::get()) {
                    return inverse<Tp,Op>::get(e);
                }
            }
            return e;
        }
        constexpr static Tp& make(Tp& e)
        {
            if constexpr (inverse<Tp,Op>::has() != logic::none) {
                if (e < identity<Tp,Op>::get()) {
                    return inverse<Tp,Op>::make(e);
                }
            }
            return e;
        }
    };

    // modulo

    template <typename Tp, typename Tq>
        requires requires (Tp e, Tq n) { e % n; }
    struct modulo<Tp,Tq>
    {
        constexpr static tristate has()
        {
            return logic::all;
        }
        constexpr static bool can(Tp const&, Tq const&)
        {
            return true;
        }
        constexpr static auto get(Tp const& e, Tq const& n)
        {
            auto const ident = identity<Tp,op_add>::get();
            return (e < ident) ? n - (-e % n) : (e % n);
        }
        constexpr static auto& make(Tp& e, Tq const& n)
        {
            return e = get(e,n);
        }
    };

    // division

    template <typename Tp, typename Tq>
        requires requires (Tp x, Tq y) { x - (x / y) * y; }
    struct division<Tp,Tq>
    {
        constexpr static tristate has()
        {
            return logic::all;
        }
        constexpr static bool can(Tp const&, Tq const&)
        {
            return true;
        }
        constexpr static auto get(Tp const& dividend, Tq const& divisor)
        {
            auto s = dividend / divisor;
            auto r = dividend - s * divisor;
            return std::make_tuple(r,s);
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
            return logic::all;
        }
        constexpr static Tp get()
        {
            return Tp{0};
        }
        constexpr static Tp& make(Tp& e)
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
            return logic::all;
        }
        constexpr static Tp get()
        {
            return Tp{1};
        }
        constexpr static Tp& make(Tp& e)
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
            return logic::all;
        }
        constexpr static bool can(Tp const&)
        {
            return true;
        }
        constexpr static Tp get(Tp e)
        {
            return -e;
        }
        constexpr static Tp& make(Tp& e)
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
            return logic::all;
        }
        constexpr static bool can(Tp const&)
        {
            return true;
        }
        constexpr static Tp get(Tp e)
        {
            return 1/e;
        }
        constexpr static Tp& make(Tp& e)
        {
            return e = get(e);
        }
    };

    // modulo

    template <typename Tp, typename Tq>
        requires std::is_floating_point<Tp>::value
            || std::is_floating_point<Tq>::value
    struct modulo<Tp,Tq>
    {
        constexpr static tristate has()
        {
            return logic::all;
        }
        constexpr static bool can(Tp const&, Tq const&)
        {
            return true;
        }
        constexpr static Tp get(Tp e, Tq n)
        {
            return (e < 0) ? n - std::fmod(-e,n) : std::fmod(e,n);
        }
        constexpr static Tp& make(Tp& e, Tq n)
        {
            return e = get(e,n);
        }
    };

} // namespace mpp

#endif /* __HH_MPP_MATHPP */
