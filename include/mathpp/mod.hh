
#ifndef __HH_MPP_MOD
#define __HH_MPP_MOD

#include "mathpp/mathpp.hh"
#include "mathpp/gcd.hh"

#include <stdexcept>
#include <compare>

/* ************************************************************************** */
// Definitions
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp>
    class Mod
    {
    public:
        explicit Mod(Tp const&);
        explicit Mod(Tp const&, Tp const&);
        explicit Mod(Tp const&, Tp&&);
        virtual ~Mod() = default;

        template <typename Tq>
        Mod(Mod<Tq> const&);
        template <typename Tq>
        Mod(Tp const&, Mod<Tq> const&);

        Mod<Tp>& operator=(Tp const&);
        Mod<Tp>& operator=(Tp&&);

        template <typename Tq>
        Mod<Tp>& operator=(Mod<Tq> const&);

        Mod(Mod<Tp> const&) = default;
        Mod<Tp>& operator=(Mod<Tp> const&);

        Mod(Mod<Tp>&&) = default;
        Mod<Tp>& operator=(Mod<Tp>&&);

    protected:
        virtual void validate();

    public:
        virtual auto modulus() const -> Tp const& { return m_Modulus; }
        virtual auto value() const -> Tp const& { return m_Value; }

        virtual void swap(Mod<Tp>&);

        template <typename Tq>
        Mod<Tp>& operator<<=(Tq const&);
        template <typename Tq>
        Mod<Tp>& operator>>=(Tq const&);

        virtual Mod<Tp>& operator++();
        virtual Mod<Tp>& operator--();
        virtual Mod<Tp> operator++(int);
        virtual Mod<Tp> operator--(int);

        virtual Mod<Tp>& operator+=(Tp const&);
        virtual Mod<Tp>& operator-=(Tp const&);
        virtual Mod<Tp>& operator*=(Tp const&);
        virtual Mod<Tp>& operator/=(Tp const&);
        virtual Mod<Tp>& operator%=(Tp const&);

        template <typename Tq>
        Mod<Tp>& operator+=(Mod<Tq> const&);
        template <typename Tq>
        Mod<Tp>& operator-=(Mod<Tq> const&);
        template <typename Tq>
        Mod<Tp>& operator*=(Mod<Tq> const&);
        template <typename Tq>
        Mod<Tp>& operator/=(Mod<Tq> const&);
        template <typename Tq>
        Mod<Tp>& operator%=(Mod<Tq> const&);

    protected:
        Tp const m_Modulus{};
        Tp m_Value{};
    };

} // namespace mpp

/* ************************************************************************** */
// MathPP Specialisations
/* ************************************************************************** */

namespace mpp
{

    // identity

    template <typename Tp, typename Op>
    struct identity<Mod<Tp>,Op>
    {
        constexpr static tristate has()
        {
            return identity<Tp,Op>::has();
        }
        static Mod<Tp> const& get(Tp const& mod)
        {
            static auto const s_ident = Mod<Tp>{mod,identity<Tp,Op>::get()};
            return s_ident;
        }
        static Mod<Tp>& make(Mod<Tp>& e)
        {
            return e = identity<Tp,Op>::get();
        }
    };

    // inverse

    template <typename Tp>
    struct inverse<Mod<Tp>,op_mul>
    {
        constexpr static tristate has()
        {
            return logic::some;
        }
        constexpr static bool can(Mod<Tp> const& e)
        {
            return gcd<Tp>(e.value(),e.modulus()) == identity<Tp,op_mul>::get();
        }
        static Mod<Tp> get(Mod<Tp> const& e)
        {
            if (!can(e)) {
                throw std::runtime_error("Mod<Tp> must have gcd(val,mod)=1 for defined inverses.");
            }
            auto [x,y] = gcd_extended(e.value(),e.modulus());
            return Mod<Tp>{e.modulus(),x};
        }
        static Mod<Tp>& make(Mod<Tp>& e)
        {
            return e = get(e);
        }
    };

    // absolute

    template <typename Tp, typename Op>
    struct absolute<Mod<Tp>,Op>
    {
        constexpr static tristate has()
        {
            return absolute<Tp,Op>::has();
        }
        constexpr static bool can(Mod<Tp> const& e)
        {
            return absolute<Tp,Op>::can(e.value());
        }
        static Mod<Tp> get(Mod<Tp> const& e)
        {
            return Mod<Tp>{e.modulus(),absolute<Tp,Op>::get(e.value())};
        }
        static Mod<Tp>& make(Mod<Tp>& e)
        {
            return e = absolute<Tp,Op>::get(e.value());
        }
    };

} // namespace mpp

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp>
    Mod<Tp>::Mod(Tp const& mod)
        : m_Modulus{mod}
        , m_Value{identity<Tp,op_add>::get()}
    {
        validate();
    }

    template <typename Tp>
    Mod<Tp>::Mod(Tp const& mod, Tp const& val)
        : m_Modulus{mod}
        , m_Value{val}
    {
        validate();
    }

    template <typename Tp>
    Mod<Tp>::Mod(Tp const& mod, Tp&& val)
        : m_Modulus{mod}
        , m_Value{std::move(val)}
    {
        validate();
    }

    template <typename Tp>
    template <typename Tq>
    Mod<Tp>::Mod(Mod<Tq> const& other)
        : m_Modulus{other.m_Modulus}
        , m_Value{static_cast<Tp>(other.value())}
    {
        validate();
    }

    template <typename Tp>
    template <typename Tq>
    Mod<Tp>::Mod(Tp const& mod, Mod<Tq> const& other)
        : m_Modulus{mod}
        , m_Value{static_cast<Tp>(other.value())}
    {
        validate();
    }

    template <typename Tp>
    Mod<Tp>& Mod<Tp>::operator=(Tp const& val)
    {
        m_Value = val;
        validate();
        return *this;
    }

    template <typename Tp>
    Mod<Tp>& Mod<Tp>::operator=(Tp&& val)
    {
        m_Value = std::move(val);
        validate();
        return *this;
    }

    template <typename Tp>
    Mod<Tp>& Mod<Tp>::operator=(Mod<Tp> const& other)
    {
        m_Value = other.m_Value;
        validate();
        return *this;
    }

    template <typename Tp>
    Mod<Tp>& Mod<Tp>::operator=(Mod<Tp>&& other)
    {
        m_Value = std::move(other.m_Value);
        validate();
        return *this;
    }

    template <typename Tp>
    template <typename Tq>
    Mod<Tp>& Mod<Tp>::operator=(Mod<Tq> const& other)
    {
        m_Value = static_cast<Tp>(other.value());
        validate();
        return *this;
    }

    template <typename Tp>
    void Mod<Tp>::validate()
    {
        modulo<Tp,Tp>::make(m_Value,m_Modulus);
    }

    template <typename Tp>
    void Mod<Tp>::swap(Mod<Tp>& other)
    {
        std::swap(m_Value,other.m_Value);
        validate();
        other.validate();
    }

    template <typename Tp>
    template <typename Tq>
    Mod<Tp>& Mod<Tp>::operator<<=(Tq const& n)
    {
        m_Value <<= n;
        validate();
        return *this;
    }

    template <typename Tp>
    template <typename Tq>
    Mod<Tp>& Mod<Tp>::operator>>=(Tq const& n)
    {
        m_Value >>= n;
        validate();
        return *this;
    }

    template <typename Tp>
    Mod<Tp>& Mod<Tp>::operator++()
    {
        m_Value += identity<Tp,op_mul>::get();
        validate();
        return *this;
    }

    template <typename Tp>
    Mod<Tp>& Mod<Tp>::operator--()
    {
        m_Value -= identity<Tp,op_mul>::get();
        validate();
        return *this;
    }

    template <typename Tp>
    Mod<Tp> Mod<Tp>::operator++(int)
    {
        auto copy = *this;
        ++*this; // validates
        return copy;
    }

    template <typename Tp>
    Mod<Tp> Mod<Tp>::operator--(int)
    {
        auto copy = *this;
        --*this; // validates
        return copy;
    }

    template <typename Tp>
    Mod<Tp>& Mod<Tp>::operator+=(Tp const& num)
    {
        m_Value += num;
        validate();
        return *this;
    }

    template <typename Tp>
    Mod<Tp>& Mod<Tp>::operator-=(Tp const& num)
    {
        m_Value -= num;
        validate();
        return *this;
    }

    template <typename Tp>
    Mod<Tp>& Mod<Tp>::operator*=(Tp const& num)
    {
        m_Value *= num;
        validate();
        return *this;
    }

    template <typename Tp>
    Mod<Tp>& Mod<Tp>::operator/=(Tp const& num)
    {
        m_Value /= num;
        validate();
        return *this;
    }

    template <typename Tp>
    Mod<Tp>& Mod<Tp>::operator%=(Tp const& num)
    {
        modulo<Tp,Tp>::make(m_Value,num);
        validate();
        return *this;
    }

    template <typename Tp>
    template <typename Tq>
    Mod<Tp>& Mod<Tp>::operator+=(Mod<Tq> const& other)
    {
        m_Value += static_cast<Tp>(other.value());
        validate();
        return *this;
    }

    template <typename Tp>
    template <typename Tq>
    Mod<Tp>& Mod<Tp>::operator-=(Mod<Tq> const& other)
    {
        m_Value -= static_cast<Tp>(other.value());
        validate();
        return *this;
    }

    template <typename Tp>
    template <typename Tq>
    Mod<Tp>& Mod<Tp>::operator*=(Mod<Tq> const& other)
    {
        m_Value *= static_cast<Tp>(other.value());
        validate();
        return *this;
    }

    template <typename Tp>
    template <typename Tq>
    Mod<Tp>& Mod<Tp>::operator/=(Mod<Tq> const& other)
    {
        m_Value /= static_cast<Tp>(other.value());
        validate();
        return *this;
    }

    template <typename Tp>
    template <typename Tq>
    Mod<Tp>& Mod<Tp>::operator%=(Mod<Tq> const& other)
    {
        modulo<Tp,Tq>::make(m_Value,other.value());
        validate();
        return *this;
    }

} // namespace mpp

/* ************************************************************************** */
// Non-Member Extensions
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp, typename Tq>
    bool operator==(Mod<Tp> const& mod1, Mod<Tq> const& mod2)
    {
        auto const d = gcd<Tp>(mod1.modulus(),mod2.modulus());
        auto const val1 = modulo<Tp,Tp>::get(mod1.value(),d);
        auto const val2 = modulo<Tp,Tp>::get(mod2.value(),d);
        return val1 == val2;
    }

    template <typename Tp, typename Tq>
    bool operator==(Mod<Tp> const& mod, Tq const& val)
    {
        return mod.value() == val;
    }

    template <typename Tp, typename Tq>
    bool operator==(Tq const& val, Mod<Tp> const& mod)
    {
        return val == mod.value();
    }

    template <typename Tp, typename Tq>
    auto operator<=>(Mod<Tp> const& mod1, Mod<Tq> const& mod2)
        -> std::compare_three_way_result_t<Tp,Tq>
    {
        auto const d = gcd<Tp>(mod1.modulus(),mod2.modulus());
        auto const val1 = modulo<Tp,Tp>::get(mod1.value(),d);
        auto const val2 = modulo<Tp,Tp>::get(mod2.value(),d);
        return val1 <=> val2;
    }

    template <typename Tp, typename Tq>
    auto operator<=>(Mod<Tp> const& mod, Tq const& val)
        -> std::compare_three_way_result_t<Tp,Tq>
    {
        return mod.value() <=> val;
    }

    template <typename Tp, typename Tq>
    auto operator<=>(Tq const& val, Mod<Tp> const& mod)
        -> std::compare_three_way_result_t<Tp,Tq>
    {
        return val <=> mod.value();
    }

    template <typename Tp, typename Tq>
    auto operator<<(Mod<Tp> const& mod, Tq const& n)
    {
        auto copy = mod;
        return copy <<= n;
    }

    template <typename Tp, typename Tq>
    auto operator>>(Mod<Tp> const& mod, Tq const& n)
    {
        auto copy = mod;
        return copy >>= n;
    }

    template <typename Tp>
    auto operator+(Mod<Tp> const& mod)
    {
        auto value = identity<Tp,op_add>::get() + mod.value();
        return Mod<Tp>{mod.modulus(),std::move(value)};
    }

    template <typename Tp>
    auto operator-(Mod<Tp> const& mod)
    {
        auto value = identity<Tp,op_add>::get() - mod.value();
        return Mod<Tp>{mod.modulus(),std::move(value)};
    }

    template <typename Tp, typename Tq>
    auto operator+(Mod<Tp> const& mod1, Mod<Tq> const& mod2)
    {
        using Tr = op_add::result<Tp,Tq>::type;
        auto value = mod1.value() + mod2.value();
        auto const d = gcd<Tr>(mod1.modulus(),mod2.modulus());
        return Mod<Tr>{d,std::move(value)};
    }

    template <typename Tp>
    auto operator+(Mod<Tp> const& mod, Tp const& c)
    {
        auto value = mod.value() + c;
        return Mod<Tp>{mod.modulus(),std::move(value)};
    }

    template <typename Tp>
    auto operator+(Tp const& c, Mod<Tp> const& mod)
    {
        auto value = c + mod.value();
        return Mod<Tp>{mod.modulus(),std::move(value)};
    }

    template <typename Tp, typename Tq>
    auto operator-(Mod<Tp> const& mod1, Mod<Tq> const& mod2)
    {
        using Tr = op_add::result<Tp,Tq>::type;
        auto value = mod1.value() - mod2.value();
        auto const d = gcd<Tr>(mod1.modulus(),mod2.modulus());
        return Mod<Tr>{d,std::move(value)};
    }

    template <typename Tp>
    auto operator-(Mod<Tp> const& mod, Tp const& c)
    {
        auto value = mod.value() - c;
        return Mod<Tp>{mod.modulus(),std::move(value)};
    }

    template <typename Tp>
    auto operator-(Tp const& c, Mod<Tp> const& mod)
    {
        auto value = c - mod.value();
        return Mod<Tp>{mod.modulus(),std::move(value)};
    }

    template <typename Tp, typename Tq>
    auto operator*(Mod<Tp> const& mod1, Mod<Tq> const& mod2)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        auto value = mod1.value() * mod2.value();
        auto const d = gcd<Tr>(mod1.modulus(),mod2.modulus());
        return Mod<Tr>{d,std::move(value)};
    }

    template <typename Tp>
    auto operator*(Mod<Tp> const& mod, Tp const& c)
    {
        auto value = mod.value() * c;
        return Mod<Tp>{mod.modulus(),std::move(value)};
    }

    template <typename Tp>
    auto operator*(Tp const& c, Mod<Tp> const& mod)
    {
        auto value = c * mod.value();
        return Mod<Tp>{mod.modulus(),std::move(value)};
    }

    template <typename Tp, typename Tq>
    auto operator/(Mod<Tp> const& mod1, Mod<Tq> const& mod2)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        auto value = mod1.value() / mod2.value();
        auto const d = gcd<Tr>(mod1.modulus(),mod2.modulus());
        return Mod<Tr>{d,std::move(value)};
    }

    template <typename Tp>
    auto operator/(Mod<Tp> const& mod, Tp const& c)
    {
        auto value = mod.value() / c;
        return Mod<Tp>{mod.modulus(),std::move(value)};
    }

    template <typename Tp>
    auto operator/(Tp const& c, Mod<Tp> const& mod)
    {
        auto value = c / mod.value();
        return Mod<Tp>{mod.modulus(),std::move(value)};
    }

    template <typename Tp, typename Tq>
    auto operator%(Mod<Tp> const& mod1, Mod<Tq> const& mod2)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        auto value = mod1.value() % mod2.value();
        auto const d = gcd<Tr>(mod1.modulus(),mod2.modulus());
        return Mod<Tr>{d,std::move(value)};
    }

    template <typename Tp>
    auto operator%(Mod<Tp> const& mod, Tp const& c)
    {
        auto value = modulo<Tp,Tp>::get(mod.value(),c);
        return Mod<Tp>{mod.modulus(),std::move(value)};
    }

    template <typename Tp>
    auto operator%(Tp const& c, Mod<Tp> const& mod)
    {
        auto value = modulo<Tp,Tp>::get(c,mod.value());
        return Mod<Tp>{mod.modulus(),std::move(value)};
    }

} // namespace mpp

/* ************************************************************************** */
// Standard Overloads
/* ************************************************************************** */

namespace std
{

    template <typename Tp>
    void swap(mpp::Mod<Tp>& mod1, mpp::Mod<Tp>& mod2)
    {
        mod1.swap(mod2);
    }

} // namespace std

#endif /* __HH_MPP_MOD */
