
#ifndef __HH_MPP_POLYCONV
#define __HH_MPP_POLYCONV

#include "mathpp/mathpp.hh"
#include "mathpp/poly.hh"

#include <algorithm>

/* ************************************************************************** */
// Definitions
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp, size_t Nm>
        requires (Nm >= 1)
    class PolyConv
        : public mpp::Poly<Tp>
    {
    public:
        PolyConv();
        PolyConv(size_t, Tp const&);
        PolyConv(std::vector<Tp> const&);
        PolyConv(std::vector<Tp>&&);
        PolyConv(std::initializer_list<Tp>);
        virtual ~PolyConv() = default;

        template <typename Tq, size_t Nn>
        PolyConv(PolyConv<Tq,Nn> const&);

    protected:
        virtual void validate() override;

    public:
        virtual auto order() const -> size_t override;
        virtual auto size() const -> size_t override;

        virtual auto operator[](size_t i) const -> Tp const& override;
        virtual auto operator[](size_t i) -> Tp& override;
        virtual auto at(size_t i) const -> Tp const& override;
        virtual auto at(size_t i) -> Tp& override;

    };

} // namespace mpp

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

template <typename Tp, size_t Nm>
mpp::PolyConv<Tp,Nm>::PolyConv()
    : mpp::Poly<Tp>(Nm,mpp::identity<Tp,op_add>::get())
{
}

template <typename Tp, size_t Nm>
mpp::PolyConv<Tp,Nm>::PolyConv(size_t n, Tp const& val)
    : mpp::Poly<Tp>(n,val)
{
    validate();
}

template <typename Tp, size_t Nm>
mpp::PolyConv<Tp,Nm>::PolyConv(std::vector<Tp> const& coeffs)
    : mpp::Poly<Tp>{coeffs}
{
    validate();
}

template <typename Tp, size_t Nm>
mpp::PolyConv<Tp,Nm>::PolyConv(std::vector<Tp>&& coeffs)
    : mpp::Poly<Tp>{coeffs}
{
    validate();
}

template <typename Tp, size_t Nm>
mpp::PolyConv<Tp,Nm>::PolyConv(std::initializer_list<Tp> coeffs)
    : mpp::Poly<Tp>{coeffs}
{
    validate();
}

template <typename Tp, size_t Nm>
template <typename Tq, size_t Nn>
mpp::PolyConv<Tp,Nm>::PolyConv(PolyConv<Tq,Nn> const& other)
    : mpp::Poly<Tp>{other}
{
    validate();
}

template <typename Tp, size_t Nm>
void mpp::PolyConv<Tp,Nm>::validate()
{
    if (this->m_Coefficients.size() > Nm)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            for (size_t j = Nm + i; j < this->m_Coefficients.size(); j += Nm)
            {
                this->m_Coefficients[i] += this->m_Coefficients[j];
            }
        }
    }
    this->m_Coefficients.resize(Nm,mpp::identity<Tp,op_add>::get());
}

template <typename Tp, size_t Nm>
auto mpp::PolyConv<Tp,Nm>::order() const
    -> size_t
{
    auto const pred = [](auto const& coeff) { return coeff == mpp::identity<Tp,op_add>::get(); };
    auto const itr = std::find_if(this->m_Coefficients.begin(), this->m_Coefficients.end(), pred);
    auto dist = std::distance(this->m_Coefficients.begin(),itr);
    return (dist == 0) ? 0 : dist-1;
}

template <typename Tp, size_t Nm>
auto mpp::PolyConv<Tp,Nm>::size() const
    -> size_t
{
    return Nm;
}

template <typename Tp, size_t Nm>
auto mpp::PolyConv<Tp,Nm>::operator[](size_t i) const
    -> Tp const&
{
    return this->m_Coefficients[i%Nm];
}

template <typename Tp, size_t Nm>
auto mpp::PolyConv<Tp,Nm>::operator[](size_t i)
    -> Tp&
{
    return this->m_Coefficients[i%Nm];
}

template <typename Tp, size_t Nm>
auto mpp::PolyConv<Tp,Nm>::at(size_t i) const
    -> Tp const&
{
    return this->m_Coefficients[i%Nm];
}

template <typename Tp, size_t Nm>
auto mpp::PolyConv<Tp,Nm>::at(size_t i)
    -> Tp&
{
    return this->m_Coefficients[i%Nm];
}

#endif /* __HH_MPP_POLYCONV */
