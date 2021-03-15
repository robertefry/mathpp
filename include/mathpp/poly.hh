
#ifndef __HH_MPP_POLY
#define __HH_MPP_POLY

#include "mathpp/group_traits.hh"

#include <vector>
#include <tuple>
#include <algorithm>
#include <compare>

/* ************************************************************************** */
// Definitions
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp>
    class Poly
    {
    public:
        Poly();
        Poly(size_t, Tp const&);
        Poly(std::vector<Tp> const&);
        Poly(std::vector<Tp>&&);
        Poly(std::initializer_list<Tp>);

        template <typename Tq>
        Poly(Poly<Tq> const&);

        template <typename Tq>
        Poly<Tp>& operator=(Poly<Tq> const&);
        template <typename Tq>
        Poly<Tp>& operator=(std::initializer_list<Tq>);

    private:
        void validate();

    public:
        auto coeffs() const -> std::vector<Tp> const& { return m_Coefficients; }
        auto order() const { return m_Coefficients.size()-1; }
        auto size() const { return m_Coefficients.size(); }

        void assign(size_t, Tp const&);
        void assign(std::vector<Tp> const&);
        void assign(std::vector<Tp>&&);
        void assign(std::initializer_list<Tp>);
        void clear();

        auto operator[](size_t i) const -> Tp const& { return m_Coefficients[i]; }
        auto operator[](size_t i) -> Tp& { return m_Coefficients[i]; }
        auto at(size_t i) const -> Tp const& { return m_Coefficients.at(i); }
        auto at(size_t i) -> Tp& { return m_Coefficients.at(i); }

        void swap(Poly<Tp>& other) { std::swap(m_Coefficients, other.m_Coefficients); }

        Poly<Tp>& operator<<=(size_t);
        Poly<Tp>& operator>>=(size_t);

        template <typename Tq>
        Poly<Tp>& operator+=(Poly<Tq> const&);
        template <typename Tq>
        Poly<Tp>& operator-=(Poly<Tq> const&);
        template <typename Tq>
        Poly<Tp>& operator*=(Poly<Tq> const&);
        template <typename Tq>
        Poly<Tp>& operator/=(Poly<Tq> const&);
        template <typename Tq>
        Poly<Tp>& operator%=(Poly<Tq> const&);

    private:
        std::vector<Tp> m_Coefficients{};
    };

    template <typename Tp, typename Tq>
    auto divide_euclidean(mpp::Poly<Tp> const&, mpp::Poly<Tq> const&);

} // namespace mpp

/* Group traits ************************************************************* */

namespace mpp
{

    MPP_REQUIRE_EUCLIDEAN_DOMAIN(typename Tp,mpp::Poly<Tp>,op_add,op_mul,true);

    template <typename Tp, typename Op>
    struct identity<mpp::Poly<Tp>,Op>
    {
        static mpp::Poly<Tp> const& get()
        {
            static mpp::Poly<Tp> s_ident = mpp::Poly<Tp>{identity<Tp,Op>::get()};
            return s_ident;
        }
    };

    template <typename Tp>
    struct inverse<mpp::Poly<Tp>,op_add>
    {
        static mpp::Poly<Tp> get(mpp::Poly<Tp> const& e)
        {
            return -e;
        }
        static mpp::Poly<Tp>& make(mpp::Poly<Tp>& e)
        {
            return e *= -1;
        }
    };

    template <typename Tp, typename Op>
    struct absolute<mpp::Poly<Tp>,Op>
    {
        static mpp::Poly<Tp> get(mpp::Poly<Tp> const& e)
        {
            using identity = mpp::identity<mpp::Poly<Tp>,Op>;
            using inverse = mpp::inverse<mpp::Poly<Tp>,Op>;
            return (e < identity::get()) ? inverse::get(e) : e;
        }
        static mpp::Poly<Tp> make(mpp::Poly<Tp>& e)
        {
            using identity = mpp::identity<mpp::Poly<Tp>,Op>;
            using inverse = mpp::inverse<mpp::Poly<Tp>,Op>;
            return (e < identity::get()) ? inverse::make(e) : e;
        }
    };

} // namespace mpp

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

template <typename Tp>
mpp::Poly<Tp>::Poly()
    : m_Coefficients{Tp{}}
{
}

template <typename Tp>
mpp::Poly<Tp>::Poly(size_t n, Tp const& val)
    : m_Coefficients(n,val)
{
    validate();
}

template <typename Tp>
mpp::Poly<Tp>::Poly(std::vector<Tp> const& coeffs)
    : m_Coefficients{coeffs}
{
    validate();
}

template <typename Tp>
mpp::Poly<Tp>::Poly(std::vector<Tp>&& coeffs)
    : m_Coefficients{std::move(coeffs)}
{
    validate();
}

template <typename Tp>
mpp::Poly<Tp>::Poly(std::initializer_list<Tp> coeffs)
    : m_Coefficients{coeffs}
{
    validate();
}

template <typename Tp>
template <typename Tq>
mpp::Poly<Tp>::Poly(Poly<Tq> const& other)
{
    m_Coefficients.reserve(other.size());

    for (auto const& elem : other.coeffs())
    {
        m_Coefficients.push_back(static_cast<Tp>(elem));
    }
    validate();
}

template <typename Tp>
template <typename Tq>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator=(Poly<Tq> const& other)
{
    m_Coefficients.clear();
    m_Coefficients.reserve(other.size());

    for (auto const& elem : other.coeffs())
    {
        m_Coefficients.push_back(static_cast<Tp>(elem));
    }
    validate();
    return *this;
}


template <typename Tp>
template <typename Tq>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator=(std::initializer_list<Tq> coeffs)
{
    m_Coefficients = coeffs;
    validate();
    return *this;
}

template <typename Tp>
void mpp::Poly<Tp>::validate()
{
    auto itr = std::find_if(m_Coefficients.rbegin(), m_Coefficients.rend(), [](auto coeff){ return coeff != Tp{}; });
    m_Coefficients.erase(itr.base(), m_Coefficients.end());
    if (m_Coefficients.size() == 0) m_Coefficients.emplace_back();
}

template <typename Tp>
void mpp::Poly<Tp>::assign(size_t n, Tp const& val)
{
    m_Coefficients = std::vector<Tp>(n,val);
    validate();
}

template <typename Tp>
void mpp::Poly<Tp>::assign(std::vector<Tp> const& coeffs)
{
    m_Coefficients = coeffs;
    validate();
}

template <typename Tp>
void mpp::Poly<Tp>::assign(std::vector<Tp>&& coeffs)
{
    m_Coefficients = std::move(coeffs);
    validate();
}

template <typename Tp>
void mpp::Poly<Tp>::assign(std::initializer_list<Tp> coeffs)
{
    m_Coefficients = coeffs;
    validate();
}

template <typename Tp>
void mpp::Poly<Tp>::clear()
{
    m_Coefficients.clear();
    validate();
}

template <typename Tp>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator<<=(size_t n)
{
    if (m_Coefficients.size() <= n)
    {
        *this = Poly<Tp>{};
    }
    else
    {
        m_Coefficients.erase(m_Coefficients.begin(), m_Coefficients.begin()+n);
    }
    return *this;
}

template <typename Tp>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator>>=(size_t n)
{
    m_Coefficients.insert(m_Coefficients.begin(), n, Tp{});
    return *this;
}

template <typename Tp>
template <typename Tq>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator+=(Poly<Tq> const& other)
{
    m_Coefficients.resize(std::max(size(),other.size()));
    for (size_t i = 0; i < other.size(); ++i) {
        m_Coefficients[i] += other[i];  // could be zero
    }
    validate();
    return *this;
}

template <typename Tp>
template <typename Tq>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator-=(Poly<Tq> const& other)
{
    m_Coefficients.resize(std::max(size(),other.size()));
    for (size_t i = 0; i < other.size(); ++i) {
        m_Coefficients[i] -= other[i];  // could be zero
    }
    validate();
    return *this;
}

template <typename Tp>
template <typename Tq>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator*=(Poly<Tq> const& other)
{
    std::vector<Tp> coeffs;
    coeffs.resize(size()*other.size());

    for (size_t i = 0; i < size(); ++i)
    {
        for (size_t j = 0; j < other.size(); ++j)
        {
            coeffs[i+j] += m_Coefficients[i] * other[j];
        }
    }
    assign(coeffs); // validates
    return *this;
}

template <typename Tp>
template <typename Tq>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator/=(Poly<Tq> const& other)
{
    mpp::Poly<Tp> quotient;

    using abs = mpp::absolute<mpp::Poly<Tp>,op_add>;
    while (abs::get(*this) >= abs::get(other))
    {
        auto const coeff = coeffs().back() / other.coeffs().back();
        auto const power = order() - other.order();
        auto const term = mpp::Poly<Tp>{coeff} >>= power;
        quotient += term;
        *this -= term * other;
    }
    return *this = std::move(quotient);
}

template <typename Tp>
template <typename Tq>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator%=(Poly<Tq> const& other)
{
    using abs = mpp::absolute<mpp::Poly<Tp>,op_add>;
    while (abs::get(*this) >= abs::get(other))
    {
        auto const coeff = coeffs().back() / other.coeffs().back();
        auto const power = order() - other.order();
        auto const term = mpp::Poly<Tp>{coeff} >>= power;
        *this -= other * term;
    }
    return *this;
}

/* ************************************************************************** */
// Namespace Extensions
/* ************************************************************************** */

template <typename Tp, typename Tq>
auto mpp::divide_euclidean(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
{
    using Tr = decltype(poly1[0]/poly2[0]);
    using Ts = decltype(poly1[0]%poly2[0]);
    mpp::Poly<Tr> quotient;
    mpp::Poly<Ts> remainder = poly1;

    using abs = mpp::absolute<mpp::Poly<Tp>,op_add>;
    while (abs::get(remainder) >= abs::get(poly2))
    {
        auto const coeff = remainder.coeffs().back() / poly2.coeffs().back();
        auto const power = remainder.order() - poly2.order();
        auto const term = mpp::Poly<Tr>{coeff} >>= power;
        quotient += term;
        remainder -= term * poly2;
    }
    return std::tuple<mpp::Poly<Tr>,mpp::Poly<Ts>>{quotient,remainder};
}

/* ************************************************************************** */
// External Extensions
/* ************************************************************************** */

template <typename Tp, typename Tq>
bool operator==(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
{
    if (poly1.order() != poly2.order()) return false;

    for (size_t i = poly1.order(); i <= poly1.order(); --i)
    {
        if (poly1[i] != poly2[i]) return false;
    }
    return true;
}

template <typename Tp>
bool operator==(mpp::Poly<Tp> const& poly1, mpp::Poly<Tp> const& poly2)
{
    return poly1.coeffs() == poly2.coeffs();
}

template <typename Tp, typename Tq>
std::compare_three_way_result_t<Tp,Tq> operator<=>(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
{
    if (poly1.order() > poly2.order()) return poly1.coeffs().back() <=> mpp::identity<Tp,mpp::op_add>::get();
    if (poly1.order() < poly2.order()) return mpp::identity<Tp,mpp::op_add>::get() <=> poly2.coeffs().back();

    const size_t order = poly1.order();
    for (size_t i = order; i <= order; --i)
    {
        if (poly1[i] > poly2[i]) return std::compare_three_way_result_t<Tp,Tq>::greater;
        if (poly1[i] < poly2[i]) return std::compare_three_way_result_t<Tp,Tq>::less;
    }
    return std::compare_three_way_result_t<Tp,Tq>::equivalent;
}

template <typename Tp>
auto operator<<(mpp::Poly<Tp> const& poly, size_t n)
{
    if (poly.size() <= n) {
        return mpp::Poly<Tp>{};
    }

    auto coeffs = std::vector<Tp>{poly.coeffs().begin()+n,poly.coeffs().end()};
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator>>(mpp::Poly<Tp> const& poly, size_t n)
{
    auto coeffs = std::vector<Tp>(n,Tp{});
    coeffs.insert(coeffs.end(),poly.coeffs().begin(),poly.coeffs().end());
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator+(mpp::Poly<Tp> const& poly)
{
    using Tr = decltype(Tp{}+poly[0]);
    std::vector<Tr> coeffs;
    coeffs.reserve(poly.size());

    for (auto const& coeff : poly.coeffs())
    {
        coeffs.push_back(Tp{}+coeff);
    }
    return mpp::Poly<Tr>{coeffs};
}

template <typename Tp>
auto operator-(mpp::Poly<Tp> const& poly)
{
    using Tr = decltype(Tp{}-poly[0]);
    std::vector<Tr> coeffs;
    coeffs.reserve(poly.size());

    for (auto const& coeff : poly.coeffs())
    {
        coeffs.push_back(Tp{}-coeff);
    }
    return mpp::Poly<Tr>{coeffs};
}

template <typename Tp, typename Tq>
auto operator+(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
{
    using Tr = decltype(poly1[0]+poly2[0]);
    std::vector<Tr> coeffs;
    coeffs.reserve(std::max(poly1.size(),poly2.size()));

    size_t index = 0;
    for (; index <= std::min(poly1.order(),poly2.order()); ++index)
    {
        coeffs.push_back(poly1[index]+poly2[index]);
    }
    for (; index <= poly1.order(); ++index)
    {
        coeffs.push_back(poly1[index]+Tq{});
    }
    for (; index <= poly2.order(); ++index)
    {
        coeffs.push_back(Tp{}+poly2[index]);
    }
    return mpp::Poly<Tr>{std::move(coeffs)};
}

template <typename Tp, typename Tq>
auto operator-(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
{
    using Tr = decltype(poly1[0]-poly2[0]);
    std::vector<Tr> coeffs;
    coeffs.reserve(std::max(poly1.size(),poly2.size()));

    size_t index = 0;
    for (; index <= std::min(poly1.order(),poly2.order()); ++index)
    {
        coeffs.push_back(poly1[index]-poly2[index]);
    }
    for (; index <= poly1.order(); ++index)
    {
        coeffs.push_back(poly1[index]-Tq{});
    }
    for (; index <= poly2.order(); ++index)
    {
        coeffs.push_back(Tp{}-poly2[index]);
    }
    return mpp::Poly<Tr>{std::move(coeffs)};
}

template <typename Tp, typename Tq>
auto operator*(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
{
    using Tr = decltype(poly1[0]*poly2[0]);
    std::vector<Tr> coeffs;
    coeffs.resize(poly1.size()*poly2.size());

    for (size_t i = 0; i <= poly1.order(); ++i)
    {
        for (size_t j = 0; j <= poly2.order(); ++j)
        {
            coeffs[i+j] += poly1[i] * poly2[j];
        }
    }
    auto itr = std::find_if(coeffs.rbegin(),coeffs.rend(),[](auto coeff){ return coeff != Tr{}; });
    coeffs.erase(itr.base(),coeffs.end());
    return mpp::Poly<Tr>{std::move(coeffs)};
}

template <typename Tp, typename Tq>
auto operator/(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
{
    auto result = mpp::divide_euclidean(poly1,poly2);
    return std::get<0>(result);
}

template <typename Tp, typename Tq>
auto operator%(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
{
    auto result = mpp::divide_euclidean(poly1,poly2);
    return std::get<1>(result);
}

/* ************************************************************************** */
// Standard Extensions
/* ************************************************************************** */

namespace std
{

    template <typename Tp>
    void swap(mpp::Poly<Tp>& poly1, mpp::Poly<Tp>& poly2)
    {
        poly1.swap(poly2);
    }

} // namespace std

#endif /* __HH_MPP_POLY */
