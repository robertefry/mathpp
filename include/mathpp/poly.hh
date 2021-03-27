
#ifndef __HH_MPP_POLY
#define __HH_MPP_POLY

#include "mathpp/mathpp.hh"

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
        virtual ~Poly() = default;

        template <typename Tq>
        Poly(Poly<Tq> const&);

        template <typename Tq>
        Poly<Tp>& operator=(Poly<Tq> const&);

    protected:
        virtual void validate();

    public:
        virtual auto coeffs() const -> std::vector<Tp> const& { return m_Coefficients; }
        virtual auto order() const -> size_t { return m_Coefficients.size()-1; }
        virtual auto size() const -> size_t { return m_Coefficients.size(); }

        virtual void swap(Poly<Tp>& other);
        virtual void assign(size_t, Tp const&);
        virtual void assign(std::vector<Tp> const&);
        virtual void assign(std::vector<Tp>&&);
        virtual void assign(std::initializer_list<Tp>);
        virtual void clear();

        virtual auto operator[](size_t i) const -> Tp const& { return m_Coefficients[i]; }
        virtual auto operator[](size_t i) -> Tp& { return m_Coefficients[i]; }
        virtual auto at(size_t i) const -> Tp const& { return m_Coefficients.at(i); }
        virtual auto at(size_t i) -> Tp& { return m_Coefficients.at(i); }

        virtual Poly<Tp>& operator<<=(size_t);
        virtual Poly<Tp>& operator>>=(size_t);

        virtual Poly<Tp>& operator+=(Tp const&);
        virtual Poly<Tp>& operator-=(Tp const&);
        virtual Poly<Tp>& operator*=(Tp const&);
        virtual Poly<Tp>& operator/=(Tp const&);
        virtual Poly<Tp>& operator%=(Tp const&);

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

    protected:
        std::vector<Tp> m_Coefficients{};
    };

} // namespace mpp

/* ************************************************************************** */
// MathPP Specialisations
/* ************************************************************************** */

namespace mpp
{

    // identity

    template <typename Tp, typename Op>
    struct identity<mpp::Poly<Tp>,Op>
    {
        constexpr static tristate has()
        {
            return identity<Tp,Op>::has();
        }
        static mpp::Poly<Tp> const& get()
        {
            static mpp::Poly<Tp> s_ident = mpp::Poly<Tp>{identity<Tp,Op>::get()};
            return s_ident;
        }
        static mpp::Poly<Tp>& make(mpp::Poly<Tp>& poly)
        {
            return poly = get();
        }
    };

    // inverse

    template <typename Tp>
    struct inverse<mpp::Poly<Tp>,op_add>
    {
        constexpr static tristate has()
        {
            return mpp::inverse<Tp,op_add>::has();
        }
        constexpr static bool can(mpp::Poly<Tp> const& e)
        {
            for (auto const& coeff : e.coeffs())
            {
                if (mpp::inverse<Tp,op_add>::can(coeff) == false) return false;
            }
            return true;
        }
        static mpp::Poly<Tp> get(mpp::Poly<Tp> const& e)
        {
            return -e;
        }
        static mpp::Poly<Tp>& make(mpp::Poly<Tp>& e)
        {
            return e *= mpp::inverse<Tp,op_add>::get(mpp::identity<Tp,op_mul>::get());
        }
    };

    // absolute

    template <typename Tp>
    struct absolute<mpp::Poly<Tp>,op_add>
    {
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(mpp::Poly<Tp> const&)
        {
            return true;
        }
        static mpp::Poly<Tp> get(mpp::Poly<Tp> const& e)
        {
            auto const negative = e.coeffs().back() < mpp::identity<Tp,op_add>::get();
            return negative ? -e : e;
        }
        static mpp::Poly<Tp>& make(mpp::Poly<Tp>& e)
        {
            auto const negative = e.coeffs().back() < mpp::identity<Tp,op_add>::get();
            if (negative) e *= mpp::inverse<Tp,op_add>::get(mpp::identity<Tp,op_mul>::get());
            return e;
        }
    };

    // modulo

    template <typename Tp, typename Tq>
        requires std::is_convertible<Tq,mpp::Poly<Tp>>::value
    struct modulo<mpp::Poly<Tp>,Tq>
    {
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(mpp::Poly<Tp> const&, Tq const&)
        {
            return true;
        }
        static mpp::Poly<Tp> get(mpp::Poly<Tp> const& e, Tq const& n)
        {
            return e % n;
        }
        static mpp::Poly<Tp>& make(mpp::Poly<Tp>& e, Tq const& n)
        {
            return e %= n;
        }
    };

    template <typename Tp, typename Tq>
        requires std::is_convertible<Tq,Tp>::value
    struct modulo<mpp::Poly<Tp>,mpp::Poly<Tq>>
    {
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(mpp::Poly<Tp> const&, mpp::Poly<Tq> const&)
        {
            return true;
        }
        static mpp::Poly<Tp> get(mpp::Poly<Tp> const& e, mpp::Poly<Tq> const& n)
        {
            auto copy = e;
            return make(copy,n);
        }
        static mpp::Poly<Tp>& make(mpp::Poly<Tp>& e, mpp::Poly<Tq> const& n)
        {
            auto const c_coeffs = std::vector<Tq>{n.coeffs().begin(),n.coeffs().end()-1};
            auto const c_poly = -mpp::Poly<Tq>{std::move(c_coeffs)};

            while (e.order() >= n.order())
            {
                auto const coeff = e.coeffs().back();
                e -= mpp::Poly<Tp>{coeff} <<= e.order();
                e += (coeff * c_poly) <<= (e.order() - n.order() + 1);
            }
            return e;
        }
    };

    // division

    template <typename Tp, typename Tq>
        requires std::is_convertible<Tq,Tp>::value
    struct division<mpp::Poly<Tp>,Tq>
    {
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(mpp::Poly<Tp> const&, Tq const&)
        {
            return true;
        }
        static mpp::Poly<Tp> get(mpp::Poly<Tp> const& e, Tq const& c)
        {
            return e / c;
        }
        static mpp::Poly<Tp>& make(mpp::Poly<Tp>& e, Tq const& c)
        {
            return e /= c;
        }
    };

    template <typename Tp, typename Tq>
        requires std::is_convertible<Tq,Tp>::value
    struct division<mpp::Poly<Tp>,mpp::Poly<Tq>>
    {
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(mpp::Poly<Tp> const&, mpp::Poly<Tq> const&)
        {
            return true;
        }
        static auto get(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
        {
            using Tr = decltype(poly1[0]/poly2[0]);
            mpp::Poly<Tr> remainder = poly1;
            mpp::Poly<Tr> quotient;

            using abs = mpp::absolute<mpp::Poly<Tp>,op_add>;
            while (abs::get(remainder) >= abs::get(poly2))
            {
                auto const coeff = remainder.coeffs().back() / poly2.coeffs().back();
                auto const power = remainder.order() - poly2.order();
                auto const term = mpp::Poly<Tr>{coeff} <<= power;
                remainder -= term * poly2;
                quotient += term;
            }
            return std::make_tuple(remainder,quotient);
        }
    };

} // namespace mpp

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

template <typename Tp>
mpp::Poly<Tp>::Poly()
    : m_Coefficients{mpp::identity<Tp,op_add>::get()}
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
void mpp::Poly<Tp>::validate()
{
    auto const pred = [](auto const& coeff) { return coeff != mpp::identity<Tp,op_add>::get(); };
    auto const itr = std::find_if(m_Coefficients.rbegin(), m_Coefficients.rend(), pred);
    m_Coefficients.erase(itr.base(), m_Coefficients.end());
    if (m_Coefficients.size() == 0) m_Coefficients.push_back(mpp::identity<Tp,op_add>::get());
}

template <typename Tp>
void mpp::Poly<Tp>::swap(mpp::Poly<Tp>& other)
{
    std::swap(m_Coefficients, other.m_Coefficients);
    validate();
    other.validate();
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
mpp::Poly<Tp>& mpp::Poly<Tp>::operator>>=(size_t n)
{
    if (size() <= n)
    {
        *this = Poly<Tp>{};
    }
    else
    {
        m_Coefficients.erase(m_Coefficients.begin(), m_Coefficients.begin()+n);
    }
    validate();
    return *this;
}

template <typename Tp>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator<<=(size_t n)
{
    m_Coefficients.insert(m_Coefficients.begin(), n, mpp::identity<Tp,op_add>::get());
    validate();
    return *this;
}

template <typename Tp>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator+=(Tp const& c)
{
    m_Coefficients[0] += c;
    validate();
    return *this;
}

template <typename Tp>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator-=(Tp const& c)
{
    m_Coefficients[0] -= c;
    validate();
    return *this;
}

template <typename Tp>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator*=(Tp const& c)
{
    for (Tp& coeff : m_Coefficients)
    {
        coeff *= c;
    }
    validate();
    return *this;
}

template <typename Tp>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator/=(Tp const& c)
{
    for (Tp& coeff : m_Coefficients)
    {
        coeff /= c;
    }
    validate();
    return *this;
}

template <typename Tp>
mpp::Poly<Tp>& mpp::Poly<Tp>::operator%=(Tp const& c)
{
    for (Tp& coeff : m_Coefficients)
    {
        mpp::modulo<Tp,Tp>::make(coeff,c);
    }
    validate();
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
    assign(std::move(coeffs)); // validates
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
        auto const term = mpp::Poly<Tp>{coeff} <<= power;
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
        auto const term = mpp::Poly<Tp>{coeff} <<= power;
        *this -= other * term;
    }
    return *this;
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
auto operator<=>(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
    -> std::compare_three_way_result_t<Tp,Tq>
{
    using mpp::op_add;  using mpp::op_mul;
    if (poly1.order() > poly2.order()) return poly1.coeffs().back() <=> mpp::identity<Tp,op_add>::get();
    if (poly1.order() < poly2.order()) return mpp::identity<Tp,op_add>::get() <=> poly2.coeffs().back();

    const size_t order = poly1.order();
    for (size_t i = order; i <= order; --i)
    {
        if (poly1[i] > poly2[i]) return std::compare_three_way_result_t<Tp,Tq>::greater;
        if (poly1[i] < poly2[i]) return std::compare_three_way_result_t<Tp,Tq>::less;
    }
    return std::compare_three_way_result_t<Tp,Tq>::equivalent;
}

template <typename Tp>
auto operator>>(mpp::Poly<Tp> const& poly, size_t n)
{
    if (poly.size() <= n) {
        return mpp::Poly<Tp>{};
    }

    auto coeffs = std::vector<Tp>{poly.coeffs().begin()+n,poly.coeffs().end()};
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator<<(mpp::Poly<Tp> const& poly, size_t n)
{
    using mpp::op_add;  using mpp::op_mul;
    auto coeffs = std::vector<Tp>(n,mpp::identity<Tp,op_add>::get());
    coeffs.insert(coeffs.end(),poly.coeffs().begin(),poly.coeffs().end());
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator+(mpp::Poly<Tp> const& poly)
{
    using mpp::op_add;  using mpp::op_mul;
    std::vector<Tp> coeffs;
    coeffs.reserve(poly.size());

    for (auto const& coeff : poly.coeffs())
    {
        coeffs.push_back(mpp::identity<Tp,op_add>::get()+coeff);
    }
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator-(mpp::Poly<Tp> const& poly)
{
    using mpp::op_add;  using mpp::op_mul;
    std::vector<Tp> coeffs;
    coeffs.reserve(poly.size());

    for (auto const& coeff : poly.coeffs())
    {
        coeffs.push_back(mpp::identity<Tp,op_add>::get()-coeff);
    }
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator+(mpp::Poly<Tp> const& poly, Tp const& c)
{
    std::vector<Tp> coeffs = poly.coeffs();
    coeffs[0] = coeffs[0] + c;
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator+(Tp const& c, mpp::Poly<Tp> const& poly)
{
    std::vector<Tp> coeffs = poly.coeffs();
    coeffs[0] = c + coeffs[0];
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator-(mpp::Poly<Tp> const& poly, Tp const& c)
{
    std::vector<Tp> coeffs = poly.coeffs();
    coeffs[0] = coeffs[0] - c;
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator-(Tp const& c, mpp::Poly<Tp> const& poly)
{
    std::vector<Tp> coeffs = poly.coeffs();
    coeffs[0] = c - coeffs[0];
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator*(mpp::Poly<Tp> const& poly, Tp const& c)
{
    std::vector<Tp> coeffs;
    coeffs.reserve(poly.size());

    for (Tp const& coeff : poly.coeffs())
    {
        coeffs.push_back(coeff*c);
    }
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator*(Tp const& c, mpp::Poly<Tp> const& poly)
{
    std::vector<Tp> coeffs;
    coeffs.reserve(poly.size());

    for (Tp const& coeff : poly.coeffs())
    {
        coeffs.push_back(c*coeff);
    }
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator/(mpp::Poly<Tp> const& poly, Tp const& c)
{
    std::vector<Tp> coeffs;
    coeffs.reserve(poly.size());

    for (Tp const& coeff : poly.coeffs())
    {
        coeffs.push_back(coeff/c);
    }
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp>
auto operator%(mpp::Poly<Tp> const& poly, Tp const& c)
{
    std::vector<Tp> coeffs;
    coeffs.reserve(poly.size());

    for (Tp const& coeff : poly.coeffs())
    {
        coeffs.push_back(mpp::modulo<Tp,Tp>::get(coeff,c));
    }
    return mpp::Poly<Tp>{coeffs};
}

template <typename Tp, typename Tq>
auto operator+(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
{
    using mpp::op_add;  using mpp::op_mul;
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
        coeffs.push_back(mpp::identity<Tp,op_add>::get()+poly2[index]);
    }
    return mpp::Poly<Tr>{std::move(coeffs)};
}

template <typename Tp, typename Tq>
auto operator-(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
{
    using mpp::op_add;  using mpp::op_mul;
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
        coeffs.push_back(mpp::identity<Tp,op_add>::get()-poly2[index]);
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
    using division = mpp::division<mpp::Poly<Tp>,mpp::Poly<Tq>>;
    return std::get<1>(division::get(poly1,poly2));
}

template <typename Tp, typename Tq>
auto operator%(mpp::Poly<Tp> const& poly1, mpp::Poly<Tq> const& poly2)
{
    using division = mpp::division<mpp::Poly<Tp>,mpp::Poly<Tq>>;
    return std::get<0>(division::get(poly1,poly2));
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
