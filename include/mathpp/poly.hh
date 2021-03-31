
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
        virtual auto coeffs() const -> std::vector<Tp> const& { return m_Coefficients; }
        virtual auto order() const -> size_t { return m_Coefficients.size()-1; }
        virtual auto size() const -> size_t { return m_Coefficients.size(); }

        virtual void swap(Poly<Tp>&);
        virtual void assign(size_t, Tp const&);
        virtual void assign(std::initializer_list<Tp>);
        virtual void assign(std::vector<Tp> const&);
        virtual void assign(std::vector<Tp>&&);
        virtual void zero();

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
    struct identity<Poly<Tp>,Op>
    {
        constexpr static tristate has()
        {
            return identity<Tp,Op>::has();
        }
        static Poly<Tp> const& get()
        {
            static auto const s_ident = Poly<Tp>{identity<Tp,Op>::get()};
            return s_ident;
        }
        static Poly<Tp>& make(Poly<Tp>& poly)
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
        constexpr static bool can(Poly<Tp> const& e)
        {
            for (auto const& coeff : e.coeffs())
            {
                if (inverse<Tp,op_add>::can(coeff) == false) return false;
            }
            return true;
        }
        static Poly<Tp> get(Poly<Tp> const& e)
        {
            return -e;
        }
        static Poly<Tp>& make(Poly<Tp>& e)
        {
            return e *= inverse<Tp,op_add>::get(identity<Tp,op_mul>::get());
        }
    };

    // absolute

    template <typename Tp>
    struct absolute<Poly<Tp>,op_add>
    {
        constexpr static tristate has()
        {
            return logic::all;
        }
        constexpr static bool can(Poly<Tp> const&)
        {
            return true;
        }
        static Poly<Tp> get(Poly<Tp> const& e)
        {
            auto const negative = e.coeffs().back() < identity<Tp,op_add>::get();
            return negative ? -e : e;
        }
        static Poly<Tp>& make(Poly<Tp>& e)
        {
            auto const negative = e.coeffs().back() < identity<Tp,op_add>::get();
            if (negative) e *= inverse<Tp,op_add>::get(identity<Tp,op_mul>::get());
            return e;
        }
    };

    // modulo

    template <typename Tp, typename Tq>
        requires std::is_convertible<Tq,Poly<Tp>>::value
    struct modulo<Poly<Tp>,Tq>
    {
        constexpr static tristate has()
        {
            return logic::all;
        }
        constexpr static bool can(Poly<Tp> const&, Tq const&)
        {
            return true;
        }
        static Poly<Tp> get(Poly<Tp> const& e, Tq const& n)
        {
            return e % n;
        }
        static Poly<Tp>& make(Poly<Tp>& e, Tq const& n)
        {
            return e %= n;
        }
    };

    template <typename Tp, typename Tq>
        requires std::is_convertible<Tq,Tp>::value
    struct modulo<Poly<Tp>,Poly<Tq>>
    {
        constexpr static tristate has()
        {
            return logic::all;
        }
        constexpr static bool can(Poly<Tp> const&, Poly<Tq> const&)
        {
            return true;
        }
        static Poly<Tp> get(Poly<Tp> const& e, Poly<Tq> const& n)
        {
            return std::get<0>(division<Poly<Tp>,Poly<Tq>>::get(e,n));
        }
        static Poly<Tp>& make(Poly<Tp>& e, Poly<Tq> const& n)
        {
            return e = get(e,n);
        }
    };

    // division

    template <typename Tp, typename Tq>
        requires std::is_convertible<Tq,Tp>::value
    struct division<Poly<Tp>,Tq>
    {
        constexpr static tristate has()
        {
            return logic::all;
        }
        constexpr static bool can(Poly<Tp> const&, Tq const&)
        {
            return true;
        }
        static Poly<Tp> get(Poly<Tp> const& e, Tq const& c)
        {
            return e / c;
        }
        static Poly<Tp>& make(Poly<Tp>& e, Tq const& c)
        {
            return e /= c;
        }
    };

    template <typename Tp, typename Tq>
        requires std::is_convertible<Tq,Tp>::value
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
        static auto get(Poly<Tp> const& poly1, Poly<Tq> const& poly2)
        {
            using Tr = op_mul::result<Tp,Tq>::type;
            Poly<Tr> remainder = poly1;
            Poly<Tr> quotient;

            using abs = absolute<Poly<Tp>,op_add>;
            while (abs::get(remainder) >= abs::get(poly2))
            {
                auto const coeff = remainder.coeffs().back() / poly2.coeffs().back();
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
        auto const pred = [](auto const& coeff) { return coeff != identity<Tp,op_add>::get(); };
        auto const itr = std::find_if(m_Coefficients.rbegin(), m_Coefficients.rend(), pred);
        m_Coefficients.erase(itr.base(), m_Coefficients.end());
        if (m_Coefficients.size() == 0) m_Coefficients.push_back(identity<Tp,op_add>::get());
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
    void Poly<Tp>::zero()
    {
        m_Coefficients.clear();
        validate();
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
    Poly<Tp>& Poly<Tp>::operator/=(Poly<Tq> const& other)
    {
        Poly<Tp> quotient;

        using abs = absolute<Poly<Tp>,op_add>;
        while (abs::get(*this) >= abs::get(other))
        {
            auto const coeff = coeffs().back() / other.coeffs().back();
            auto const power = order() - other.order();
            auto const term = Poly<Tp>{coeff} <<= power;
            quotient += term;
            *this -= term * other;
        }
        return *this = std::move(quotient);
    }

    template <typename Tp>
    template <typename Tq>
    Poly<Tp>& Poly<Tp>::operator%=(Poly<Tq> const& other)
    {
        using abs = absolute<Poly<Tp>,op_add>;
        while (abs::get(*this) >= abs::get(other))
        {
            auto const coeff = coeffs().back() / other.coeffs().back();
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
    auto operator<=>(Poly<Tp> const& poly1, Poly<Tq> const& poly2)
        -> std::compare_three_way_result_t<Tp,Tq>
    {
        if (poly1.order() > poly2.order()) return poly1.coeffs().back() <=> identity<Tp,op_add>::get();
        if (poly1.order() < poly2.order()) return identity<Tp,op_add>::get() <=> poly2.coeffs().back();

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
    auto operator+(Poly<Tp> const& poly, Tp const& c)
    {
        std::vector<Tp> coeffs = poly.coeffs();
        coeffs[0] = coeffs[0] + c;
        return Poly<Tp>{coeffs};
    }

    template <typename Tp>
    auto operator+(Tp const& c, Poly<Tp> const& poly)
    {
        std::vector<Tp> coeffs = poly.coeffs();
        coeffs[0] = c + coeffs[0];
        return Poly<Tp>{coeffs};
    }

    template <typename Tp, typename Tq>
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
    auto operator-(Poly<Tp> const& poly, Tp const& c)
    {
        std::vector<Tp> coeffs = poly.coeffs();
        coeffs[0] = coeffs[0] - c;
        return Poly<Tp>{coeffs};
    }

    template <typename Tp>
    auto operator-(Tp const& c, Poly<Tp> const& poly)
    {
        std::vector<Tp> coeffs = poly.coeffs();
        coeffs[0] = c - coeffs[0];
        return Poly<Tp>{coeffs};
    }

    template <typename Tp, typename Tq>
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

    template <typename Tp, typename Tq>
    auto operator/(Poly<Tp> const& poly1, Poly<Tq> const& poly2)
    {
        using division = division<Poly<Tp>,Poly<Tq>>;
        return std::get<1>(division::get(poly1,poly2));
    }

    template <typename Tp>
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
    auto operator%(Poly<Tp> const& poly1, Poly<Tq> const& poly2)
    {
        using division = division<Poly<Tp>,Poly<Tq>>;
        return std::get<0>(division::get(poly1,poly2));
    }

    template <typename Tp>
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
