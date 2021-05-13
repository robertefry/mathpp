
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
    constexpr Tp gcd(Tp const& a, Tp const& b);

    template <typename Tp>
    constexpr std::tuple<Tp,Tp> gcd_extended(Tp const& a, Tp const& b);

} // namespace mpp

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp>
    constexpr Tp gcd(Tp const& a, Tp const& b)
    {
        if (a < b) return gcd(b,a);

        Tp rn[] = {a,b};

        while (rn[1] != identity<Tp,op_add>::get())
        {
            modulo<Tp,Tp>::make(rn[0],rn[1]);
            std::swap(rn[0],rn[1]);
        }
        return absolute<Tp,op_add>::make(rn[0]);
    }

    template <typename Tp>
    constexpr std::tuple<Tp,Tp> gcd_extended(Tp const& a, Tp const& b)
    {
        if (a < b)
        {
            std::tuple<Tp,Tp> result = gcd_extended(b,a);
            std::swap(std::get<0>(result),std::get<1>(result));
            return result;
        }

        std::tuple<Tp,Tp> sn[] = {
            { identity<Tp,op_mul>::get(), identity<Tp,op_add>::get() },
            { identity<Tp,op_add>::get(), identity<Tp,op_mul>::get() },
        };
        Tp rn[] = {a,b};

        while (rn[1] != identity<Tp,op_add>::get())
        {
            Tp const qm = std::get<1>(division<Tp,Tp>::get(rn[0],rn[1]));
            sn[0] = {
                std::get<0>(sn[0]) - std::get<0>(sn[1]) * qm,
                std::get<1>(sn[0]) - std::get<1>(sn[1]) * qm,
            };
            std::swap(sn[0],sn[1]);

            modulo<Tp,Tp>::make(rn[0],rn[1]);
            std::swap(rn[0],rn[1]);
        }
        if (rn[0] < identity<Tp,op_add>::get())
        {
            inverse<Tp,op_add>::make(std::get<0>(sn[0]));
            inverse<Tp,op_add>::make(std::get<1>(sn[0]));
            std::swap(std::get<0>(sn[0]),std::get<1>(sn[0]));
        }
        return sn[0];
    }

} // namespace mpp

#endif /* __HH_MPP_GCD */
