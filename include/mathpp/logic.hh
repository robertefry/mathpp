
#ifndef __HH_MPP_LOGIC
#define __HH_MPP_LOGIC

#include <cstddef>
#include <compare>

/* ************************************************************************** */
// Definitions
/* ************************************************************************** */

namespace mpp
{

    class tristate
    {
    public:
        enum struct type { none, some, all };
        constexpr tristate(type v) : value{v} {}

    private:
        type value;

    public:
        friend inline constexpr bool operator==(tristate, tristate);
        friend inline constexpr auto operator<=>(tristate, tristate) -> std::strong_ordering;

        inline constexpr tristate& operator&=(tristate b) { return *this = (*this & b); }
        inline constexpr tristate& operator|=(tristate b) { return *this = (*this | b); }
        inline constexpr tristate& operator^=(tristate b) { return *this = (*this ^ b); }
        inline constexpr tristate& operator<<=(size_t b) { return *this = (*this << b); }
        inline constexpr tristate& operator>>=(size_t b) { return *this = (*this >> b); }

        friend inline constexpr tristate operator~(tristate);
        friend inline constexpr tristate operator&(tristate, tristate);
        friend inline constexpr tristate operator|(tristate, tristate);
        friend inline constexpr tristate operator^(tristate, tristate);
        friend inline constexpr tristate operator<<(tristate, size_t);
        friend inline constexpr tristate operator>>(tristate, size_t);
    };

    namespace logic
    {

        inline constexpr static tristate none = tristate::type::none;
        inline constexpr static tristate some = tristate::type::some;
        inline constexpr static tristate all  = tristate::type::all;

    } // namespace logic

} // namespace mpp

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

namespace mpp
{

    inline constexpr bool operator==(tristate a, tristate b)
    {
        return a.value == b.value;
    }

    inline constexpr auto operator<=>(tristate a, tristate b) -> std::strong_ordering
    {
        return a.value <=> b.value;
    }

    inline constexpr tristate operator~(tristate a)
    {
        switch (a.value)
        {
            case tristate::type::all:   return tristate::type::none;
            case tristate::type::some:  return tristate::type::some;
            case tristate::type::none:  return tristate::type::all;
            default: return tristate::type::some;
        }
    }

    inline constexpr tristate operator&(tristate a, tristate b)
    {
        return (a.value < b.value) ? a : b;
    }

    inline constexpr tristate operator|(tristate a, tristate b)
    {
        return (a.value > b.value) ? a : b;
    }

    inline constexpr tristate operator^(tristate a, tristate b)
    {
        return (a & ~b) | (b & ~a);
    }

    inline constexpr tristate operator<<(tristate a, size_t n)
    {
        switch (n)
        {
            case 0: return a;
            case 1: switch (a.value)
            {
                case tristate::type::all:   return tristate::type::all;
                case tristate::type::some:  return tristate::type::all;
                case tristate::type::none:  return tristate::type::some;
                default: return tristate::type::some;
            }
            default: return tristate::type::all;
        }
    }

    inline constexpr tristate operator>>(tristate a, size_t n)
    {
        switch (n)
        {
            case 0: return a;
            case 1: switch (a.value)
            {
                case tristate::type::all:   return tristate::type::some;
                case tristate::type::some:  return tristate::type::none;
                case tristate::type::none:  return tristate::type::none;
                default: return tristate::type::some;
            }
            default: return tristate::type::none;
        }
    }

    inline constexpr tristate operator!(tristate a)
    {
        return ~a;
    }

    inline constexpr tristate operator&&(tristate a, tristate b)
    {
        return a & b;
    }

    inline constexpr tristate operator||(tristate a, tristate b)
    {
        return a | b;
    }

} // namespace mpp

#endif /* __HH_MPP_LOGIC */