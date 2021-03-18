
#ifndef __HH_MPP_GCD
#define __HH_MPP_GCD

#include "mathpp/mathpp.hh"

#include <tuple>
#include <utility>

/* ************************************************************************** */
// Definitions
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp>
    Tp gcd(Tp const& a, Tp const& b);

    template <typename Tp>
    std::tuple<Tp,Tp> gcd_extended(Tp const& a, Tp const& b);

} // namespace mpp

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

template <typename Tp>
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

template <typename Tp>
std::tuple<Tp,Tp> mpp::gcd_extended(Tp const& a, Tp const& b)
{
    if (a < b)
    {
        std::tuple<Tp,Tp> result = gcd_extended(b,a);
        std::swap(std::get<0>(result),std::get<1>(result));
        return result;
    }

    using mpp::op_add;  using mpp::op_mul;
    std::tuple<Tp,Tp> sn[] = {
        { mpp::identity<Tp,op_mul>::get(), mpp::identity<Tp,op_add>::get() },
        { mpp::identity<Tp,op_add>::get(), mpp::identity<Tp,op_mul>::get() },
    };
    Tp rn[] = {a,b};

    while (rn[1] != mpp::identity<Tp,op_add>::get())
    {
        Tp const qm = rn[0] / rn[1];
        sn[0] = {
            std::get<0>(sn[0]) - std::get<0>(sn[1]) * qm,
            std::get<1>(sn[0]) - std::get<1>(sn[1]) * qm,
        };
        std::swap(sn[0],sn[1]);

        rn[0] %= rn[1];
        std::swap(rn[0],rn[1]);
    }
    if (rn[0] < mpp::identity<Tp,op_add>::get())
    {
        mpp::inverse<Tp,op_add>::make(std::get<0>(sn[0]));
        mpp::inverse<Tp,op_add>::make(std::get<1>(sn[0]));
        std::swap(std::get<0>(sn[0]),std::get<1>(sn[0]));
    }
    return sn[0];
}

#endif /* __HH_MPP_GCD */
