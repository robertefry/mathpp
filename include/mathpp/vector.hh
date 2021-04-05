
#ifndef __HH_MPP_VECTOR
#define __HH_MPP_VECTOR

#include "mathpp/mathpp.hh"

#include <vector>

/* ************************************************************************** */
// Definitions
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp, size_t Nm, bool Vh>
    class VectorBase
    {
    public:
        explicit VectorBase();
        explicit VectorBase(Tp const&);
        virtual ~VectorBase() = default;

        template <typename... Args>
            requires (std::conjunction<std::is_convertible<Args,Tp>...>::value)
        VectorBase(Args const&... args)
            requires (sizeof...(args) == Nm);

        template <typename... Args>
            requires (std::conjunction<std::is_same<Args,Tp>...>::value)
        VectorBase(Args&&... args)
            requires (sizeof...(args) == Nm);

        template <typename Tq>
            requires (std::is_convertible<Tq,Tp>::value)
        VectorBase(VectorBase<Tq,Nm,Vh> const&);

        template <typename Tq>
            requires (std::is_assignable<Tq,Tp>::value)
        VectorBase<Tp,Nm,Vh>& operator=(VectorBase<Tq,Nm,Vh> const&);

        void swap(VectorBase<Tp,Nm,Vh>&);

    public:
        constexpr static auto size() { return Nm; }
        auto elements() const -> std::vector<Tp> const& { return m_Elements; }

        void assign(Tp const&);

        template <typename... Args>
            requires (std::conjunction<std::is_assignable<Args,Tp>...>::value)
        void assign(Args&&... args)
            requires (sizeof...(args) == Nm);

        auto operator[](size_t i) const -> Tp const& { return m_Elements[i]; }
        auto operator[](size_t i) -> Tp& { return m_Elements[i]; }
        auto at(size_t i) const -> Tp const& { return m_Elements.at(i); }
        auto at(size_t i) -> Tp& { return m_Elements.at(i); }

        template <typename Tq>
        VectorBase<Tp,Nm,Vh>& operator+=(VectorBase<Tq,Nm,Vh> const&);
        template <typename Tq>
        VectorBase<Tp,Nm,Vh>& operator-=(VectorBase<Tq,Nm,Vh> const&);
        template <typename Tq>
        VectorBase<Tp,Nm,Vh>& operator*=(Tq const&);
        template <typename Tq>
        VectorBase<Tp,Nm,Vh>& operator/=(Tq const&);
        template <typename Tq>
        VectorBase<Tp,Nm,Vh>& operator%=(Tq const&);

    private:
        std::vector<Tp> m_Elements{};
    };

    template <typename Tp, size_t Nm>
    using Vector = VectorBase<Tp,Nm,true>;

    template <typename Tp, size_t Nm>
    using CoVector = VectorBase<Tp,Nm,false>;

} // namespace mpp

/* ************************************************************************** */
// MathPP Specialisations
/* ************************************************************************** */

namespace mpp
{

    // identity

    template <typename Tp, size_t Nm, bool Vh>
    struct identity<VectorBase<Tp,Nm,Vh>,op_add>
    {
        constexpr static tristate has()
        {
            return identity<Tp,op_add>::has();
        }
        static VectorBase<Tp,Nm,Vh> const& get()
        {
            static auto const s_ident = VectorBase<Tp,Nm,Vh>{identity<Tp,op_add>::get()};
            return s_ident;
        }
        static VectorBase<Tp,Nm,Vh>& make(VectorBase<Tp,Nm,Vh>& vec)
        {
            return vec = get();
        }
    };

    // inverse

    template <typename Tp, size_t Nm, bool Vh>
    struct inverse<VectorBase<Tp,Nm,Vh>,op_add>
    {
        constexpr static tristate has()
        {
            return inverse<Tp,op_add>::has();
        }
        constexpr static bool can(VectorBase<Tp,Nm,Vh> const&)
        {
            return has() != logic::none;
        }
        static VectorBase<Tp,Nm,Vh> get(VectorBase<Tp,Nm,Vh> const& vec)
        {
            return -vec;
        }
        static VectorBase<Tp,Nm,Vh>& make(VectorBase<Tp,Nm,Vh>& vec)
        {
            return vec = get(vec);
        }
    };

} // namespace mpp

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp, size_t Nm, bool Vh>
    VectorBase<Tp,Nm,Vh>::VectorBase()
        : m_Elements(Nm,identity<Tp,op_add>::get())
    {
    }

    template <typename Tp, size_t Nm, bool Vh>
    VectorBase<Tp,Nm,Vh>::VectorBase(Tp const& value)
        : m_Elements(Nm,value)
    {
    }

    template <typename Tp, size_t Nm, bool Vh>
    template <typename... Args>
        requires (std::conjunction<std::is_convertible<Args,Tp>...>::value)
    VectorBase<Tp,Nm,Vh>::VectorBase(Args const&... args)
        requires (sizeof...(args) == Nm)
        : m_Elements{static_cast<Tp>(args)...}
    {
    }

    template <typename Tp, size_t Nm, bool Vh>
    template <typename... Args>
        requires (std::conjunction<std::is_same<Args,Tp>...>::value)
    VectorBase<Tp,Nm,Vh>::VectorBase(Args&&... args)
        requires (sizeof...(args) == Nm)
        : m_Elements{std::forward<Args>(args)...}
    {
    }

    template <typename Tp, size_t Nm, bool Vh>
    template <typename Tq>
        requires (std::is_convertible<Tq,Tp>::value)
    VectorBase<Tp,Nm,Vh>::VectorBase(VectorBase<Tq,Nm,Vh> const& other)
    {
        m_Elements.reserve(Nm);

        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements.push_back(static_cast<Tp>(other[i]));
        }
    }

    template <typename Tp, size_t Nm, bool Vh>
    template <typename Tq>
        requires (std::is_assignable<Tq,Tp>::value)
    VectorBase<Tp,Nm,Vh>& VectorBase<Tp,Nm,Vh>::operator=(VectorBase<Tq,Nm,Vh> const& other)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements[i] = other[i];
        }
    }

    template <typename Tp, size_t Nm, bool Vh>
    void VectorBase<Tp,Nm,Vh>::swap(VectorBase<Tp,Nm,Vh>& other)
    {
        std::swap(m_Elements,other.m_Elements);
    }

    template <typename Tp, size_t Nm, bool Vh>
    void VectorBase<Tp,Nm,Vh>::assign(Tp const& value)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements[i] = value;
        }
    }

    template <typename Tp, size_t Nm, bool Vh>
    template <typename... Args>
        requires (std::conjunction<std::is_assignable<Args,Tp>...>::value)
    void VectorBase<Tp,Nm,Vh>::assign(Args&&... args)
        requires (sizeof...(args) == Nm)
    {
        m_Elements = {std::forward<Args>(args)...};
    }

    template <typename Tp, size_t Nm, bool Vh>
    template <typename Tq>
    VectorBase<Tp,Nm,Vh>& VectorBase<Tp,Nm,Vh>::operator+=(VectorBase<Tq,Nm,Vh> const& other)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements[i] += other[i];
        }
        return *this;
    }

    template <typename Tp, size_t Nm, bool Vh>
    template <typename Tq>
    VectorBase<Tp,Nm,Vh>& VectorBase<Tp,Nm,Vh>::operator-=(VectorBase<Tq,Nm,Vh> const& other)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements[i] -= other[i];
        }
        return *this;
    }

    template <typename Tp, size_t Nm, bool Vh>
    template <typename Tq>
    VectorBase<Tp,Nm,Vh>& VectorBase<Tp,Nm,Vh>::operator*=(Tq const& scalar)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements[i] *= scalar;
        }
        return *this;
    }

    template <typename Tp, size_t Nm, bool Vh>
    template <typename Tq>
    VectorBase<Tp,Nm,Vh>& VectorBase<Tp,Nm,Vh>::operator/=(Tq const& scalar)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements[i] /= scalar;
        }
        return *this;
    }

    template <typename Tp, size_t Nm, bool Vh>
    template <typename Tq>
    VectorBase<Tp,Nm,Vh>& VectorBase<Tp,Nm,Vh>::operator%=(Tq const& scalar)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            modulo<Tp,Tq>::make(m_Elements[i],scalar);
        }
        return *this;
    }

} // namespace mpp

/* ************************************************************************** */
// Non-Member Extensions
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires requires (Tp a, Tq b) { a != b; }
    bool operator==(VectorBase<Tp,Nm,Vh> const& vec1, VectorBase<Tq,Nm,Vh> const& vec2)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            if (vec1[i] != vec2[i]) return false;
        }
        return true;
    }

    template <typename Tp, size_t Nm, bool Vh>
        requires requires (Tp a, Tp b) { a + b; }
    auto operator+(VectorBase<Tp,Nm,Vh> const& vec)
    {
        auto result = VectorBase<Tp,Nm,Vh>{identity<Tp,op_add>::get()};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] += vec[i];
        }
        return result;
    }

    template <typename Tp, size_t Nm, bool Vh>
        requires requires (Tp a, Tp b) { a - b; }
    auto operator-(VectorBase<Tp,Nm,Vh> const& vec)
    {
        auto result = VectorBase<Tp,Nm,Vh>{identity<Tp,op_add>::get()};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] -= vec[i];
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires requires (Tp a, Tq b) { a + b; }
    auto operator+(VectorBase<Tp,Nm,Vh> const& vec1, VectorBase<Tq,Nm,Vh> const& vec2)
    {
        auto result = VectorBase<Tp,Nm,Vh>{vec1};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] += vec2[i];
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires requires (Tp a, Tq b) { a - b; }
    auto operator-(VectorBase<Tp,Nm,Vh> const& vec1, VectorBase<Tq,Nm,Vh> const& vec2)
    {
        auto result = VectorBase<Tp,Nm,Vh>{vec1};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] -= vec2[i];
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires requires (Tp a, Tq b) { a * b; }
    auto operator*(VectorBase<Tp,Nm,Vh> const& vec, Tq const& scalar)
    {
        auto result = VectorBase<Tp,Nm,Vh>{vec};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] *= scalar;
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires requires (Tp a, Tq b) { b * a; }
    auto operator*(Tq const& scalar, VectorBase<Tp,Nm,Vh> const& vec)
    {
        auto result = VectorBase<Tp,Nm,Vh>{vec};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] *= scalar;
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires requires (Tp a, Tq b) { a / b; }
    auto operator/(VectorBase<Tp,Nm,Vh> const& vec, Tq const& scalar)
    {
        auto result = VectorBase<Tp,Nm,Vh>{vec};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] /= scalar;
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires (modulo<Tp,Tq>::has() != logic::none)
    auto operator%(VectorBase<Tp,Nm,Vh> const& vec, Tq const& scalar)
    {
        auto result = VectorBase<Tp,Nm,Vh>{vec};

        for (size_t i = 0; i < Nm; ++i)
        {
            modulo<Tp,Tq>::make(result[i],scalar);
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm>
        requires requires (Tp a, Tq b) { a + b; a * b; }
            && (identity<typename op_mul::result<Tp,Tq>::type,op_add>::has() != logic::none)
    auto operator*(CoVector<Tp,Nm> const& covec, Vector<Tq,Nm> const& vec)
    {
        auto result = identity<typename op_mul::result<Tp,Tq>::type,op_add>::get();

        for (size_t i = 0; i < Nm; ++i)
        {
            result += covec[i] * vec[i];
        }
        return result;
    }

} // namespace mpp

/* ************************************************************************** */
// Standard Overloads
/* ************************************************************************** */

namespace std
{

    template <typename Tp, size_t Nm, bool Vh>
    void swap(mpp::VectorBase<Tp,Nm,Vh> const& vec1, mpp::VectorBase<Tp,Nm,Vh> const& vec2)
    {
        vec1.swap(vec2);
    }

} // namespace mpp

#endif /* __HH_MPP_VECTOR */
