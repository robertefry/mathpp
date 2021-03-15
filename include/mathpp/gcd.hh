
#ifndef __HH_MPP_GCD
#define __HH_MPP_GCD

#include "group_traits.hh"

/* ************************************************************************** */
// Definitions
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp>
        requires is_gcd_domain<Tp,op_add,op_mul>::value
    Tp gcd(Tp const& a, Tp const& b);

} // namespace mpp

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

template <typename Tp>
    requires mpp::is_gcd_domain<Tp,mpp::op_add,mpp::op_mul>::value
Tp mpp::gcd(Tp const& a, Tp const& b)
{
    if (a < b) return gcd(b,a);

    using mpp::op_add;  using mpp::op_mul;
    Tp rn[] = {a,b};

    while (rn[1] != mpp::identity<Tp,op_add>::get())
    {
        rn[0] %= rn[1];
        std::swap(rn[0],rn[1]);
    }
    return mpp::absolute<Tp,op_add>::make(rn[0]);
}

#endif /* __HH_MPP_GCD */
