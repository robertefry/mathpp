
#ifndef __HH_MPP_VECTOR
#define __HH_MPP_VECTOR

#include "mathpp/mathpp.hh"

#include <vector>
#include <span>

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
        explicit VectorBase(std::span<Tp,Nm> const&);
        virtual ~VectorBase() = default;

        template <typename... Args>
            requires (std::conjunction<std::is_convertible<Args,Tp>...>::value)
                && (sizeof...(Args) == Nm)
        VectorBase(Args const&... args);

        template <typename... Args>
            requires (std::conjunction<std::is_same<Args,Tp>...>::value)
                && (sizeof...(Args) == Nm)
        VectorBase(Args&&... args);

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
                && (sizeof...(Args) == Nm)
        void assign(Args&&... args);

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
        static VectorBase<Tp,Nm,Vh>& make(VectorBase<Tp,Nm,Vh>& vector)
        {
            return vector = get();
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
        static VectorBase<Tp,Nm,Vh> get(VectorBase<Tp,Nm,Vh> const& vector)
        {
            return -vector;
        }
        static VectorBase<Tp,Nm,Vh>& make(VectorBase<Tp,Nm,Vh>& vector)
        {
            return vector = get(vector);
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
    VectorBase<Tp,Nm,Vh>::VectorBase(std::span<Tp,Nm> const& span)
        : m_Elements(span.begin(),span.end())
    {
    }

    template <typename Tp, size_t Nm, bool Vh>
    template <typename... Args>
        requires (std::conjunction<std::is_convertible<Args,Tp>...>::value)
            && (sizeof...(Args) == Nm)
    VectorBase<Tp,Nm,Vh>::VectorBase(Args const&... args)
        : m_Elements{static_cast<Tp>(args)...}
    {
    }

    template <typename Tp, size_t Nm, bool Vh>
    template <typename... Args>
        requires (std::conjunction<std::is_same<Args,Tp>...>::value)
            && (sizeof...(Args) == Nm)
    VectorBase<Tp,Nm,Vh>::VectorBase(Args&&... args)
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
        return *this;
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
            && (sizeof...(Args) == Nm)
    void VectorBase<Tp,Nm,Vh>::assign(Args&&... args)
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

    template <typename Tp1, size_t Nm1, bool Vh1, typename Tp2, size_t Nm2, bool Vh2>
        requires requires (Tp1 a, Tp2 b) { a != b; }
    bool operator==(VectorBase<Tp1,Nm1,Vh1> const& vector1, VectorBase<Tp2,Nm2,Vh2> const& vector2)
    {
        if constexpr (Nm1 != Nm2) {
            return false;
        }
        for (size_t i = 0; i < Nm1; ++i)
        {
            if (vector1[i] != vector2[i]) return false;
        }
        return true;
    }

    template <typename Tp, size_t Nm, bool Vh>
        requires (identity<VectorBase<Tp,Nm,Vh>,op_add>::has() != logic::none)
    auto operator+(VectorBase<Tp,Nm,Vh> const& vector)
    {
        VectorBase<Tp,Nm,Vh> result = identity<VectorBase<Tp,Nm,Vh>,op_add>::get();
        return result + vector;
    }

    template <typename Tp, size_t Nm, bool Vh>
        requires (identity<VectorBase<Tp,Nm,Vh>,op_add>::has() != logic::none)
    auto operator-(VectorBase<Tp,Nm,Vh> const& vector)
    {
        VectorBase<Tp,Nm,Vh> result = identity<VectorBase<Tp,Nm,Vh>,op_add>::get();
        return result - vector;
    }

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires requires (Tp a, Tq b) { a + b; }
    auto operator+(VectorBase<Tp,Nm,Vh> const& vector1, VectorBase<Tq,Nm,Vh> const& vector2)
    {
        using Tr = op_add::result<Tp,Tq>::type;
        VectorBase<Tr,Nm,Vh> result = vector1;
        return result += vector2;
    }

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires requires (Tp a, Tq b) { a - b; }
    auto operator-(VectorBase<Tp,Nm,Vh> const& vector1, VectorBase<Tq,Nm,Vh> const& vector2)
    {
        using Tr = op_add::result<Tp,Tq>::type;
        VectorBase<Tr,Nm,Vh> result = vector1;
        return result -= vector2;
    }

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires requires (Tp a, Tq b) { a * b; }
    auto operator*(VectorBase<Tp,Nm,Vh> const& vector, Tq const& scalar)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        std::vector<Tr> elements;
        elements.reserve(Nm);

        for (size_t i = 0; i < Nm; ++i)
        {
            auto element = vector[i] * scalar;
            elements.push_back(std::move(element));
        }
        std::span<Tr,Nm> span {elements.begin(),elements.end()};
        return VectorBase<Tr,Nm,Vh>{span};
    }

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires requires (Tp a, Tq b) { b * a; }
    auto operator*(Tq const& scalar, VectorBase<Tp,Nm,Vh> const& vector)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        std::vector<Tr> elements;
        elements.reserve(Nm);

        for (size_t i = 0; i < Nm; ++i)
        {
            auto element = scalar * vector[i];
            elements.push_back(std::move(element));
        }
        std::span<Tr,Nm> span {elements.begin(),elements.end()};
        return VectorBase<Tr,Nm,Vh>{span};
    }

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires requires (Tp a, Tq b) { a / b; }
    auto operator/(VectorBase<Tp,Nm,Vh> const& vector, Tq const& scalar)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        std::vector<Tr> elements;
        elements.reserve(Nm);

        for (size_t i = 0; i < Nm; ++i)
        {
            auto element = vector[i] / scalar;
            elements.push_back(std::move(element));
        }
        std::span<Tr,Nm> span {elements.begin(),elements.end()};
        return VectorBase<Tr,Nm,Vh>{span};
    }

    template <typename Tp, typename Tq, size_t Nm, bool Vh>
        requires (modulo<Tp,Tq>::has() != logic::none)
    auto operator%(VectorBase<Tp,Nm,Vh> const& vector, Tq const& scalar)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        std::vector<Tr> elements;
        elements.reserve(Nm);

        for (size_t i = 0; i < Nm; ++i)
        {
            auto element = modulo<Tp,Tq>::get(vector[i],scalar);
            elements.push_back(std::move(element));
        }
        std::span<Tr,Nm> span {elements.begin(),elements.end()};
        return VectorBase<Tr,Nm,Vh>{span};
    }

} // namespace mpp

/* ************************************************************************** */
// Standard Overloads
/* ************************************************************************** */

namespace std
{

    template <typename Tp, size_t Nm, bool Vh>
    void swap(mpp::VectorBase<Tp,Nm,Vh> const& vector1, mpp::VectorBase<Tp,Nm,Vh> const& vector2)
    {
        vector1.swap(vector2);
    }

} // namespace mpp

#endif /* __HH_MPP_VECTOR */
