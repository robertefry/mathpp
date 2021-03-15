
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
    Tp rn0 = a, rn1 = b;

    while (rn1 != mpp::identity<Tp,mpp::op_add>::get())
    {
        rn0 %= rn1;
        std::swap(rn0,rn1);
    }
    return mpp::absolute<Tp,mpp::op_add>::get(rn0);
}

#endif /* __HH_MPP_GCD */
