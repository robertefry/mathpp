
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
        Mod(Tp const&);
        Mod(Tp const&, Tp const&);
        Mod(Tp const&, Tp&&);
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

    template <typename Tp, typename Op>
    struct identity<mpp::Mod<Tp>,Op>
    {
        constexpr static tristate has()
        {
            return mpp::identity<Tp,Op>::has();
        }
        static mpp::Mod<Tp> get(Tp const& mod)
        {
            return mpp::Mod<Tp>{mod,identity<Tp,Op>::get()};
        }
        static mpp::Mod<Tp>& make(mpp::Mod<Tp>& e)
        {
            return e = identity<Tp,Op>::get();
        }
    };

    template <typename Tp>
    struct inverse<mpp::Mod<Tp>,op_add>
    {
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(mpp::Mod<Tp> const&)
        {
            return true;
        }
        static mpp::Mod<Tp> get(mpp::Mod<Tp> const& e)
        {
            return -e;
        }
        static mpp::Mod<Tp>& make(mpp::Mod<Tp>& e)
        {
            return e *= mpp::inverse<Tp,op_add>::get(mpp::identity<Tp,op_mul>::get());
        }
    };

    template <typename Tp>
    struct inverse<mpp::Mod<Tp>,op_mul>
    {
        constexpr static tristate has()
        {
            return tristate::some;
        }
        constexpr static bool can(mpp::Mod<Tp> const& e)
        {
            return mpp::gcd<Tp>(e.value(),e.modulus()) == mpp::identity<Tp,op_mul>::get();
        }
        static mpp::Mod<Tp> get(mpp::Mod<Tp> const& e)
        {
            if (!can(e)) {
                throw std::runtime_error("mpp::Mod<Tp> must have gcd(val,mod)=1 for defined inverses.");
            }
            auto [x,y] = mpp::gcd_extended(e.value(),e.modulus());
            return mpp::Mod<Tp>{e.modulus(),x};
        }
        static mpp::Mod<Tp>& make(mpp::Mod<Tp>& e)
        {
            return e = get(e);
        }
    };

    template <typename Tp>
    struct absolute<mpp::Mod<Tp>,op_add>
    {
        constexpr static tristate has()
        {
            return absolute<Tp,op_add>::has();
        }
        constexpr static bool can(mpp::Mod<Tp> const& e)
        {
            return absolute<Tp,op_add>::can(e.value());
        }
        static mpp::Mod<Tp> get(mpp::Mod<Tp> const& e)
        {
            return e;
        }
        static mpp::Mod<Tp>& make(mpp::Mod<Tp>& e)
        {
            return e;
        }
    };

    template <typename Tp>
    struct absolute<mpp::Mod<Tp>,op_mul>
    {
        constexpr static tristate has()
        {
            return absolute<Tp,op_mul>::has();
        }
        constexpr static bool can(mpp::Mod<Tp> const& e)
        {
            return absolute<Tp,op_mul>::can(e.value());
        }
        static mpp::Mod<Tp> get(mpp::Mod<Tp> e)
        {
            auto copy = e;
            return make(copy);
        }
        static mpp::Mod<Tp>& make(mpp::Mod<Tp>& e)
        {
            return e = absolute<Tp,op_mul>::get(e.value());
        }
    };

    template <typename Tp, typename Tq>
        requires std::is_convertible<Tq,Tp>::value
    struct modulo<mpp::Mod<Tp>,mpp::Mod<Tq>>
    {
        constexpr static tristate has()
        {
            return tristate::all;
        }
        constexpr static bool can(mpp::Mod<Tp> const&, mpp::Mod<Tq> const&)
        {
            return true;
        }
        static mpp::Mod<Tp> get(mpp::Mod<Tp> const& e, mpp::Mod<Tq> const& n)
        {
            return mpp::Mod<Tp>{n.value(),e.value()};
        }
    };

    template <typename Tp, typename Tq>
        requires std::is_convertible<Tq,Tp>::value
    struct division<mpp::Mod<Tp>,mpp::Mod<Tq>>
    {
        constexpr static tristate has()
        {
            return mpp::division<Tp,Tq>::has();
        }
        constexpr static bool can(mpp::Mod<Tp> const& a, mpp::Mod<Tq> const& b)
        {
            return mpp::division<Tp,Tq>::can(a.value(),b.value());
        }
        static auto get(mpp::Mod<Tp> const& a, mpp::Mod<Tq> const& b)
        {
            auto const gcd = mpp::gcd<Tp>(a.modulus(),b.modulus());
            auto [r,s] = mpp::division<Tp,Tq>::get(a.value(),b.value());

            using Tr = decltype(r); using Ts = decltype(s);
            return std::make_tuple(mpp::Mod<Tr>{gcd,r},mpp::Mod<Ts>{gcd,s});
        }
    };

} // namespace mpp

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

template <typename Tp>
mpp::Mod<Tp>::Mod(Tp const& mod)
    : m_Modulus{mod}
    , m_Value{identity<Tp,op_add>::get()}
{
    validate();
}

template <typename Tp>
mpp::Mod<Tp>::Mod(Tp const& mod, Tp const& val)
    : m_Modulus{mod}
    , m_Value{val}
{
    validate();
}

template <typename Tp>
mpp::Mod<Tp>::Mod(Tp const& mod, Tp&& val)
    : m_Modulus{mod}
    , m_Value{std::move(val)}
{
    validate();
}

template <typename Tp>
template <typename Tq>
mpp::Mod<Tp>::Mod(Mod<Tq> const& other)
    : m_Modulus{other.m_Modulus}
    , m_Value{static_cast<Tp>(other.value())}
{
    validate();
}

template <typename Tp>
template <typename Tq>
mpp::Mod<Tp>::Mod(Tp const& mod, Mod<Tq> const& other)
    : m_Modulus{mod}
    , m_Value{static_cast<Tp>(other.value())}
{
    validate();
}

template <typename Tp>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator=(Tp const& val)
{
    m_Value = val;
    validate();
    return *this;
}

template <typename Tp>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator=(Tp&& val)
{
    m_Value = std::move(val);
    validate();
    return *this;
}

template <typename Tp>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator=(Mod<Tp> const& other)
{
    m_Value = other.m_Value;
    validate();
    return *this;
}

template <typename Tp>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator=(Mod<Tp>&& other)
{
    m_Value = std::move(other.m_Value);
    validate();
    return *this;
}

template <typename Tp>
template <typename Tq>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator=(Mod<Tq> const& other)
{
    m_Value = static_cast<Tp>(other.value());
    validate();
    return *this;
}

template <typename Tp>
void mpp::Mod<Tp>::validate()
{
    mpp::modulo<Tp,Tp>::make(m_Value,m_Modulus);
}

template <typename Tp>
void mpp::Mod<Tp>::swap(Mod<Tp>& other)
{
    std::swap(m_Value,other.m_Value);
    validate();
    other.validate();
}

template <typename Tp>
template <typename Tq>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator<<=(Tq const& n)
{
    m_Value <<= n;
    validate();
    return *this;
}

template <typename Tp>
template <typename Tq>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator>>=(Tq const& n)
{
    m_Value >>= n;
    validate();
    return *this;
}

template <typename Tp>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator++()
{
    m_Value += mpp::identity<Tp,op_mul>::get();
    validate();
    return *this;
}

template <typename Tp>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator--()
{
    m_Value -= mpp::identity<Tp,op_mul>::get();
    validate();
    return *this;
}

template <typename Tp>
mpp::Mod<Tp> mpp::Mod<Tp>::operator++(int)
{
    auto copy = *this;
    ++*this; // validates
    return copy;
}

template <typename Tp>
mpp::Mod<Tp> mpp::Mod<Tp>::operator--(int)
{
    auto copy = *this;
    --*this; // validates
    return copy;
}

template <typename Tp>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator+=(Tp const& num)
{
    m_Value += num;
    validate();
    return *this;
}

template <typename Tp>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator-=(Tp const& num)
{
    m_Value -= num;
    validate();
    return *this;
}

template <typename Tp>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator*=(Tp const& num)
{
    m_Value *= num;
    validate();
    return *this;
}

template <typename Tp>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator/=(Tp const& num)
{
    m_Value /= num;
    validate();
    return *this;
}

template <typename Tp>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator%=(Tp const& num)
{
    mpp::modulo<Tp,Tp>::make(m_Value,num);
    validate();
    return *this;
}

template <typename Tp>
template <typename Tq>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator+=(Mod<Tq> const& other)
{
    m_Value += static_cast<Tp>(other.value());
    validate();
    return *this;
}

template <typename Tp>
template <typename Tq>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator-=(Mod<Tq> const& other)
{
    m_Value -= static_cast<Tp>(other.value());
    validate();
    return *this;
}

template <typename Tp>
template <typename Tq>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator*=(Mod<Tq> const& other)
{
    m_Value *= static_cast<Tp>(other.value());
    validate();
    return *this;
}

template <typename Tp>
template <typename Tq>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator/=(Mod<Tq> const& other)
{
    m_Value /= static_cast<Tp>(other.value());
    validate();
    return *this;
}

template <typename Tp>
template <typename Tq>
mpp::Mod<Tp>& mpp::Mod<Tp>::operator%=(Mod<Tq> const& other)
{
    mpp::modulo<Tp,Tq>::make(m_Value,other.value());
    validate();
    return *this;
}

/* ************************************************************************** */
// External Extensions
/* ************************************************************************** */

template <typename Tp, typename Tq>
bool operator==(mpp::Mod<Tp> const& mod, Tq const& val)
{
    return mod.value() == val;
}

template <typename Tp, typename Tq>
bool operator<=>(mpp::Mod<Tp> const& mod, Tq const& val)
{
    return mod.value() <=> val;
}

template <typename Tp, typename Tq>
bool operator==(mpp::Mod<Tp> const& mod1, mpp::Mod<Tq> const& mod2)
{
    auto const gcd = mpp::gcd<Tp>(mod1.modulus(),mod2.modulus());
    auto const val1 = mpp::modulo<Tp,Tp>::get(mod1.value(),gcd);
    auto const val2 = mpp::modulo<Tp,Tp>::get(mod2.value(),gcd);
    return val1 == val2;
}

template <typename Tp, typename Tq>
auto operator<=>(mpp::Mod<Tp> const& mod1, mpp::Mod<Tq> const& mod2)
    -> std::compare_three_way_result_t<Tp,Tq>
{
    auto const gcd = mpp::gcd<Tp>(mod1.modulus(),mod2.modulus());
    auto const val1 = mpp::modulo<Tp,Tp>::get(mod1.value(),gcd);
    auto const val2 = mpp::modulo<Tp,Tp>::get(mod2.value(),gcd);
    return val1 <=> val2;
}

template <typename Tp, typename Tq>
auto operator<<(mpp::Mod<Tp> const& mod, Tq const& n)
{
    auto copy = mod;
    return copy <<= n;
}

template <typename Tp, typename Tq>
auto operator>>(mpp::Mod<Tp> const& mod, Tq const& n)
{
    auto copy = mod;
    return copy >>= n;
}

template <typename Tp>
auto operator+(mpp::Mod<Tp> const& mod)
{
    using mpp::op_add;  using mpp::op_mul;
    auto value = mpp::identity<Tp,op_add>::get() + mod.value();
    return mpp::Mod<Tp>{mod.modulus(),std::move(value)};
}

template <typename Tp>
auto operator-(mpp::Mod<Tp> const& mod)
{
    using mpp::op_add;  using mpp::op_mul;
    auto value = mpp::identity<Tp,op_add>::get() - mod.value();
    return mpp::Mod<Tp>{mod.modulus(),std::move(value)};
}

template <typename Tp>
auto operator+(mpp::Mod<Tp> const& mod, Tp const& c)
{
    auto value = mod.value() + c;
    return mpp::Mod<Tp>{mod.modulus(),std::move(value)};
}

template <typename Tp>
auto operator+(Tp const& c, mpp::Mod<Tp> const& mod)
{
    auto value = c + mod.value();
    return mpp::Mod<Tp>{mod.modulus(),std::move(value)};
}

template <typename Tp>
auto operator-(mpp::Mod<Tp> const& mod, Tp const& c)
{
    auto value = mod.value() - c;
    return mpp::Mod<Tp>{mod.modulus(),std::move(value)};
}

template <typename Tp>
auto operator-(Tp const& c, mpp::Mod<Tp> const& mod)
{
    auto value = c - mod.value();
    return mpp::Mod<Tp>{mod.modulus(),std::move(value)};
}

template <typename Tp>
auto operator*(mpp::Mod<Tp> const& mod, Tp const& c)
{
    auto value = mod.value() * c;
    return mpp::Mod<Tp>{mod.modulus(),std::move(value)};
}

template <typename Tp>
auto operator*(Tp const& c, mpp::Mod<Tp> const& mod)
{
    auto value = c * mod.value();
    return mpp::Mod<Tp>{mod.modulus(),std::move(value)};
}

template <typename Tp>
auto operator/(mpp::Mod<Tp> const& mod, Tp const& c)
{
    auto value = mod.value() / c;
    return mpp::Mod<Tp>{mod.modulus(),std::move(value)};
}

template <typename Tp>
auto operator/(Tp const& c, mpp::Mod<Tp> const& mod)
{
    auto value = c / mod.value();
    return mpp::Mod<Tp>{mod.modulus(),std::move(value)};
}

template <typename Tp>
auto operator%(mpp::Mod<Tp> const& mod, Tp const& c)
{
    auto value = mpp::modulo<Tp,Tp>::get(mod.value(),c);
    return mpp::Mod<Tp>{mod.modulus(),std::move(value)};
}

template <typename Tp>
auto operator%(Tp const& c, mpp::Mod<Tp> const& mod)
{
    auto value = mpp::modulo<Tp,Tp>::get(c,mod.value());
    return mpp::Mod<Tp>{mod.modulus(),std::move(value)};
}

template <typename Tp, typename Tq>
auto operator+(mpp::Mod<Tp> const& mod1, mpp::Mod<Tq> const& mod2)
{
    auto value = mod1.value() + mod2.value();

    using Tr = decltype(value);
    constexpr auto const gcd = mpp::gcd<Tr>(mod1.modulus(),mod2.modulus());
    return mpp::Mod<Tr>{gcd,std::move(value)};
}

template <typename Tp, typename Tq>
auto operator-(mpp::Mod<Tp> const& mod1, mpp::Mod<Tq> const& mod2)
{
    auto value = mod1.value() - mod2.value();

    using Tr = decltype(value);
    constexpr auto const gcd = mpp::gcd<Tr>(mod1.modulus(),mod2.modulus());
    return mpp::Mod<Tr>{gcd,std::move(value)};
}

template <typename Tp, typename Tq>
auto operator*(mpp::Mod<Tp> const& mod1, mpp::Mod<Tq> const& mod2)
{
    auto value = mod1.value() * mod2.value();

    using Tr = decltype(value);
    constexpr auto const gcd = mpp::gcd<Tr>(mod1.modulus(),mod2.modulus());
    return mpp::Mod<Tr>{gcd,std::move(value)};
}

template <typename Tp, typename Tq>
auto operator/(mpp::Mod<Tp> const& mod1, mpp::Mod<Tq> const& mod2)
{
    auto value = mod1.value() / mod2.value();

    using Tr = decltype(value);
    constexpr auto const gcd = mpp::gcd<Tr>(mod1.modulus(),mod2.modulus());
    return mpp::Mod<Tr>{gcd,std::move(value)};
}

template <typename Tp, typename Tq>
auto operator%(mpp::Mod<Tp> const& mod1, mpp::Mod<Tq> const& mod2)
{
    auto value = mod1.value() % mod2.value();

    using Tr = decltype(value);
    constexpr auto const gcd = mpp::gcd<Tr>(mod1.modulus(),mod2.modulus());
    return mpp::Mod<Tr>{gcd,std::move(value)};
}

/* ************************************************************************** */
// Standard Extensions
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
