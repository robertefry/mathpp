
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
        explicit Poly();
        explicit Poly(size_t, Tp const&);
        virtual ~Poly() = default;

        Poly(std::initializer_list<Tp>);
        Poly(std::vector<Tp> const&);
        Poly(std::vector<Tp>&&);

        template <typename Tq>
        Poly(Poly<Tq> const&);

        template <typename Tq>
        Poly<Tp>& operator=(Poly<Tq> const&);

    protected:
        virtual void validate();

    public:
        virtual void trim();

        virtual auto coeffs() const -> std::vector<Tp> const& { return m_Coefficients; }
        virtual auto coeffs() -> std::vector<Tp>& { return m_Coefficients; }
        virtual auto size() const -> size_t { return m_Coefficients.size(); }

        virtual auto order() const -> size_t;
        virtual auto front() const -> Tp const& { return m_Coefficients[0]; }
        virtual auto back() const -> Tp const& { return m_Coefficients[order()]; }

        virtual void swap(Poly<Tp>&);
        virtual void assign(size_t, Tp const&);
        virtual void assign(std::initializer_list<Tp>);
        virtual void assign(std::vector<Tp> const&);
        virtual void assign(std::vector<Tp>&&);
        virtual void resize(size_t);
        virtual void resize(size_t, Tp const&);
        virtual void zero();

        virtual auto operator[](size_t i) const -> Tp const&;
        virtual auto operator[](size_t i) -> Tp&;
        virtual auto at(size_t i) const -> Tp const&;
        virtual auto at(size_t i) -> Tp&;

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
        Poly<Tp>& operator%=(Poly<Tq> const&);

    protected:
        mutable std::vector<Tp> m_Coefficients{};
    };

} // namespace mpp

/* ************************************************************************** */
// MathPP Specialisations
/* ************************************************************************** */

namespace mpp
{

    // identity

    template <typename Tp, typename Op>
    struct identity<Poly<Tp>,Op>
    {
        constexpr static tristate has()
        {
            return identity<Tp,Op>::has();
        }
        constexpr static Poly<Tp> get()
        {
            return Poly<Tp>{identity<Tp,Op>::get()};
        }
        constexpr static Poly<Tp>& make(Poly<Tp>& poly)
        {
            return poly = get();
        }
    };

    // inverse

    template <typename Tp>
    struct inverse<Poly<Tp>,op_add>
    {
        constexpr static tristate has()
        {
            return inverse<Tp,op_add>::has();
        }
        constexpr static bool can(Poly<Tp> const&)
        {
            return has() != logic::none;
        }
        constexpr static Poly<Tp> get(Poly<Tp> const& e)
        {
            return -e;
        }
        constexpr static Poly<Tp>& make(Poly<Tp>& e)
        {
            return e = get(e);
        }
    };

    // division

    template <typename Tp, typename Tq>
    struct division<Poly<Tp>,Poly<Tq>>
    {
        constexpr static tristate has()
        {
            return logic::all;
        }
        constexpr static bool can(Poly<Tp> const&, Poly<Tq> const&)
        {
            return true;
        }
        constexpr static auto get(Poly<Tp> const& poly1, Poly<Tq> const& poly2)
        {
            using Tr = op_mul::result<Tp,Tq>::type;
            Poly<Tr> remainder = poly1;
            Poly<Tr> quotient;

            using abs = absolute<Poly<Tp>,op_add>;
            while (abs::get(remainder) >= abs::get(poly2))
            {
                auto const coeff = remainder.back() / poly2.back();
                auto const power = remainder.order() - poly2.order();
                auto const term = Poly<Tr>{coeff} <<= power;
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

namespace mpp
{

    template <typename Tp>
    Poly<Tp>::Poly()
        : m_Coefficients{identity<Tp,op_add>::get()}
    {
    }

    template <typename Tp>
    Poly<Tp>::Poly(size_t n, Tp const& val)
        : m_Coefficients(n,val)
    {
        validate();
    }

    template <typename Tp>
    Poly<Tp>::Poly(std::initializer_list<Tp> coeffs)
        : m_Coefficients{coeffs}
    {
        validate();
    }

    template <typename Tp>
    Poly<Tp>::Poly(std::vector<Tp> const& coeffs)
        : m_Coefficients{coeffs}
    {
        validate();
    }

    template <typename Tp>
    Poly<Tp>::Poly(std::vector<Tp>&& coeffs)
        : m_Coefficients{std::move(coeffs)}
    {
        validate();
    }

    template <typename Tp>
    template <typename Tq>
    Poly<Tp>::Poly(Poly<Tq> const& other)
    {
        if constexpr(std::is_same<Tp,Tq>::value)
        {
            m_Coefficients = other.coeffs();
        }
        else
        {
            m_Coefficients.reserve(other.size());

            for (auto const& elem : other.coeffs())
            {
                m_Coefficients.push_back(static_cast<Tp>(elem));
            }
        }
    }

    template <typename Tp>
    template <typename Tq>
    Poly<Tp>& Poly<Tp>::operator=(Poly<Tq> const& other)
    {
        if constexpr(std::is_same<Tp,Tq>::value)
        {
            m_Coefficients = other.coeffs();
        }
        else
        {
            m_Coefficients.clear();
            m_Coefficients.reserve(other.size());

            for (auto const& elem : other.coeffs())
            {
                m_Coefficients.push_back(static_cast<Tp>(elem));
            }
        }
        return *this;
    }

    template <typename Tp>
    void Poly<Tp>::validate()
    {
        if (m_Coefficients.size() == 0) m_Coefficients.push_back(identity<Tp,op_add>::get());
    }

    template <typename Tp>
    void Poly<Tp>::trim()
    {
        m_Coefficients.erase(m_Coefficients.begin()+order(),m_Coefficients.end());
        validate();
    }

    template <typename Tp>
    auto Poly<Tp>::order() const
        -> size_t
    {
        for (size_t i = m_Coefficients.size() - 1; i < m_Coefficients.size(); --i)
        {
            if (m_Coefficients[i] != identity<Tp,op_add>::get()) return i;
        }
        return 0;
    }

    template <typename Tp>
    void Poly<Tp>::swap(Poly<Tp>& other)
    {
        std::swap(m_Coefficients, other.m_Coefficients);
        validate();
        other.validate();
    }

    template <typename Tp>
    void Poly<Tp>::assign(size_t n, Tp const& val)
    {
        m_Coefficients = std::vector<Tp>(n,val);
        validate();
    }

    template <typename Tp>
    void Poly<Tp>::assign(std::initializer_list<Tp> coeffs)
    {
        m_Coefficients = coeffs;
        validate();
    }

    template <typename Tp>
    void Poly<Tp>::assign(std::vector<Tp> const& coeffs)
    {
        m_Coefficients = coeffs;
        validate();
    }

    template <typename Tp>
    void Poly<Tp>::assign(std::vector<Tp>&& coeffs)
    {
        m_Coefficients = std::move(coeffs);
        validate();
    }

    template <typename Tp>
    void Poly<Tp>::resize(size_t size)
    {
        m_Coefficients.resize(size);
        validate();
    }

    template <typename Tp>
    void Poly<Tp>::resize(size_t size, Tp const& value)
    {
        m_Coefficients.resize(size,value);
        validate();
    }

    template <typename Tp>
    void Poly<Tp>::zero()
    {
        m_Coefficients.clear();
        validate();
    }

    template <typename Tp>
    auto Poly<Tp>::operator[](size_t i) const
        -> Tp const&
    {
        if (i >= m_Coefficients.size()) m_Coefficients.resize(i+1);
        return m_Coefficients[i];
    }

    template <typename Tp>
    auto Poly<Tp>::operator[](size_t i)
        -> Tp&
    {
        if (i >= m_Coefficients.size()) m_Coefficients.resize(i+1);
        return m_Coefficients[i];
    }

    template <typename Tp>
    auto Poly<Tp>::at(size_t i) const
        -> Tp const&
    {
        if (i >= m_Coefficients.size()) m_Coefficients.resize(i+1);
        return m_Coefficients.at(i);
    }

    template <typename Tp>
    auto Poly<Tp>::at(size_t i)
        -> Tp&
    {
        if (i >= m_Coefficients.size()) m_Coefficients.resize(i+1);
        return m_Coefficients.at(i);
    }


    template <typename Tp>
    Poly<Tp>& Poly<Tp>::operator>>=(size_t n)
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
    Poly<Tp>& Poly<Tp>::operator<<=(size_t n)
    {
        m_Coefficients.insert(m_Coefficients.begin(), n, identity<Tp,op_add>::get());
        validate();
        return *this;
    }

    template <typename Tp>
    Poly<Tp>& Poly<Tp>::operator+=(Tp const& c)
    {
        m_Coefficients[0] += c;
        validate();
        return *this;
    }

    template <typename Tp>
    Poly<Tp>& Poly<Tp>::operator-=(Tp const& c)
    {
        m_Coefficients[0] -= c;
        validate();
        return *this;
    }

    template <typename Tp>
    Poly<Tp>& Poly<Tp>::operator*=(Tp const& c)
    {
        for (Tp& coeff : m_Coefficients)
        {
            coeff *= c;
        }
        validate();
        return *this;
    }

    template <typename Tp>
    Poly<Tp>& Poly<Tp>::operator/=(Tp const& c)
    {
        for (Tp& coeff : m_Coefficients)
        {
            coeff /= c;
        }
        validate();
        return *this;
    }

    template <typename Tp>
    Poly<Tp>& Poly<Tp>::operator%=(Tp const& c)
    {
        for (Tp& coeff : m_Coefficients)
        {
            modulo<Tp,Tp>::make(coeff,c);
        }
        validate();
        return *this;
    }

    template <typename Tp>
    template <typename Tq>
    Poly<Tp>& Poly<Tp>::operator+=(Poly<Tq> const& other)
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
    Poly<Tp>& Poly<Tp>::operator-=(Poly<Tq> const& other)
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
    Poly<Tp>& Poly<Tp>::operator*=(Poly<Tq> const& other)
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
    Poly<Tp>& Poly<Tp>::operator%=(Poly<Tq> const& other)
    {
        using abs = absolute<Poly<Tp>,op_add>;
        while (abs::get(*this) >= abs::get(other))
        {
            auto const coeff = back() / other.back();
            auto const power = order() - other.order();
            auto const term = Poly<Tp>{coeff} <<= power;
            *this -= other * term;
        }
        return *this;
    }

} // namespace mpp

/* ************************************************************************** */
// Non-Member Extensions
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp, typename Tq>
        requires std::totally_ordered_with<Tp,Tq>
    bool operator==(Poly<Tp> const& poly1, Poly<Tq> const& poly2)
    {
        if (poly1.order() != poly2.order()) return false;

        for (size_t i = poly1.order(); i <= poly1.order(); --i)
        {
            if (poly1[i] != poly2[i]) return false;
        }
        return true;
    }

    template <typename Tp, typename Tq>
        requires std::totally_ordered_with<Tp,Tq>
    auto operator<=>(Poly<Tp> const& poly1, Poly<Tq> const& poly2)
        -> std::compare_three_way_result_t<Tp,Tq>
    {
        if (poly1.order() > poly2.order()) return poly1.back() <=> identity<Tp,op_add>::get();
        if (poly1.order() < poly2.order()) return identity<Tp,op_add>::get() <=> poly2.back();

        const size_t order = poly1.order();
        for (size_t i = order; i <= order; --i)
        {
            if (poly1[i] > poly2[i]) return std::compare_three_way_result_t<Tp,Tq>::greater;
            if (poly1[i] < poly2[i]) return std::compare_three_way_result_t<Tp,Tq>::less;
        }
        return std::compare_three_way_result_t<Tp,Tq>::equivalent;
    }

    template <typename Tp>
    auto operator>>(Poly<Tp> const& poly, size_t n)
    {
        if (poly.size() <= n) {
            return Poly<Tp>{};
        }

        auto coeffs = std::vector<Tp>{poly.coeffs().begin()+n,poly.coeffs().end()};
        return Poly<Tp>{coeffs};
    }

    template <typename Tp>
    auto operator<<(Poly<Tp> const& poly, size_t n)
    {
        auto coeffs = std::vector<Tp>(n,identity<Tp,op_add>::get());
        coeffs.insert(coeffs.end(),poly.coeffs().begin(),poly.coeffs().end());
        return Poly<Tp>{coeffs};
    }

    template <typename Tp>
        requires requires (Tp a, Tp b) { a + b; }
    auto operator+(Poly<Tp> const& poly)
    {
        std::vector<Tp> coeffs;
        coeffs.reserve(poly.size());

        for (auto const& coeff : poly.coeffs())
        {
            coeffs.push_back(identity<Tp,op_add>::get()+coeff);
        }
        return Poly<Tp>{coeffs};
    }

    template <typename Tp>
        requires requires (Tp a, Tp b) { a - b; }
    auto operator-(Poly<Tp> const& poly)
    {
        std::vector<Tp> coeffs;
        coeffs.reserve(poly.size());

        for (auto const& coeff : poly.coeffs())
        {
            coeffs.push_back(identity<Tp,op_add>::get()-coeff);
        }
        return Poly<Tp>{coeffs};
    }

    template <typename Tp, typename Tq>
        requires requires (Tp a, Tq b) { a + b; }
    auto operator+(Poly<Tp> const& poly1, Poly<Tq> const& poly2)
    {
        using Tr = op_add::result<Tp,Tq>::type;
        std::vector<Tr> coeffs;
        coeffs.reserve(std::max(poly1.size(),poly2.size()));

        size_t index = 0;
        for (; index <= std::min(poly1.order(),poly2.order()); ++index)
        {
            coeffs.push_back(poly1[index]+poly2[index]);
        }
        for (; index <= poly1.order(); ++index)
        {
            coeffs.push_back(poly1[index]+identity<Tq,op_add>::get());
        }
        for (; index <= poly2.order(); ++index)
        {
            coeffs.push_back(identity<Tp,op_add>::get()+poly2[index]);
        }
        return Poly<Tr>{std::move(coeffs)};
    }

    template <typename Tp>
        requires requires (Tp a, Tp b) { a + b; }
    auto operator+(Poly<Tp> const& poly, Tp const& c)
    {
        std::vector<Tp> coeffs = poly.coeffs();
        coeffs[0] = coeffs[0] + c;
        return Poly<Tp>{coeffs};
    }

    template <typename Tp>
        requires requires (Tp a, Tp b) { a + b; }
    auto operator+(Tp const& c, Poly<Tp> const& poly)
    {
        std::vector<Tp> coeffs = poly.coeffs();
        coeffs[0] = c + coeffs[0];
        return Poly<Tp>{coeffs};
    }

    template <typename Tp, typename Tq>
        requires requires (Tp a, Tq b) { a - b; }
    auto operator-(Poly<Tp> const& poly1, Poly<Tq> const& poly2)
    {
        using Tr = op_add::result<Tp,Tq>::type;
        std::vector<Tr> coeffs;
        coeffs.reserve(std::max(poly1.size(),poly2.size()));

        size_t index = 0;
        for (; index <= std::min(poly1.order(),poly2.order()); ++index)
        {
            coeffs.push_back(poly1[index]-poly2[index]);
        }
        for (; index <= poly1.order(); ++index)
        {
            coeffs.push_back(poly1[index]-identity<Tq,op_add>::get());
        }
        for (; index <= poly2.order(); ++index)
        {
            coeffs.push_back(identity<Tp,op_add>::get()-poly2[index]);
        }
        return Poly<Tr>{std::move(coeffs)};
    }

    template <typename Tp>
        requires requires (Tp a, Tp b) { a - b; }
    auto operator-(Poly<Tp> const& poly, Tp const& c)
    {
        std::vector<Tp> coeffs = poly.coeffs();
        coeffs[0] = coeffs[0] - c;
        return Poly<Tp>{coeffs};
    }

    template <typename Tp>
        requires requires (Tp a, Tp b) { a - b; }
    auto operator-(Tp const& c, Poly<Tp> const& poly)
    {
        std::vector<Tp> coeffs = poly.coeffs();
        coeffs[0] = c - coeffs[0];
        return Poly<Tp>{coeffs};
    }

    template <typename Tp, typename Tq>
        requires requires (Tp a, Tq b) { a * b; }
    auto operator*(Poly<Tp> const& poly1, Poly<Tq> const& poly2)
    {
        using Tr = op_add::result<Tp,Tq>::type;
        std::vector<Tr> coeffs;
        coeffs.resize(poly1.size()*poly2.size());

        for (size_t i = 0; i <= poly1.order(); ++i)
        {
            for (size_t j = 0; j <= poly2.order(); ++j)
            {
                coeffs[i+j] += poly1[i] * poly2[j];
            }
        }
        auto const pred = [](auto const& coeff){ return coeff != identity<Tr,op_add>::get(); };
        auto itr = std::find_if(coeffs.rbegin(),coeffs.rend(),pred);
        coeffs.erase(itr.base(),coeffs.end());
        return Poly<Tr>{std::move(coeffs)};
    }

    template <typename Tp>
        requires requires (Tp a, Tp b) { a * b; }
    auto operator*(Poly<Tp> const& poly, Tp const& c)
    {
        std::vector<Tp> coeffs;
        coeffs.reserve(poly.size());

        for (Tp const& coeff : poly.coeffs())
        {
            coeffs.push_back(coeff*c);
        }
        return Poly<Tp>{coeffs};
    }

    template <typename Tp>
        requires requires (Tp a, Tp b) { a * b; }
    auto operator*(Tp const& c, Poly<Tp> const& poly)
    {
        std::vector<Tp> coeffs;
        coeffs.reserve(poly.size());

        for (Tp const& coeff : poly.coeffs())
        {
            coeffs.push_back(c*coeff);
        }
        return Poly<Tp>{coeffs};
    }

    template <typename Tp>
        requires requires (Tp a, Tp b) { a / b; }
    auto operator/(Poly<Tp> const& poly, Tp const& c)
    {
        std::vector<Tp> coeffs;
        coeffs.reserve(poly.size());

        for (Tp const& coeff : poly.coeffs())
        {
            coeffs.push_back(coeff/c);
        }
        return Poly<Tp>{coeffs};
    }

    template <typename Tp, typename Tq>
        requires (division<Poly<Tp>,Poly<Tq>>::has() != logic::none)
    auto operator%(Poly<Tp> const& poly1, Poly<Tq> const& poly2)
    {
        using division = division<Poly<Tp>,Poly<Tq>>;
        return std::get<0>(division::get(poly1,poly2));
    }

    template <typename Tp>
        requires (modulo<Tp,Tp>::has() != logic::none)
    auto operator%(Poly<Tp> const& poly, Tp const& c)
    {
        std::vector<Tp> coeffs;
        coeffs.reserve(poly.size());

        for (Tp const& coeff : poly.coeffs())
        {
            coeffs.push_back(modulo<Tp,Tp>::get(coeff,c));
        }
        return Poly<Tp>{coeffs};
    }

} // namespace mpp

/* ************************************************************************** */
// Standard Overloads
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
