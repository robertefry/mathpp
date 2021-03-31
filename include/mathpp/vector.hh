
#ifndef __HH_MPP_VECTOR
#define __HH_MPP_VECTOR

#include "mathpp/mathpp.hh"

#include <vector>

/* ************************************************************************** */
// Definitions
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp, size_t Nm>
    class Vector
    {
    public:
        explicit Vector();
        explicit Vector(Tp const&);
        virtual ~Vector() = default;

        template <typename... Args>
            requires (std::conjunction<std::is_convertible<Args,Tp>...>::value)
        Vector(Args const&... args)
            requires (sizeof...(args) == Nm);

        template <typename... Args>
            requires (std::conjunction<std::is_same<Args,Tp>...>::value)
        Vector(Args&&... args)
            requires (sizeof...(args) == Nm);

        template <typename Tq>
            requires (std::is_convertible<Tq,Tp>::value)
        Vector(Vector<Tq,Nm> const&);

        template <typename Tq>
            requires (std::is_assignable<Tq,Tp>::value)
        Vector<Tp,Nm>& operator=(Vector<Tq,Nm> const&);

        void swap(Vector<Tp,Nm>&);

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
        Vector<Tp,Nm>& operator+=(Vector<Tq,Nm> const&);
        template <typename Tq>
        Vector<Tp,Nm>& operator-=(Vector<Tq,Nm> const&);
        template <typename Tq>
        Vector<Tp,Nm>& operator*=(Tq const&);
        template <typename Tq>
        Vector<Tp,Nm>& operator/=(Tq const&);
        template <typename Tq>
        Vector<Tp,Nm>& operator%=(Tq const&);

    private:
        std::vector<Tp> m_Elements{};
    };

} // namespace mpp

/* ************************************************************************** */
// MathPP Specialisations
/* ************************************************************************** */

namespace mpp
{

    // identity

    template <typename Tp, size_t Nm>
    struct identity<Vector<Tp,Nm>,op_add>
    {
        constexpr static tristate has()
        {
            return identity<Tp,op_add>::has();
        }
        static Vector<Tp,Nm> const& get()
        {
            static auto const s_ident = Vector<Tp,Nm>{identity<Tp,op_add>::get()};
            return s_ident;
        }
        static Vector<Tp,Nm>& make(Vector<Tp,Nm>& vec)
        {
            return vec = get();
        }
    };

} // namespace mpp

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp, size_t Nm>
    Vector<Tp,Nm>::Vector()
        : m_Elements(Nm,identity<Tp,op_add>::get())
    {
    }

    template <typename Tp, size_t Nm>
    Vector<Tp,Nm>::Vector(Tp const& value)
        : m_Elements(Nm,value)
    {
    }

    template <typename Tp, size_t Nm>
    template <typename... Args>
        requires (std::conjunction<std::is_convertible<Args,Tp>...>::value)
    Vector<Tp,Nm>::Vector(Args const&... args)
        requires (sizeof...(args) == Nm)
        : m_Elements{static_cast<Tp>(args)...}
    {
    }

    template <typename Tp, size_t Nm>
    template <typename... Args>
        requires (std::conjunction<std::is_same<Args,Tp>...>::value)
    Vector<Tp,Nm>::Vector(Args&&... args)
        requires (sizeof...(args) == Nm)
        : m_Elements{std::forward<Args>(args)...}
    {
    }

    template <typename Tp, size_t Nm>
    template <typename Tq>
        requires (std::is_convertible<Tq,Tp>::value)
    Vector<Tp,Nm>::Vector(Vector<Tq,Nm> const& other)
    {
        m_Elements.reserve(Nm);

        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements.push_back(static_cast<Tp>(other[i]));
        }
    }

    template <typename Tp, size_t Nm>
    template <typename Tq>
        requires (std::is_assignable<Tq,Tp>::value)
    Vector<Tp,Nm>& Vector<Tp,Nm>::operator=(Vector<Tq,Nm> const& other)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements[i] = other[i];
        }
    }

    template <typename Tp, size_t Nm>
    void Vector<Tp,Nm>::swap(Vector<Tp,Nm>& other)
    {
        std::swap(m_Elements,other.m_Elements);
    }

    template <typename Tp, size_t Nm>
    void Vector<Tp,Nm>::assign(Tp const& value)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements[i] = value;
        }
    }

    template <typename Tp, size_t Nm>
    template <typename... Args>
        requires (std::conjunction<std::is_assignable<Args,Tp>...>::value)
    void Vector<Tp,Nm>::assign(Args&&... args)
        requires (sizeof...(args) == Nm)
    {
        m_Elements = {std::forward<Args>(args)...};
    }

    template <typename Tp, size_t Nm>
    template <typename Tq>
    Vector<Tp,Nm>& Vector<Tp,Nm>::operator+=(Vector<Tq,Nm> const& other)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements[i] += other[i];
        }
        return *this;
    }

    template <typename Tp, size_t Nm>
    template <typename Tq>
    Vector<Tp,Nm>& Vector<Tp,Nm>::operator-=(Vector<Tq,Nm> const& other)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements[i] -= other[i];
        }
        return *this;
    }

    template <typename Tp, size_t Nm>
    template <typename Tq>
    Vector<Tp,Nm>& Vector<Tp,Nm>::operator*=(Tq const& scalar)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements[i] *= scalar;
        }
        return *this;
    }

    template <typename Tp, size_t Nm>
    template <typename Tq>
    Vector<Tp,Nm>& Vector<Tp,Nm>::operator/=(Tq const& scalar)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            m_Elements[i] /= scalar;
        }
        return *this;
    }

    template <typename Tp, size_t Nm>
    template <typename Tq>
    Vector<Tp,Nm>& Vector<Tp,Nm>::operator%=(Tq const& scalar)
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

    template <typename Tp, typename Tq, size_t Nm>
        requires requires (Tp a, Tq b) { a != b; }
    bool operator==(Vector<Tp,Nm> const& vec1, Vector<Tq,Nm> const& vec2)
    {
        for (size_t i = 0; i < Nm; ++i)
        {
            if (vec1[i] != vec2[i]) return false;
        }
        return true;
    }

    template <typename Tp, size_t Nm>
        requires requires (Tp a, Tp b) { a + b; }
    auto operator+(Vector<Tp,Nm> const& vec)
    {
        auto result = Vector<Tp,Nm>{identity<Tp,op_add>::get()};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] += vec[i];
        }
        return result;
    }

    template <typename Tp, size_t Nm>
        requires requires (Tp a, Tp b) { a - b; }
    auto operator-(Vector<Tp,Nm> const& vec)
    {
        auto result = Vector<Tp,Nm>{identity<Tp,op_add>::get()};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] -= vec[i];
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm>
        requires requires (Tp a, Tq b) { a + b; }
    auto operator+(Vector<Tp,Nm> const& vec1, Vector<Tq,Nm> const& vec2)
    {
        auto result = Vector<Tp,Nm>{vec1};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] += vec2[i];
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm>
        requires requires (Tp a, Tq b) { a - b; }
    auto operator-(Vector<Tp,Nm> const& vec1, Vector<Tq,Nm> const& vec2)
    {
        auto result = Vector<Tp,Nm>{vec1};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] -= vec2[i];
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm>
        requires requires (Tp a, Tq b) { a * b; }
    auto operator*(Vector<Tp,Nm> const& vec, Tq const& scalar)
    {
        auto result = Vector<Tp,Nm>{vec};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] *= scalar;
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm>
        requires requires (Tp a, Tq b) { b * a; }
    auto operator*(Tq const& scalar, Vector<Tp,Nm> const& vec)
    {
        auto result = Vector<Tp,Nm>{vec};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] *= scalar;
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm>
        requires requires (Tp a, Tq b) { a / b; }
    auto operator/(Vector<Tp,Nm> const& vec, Tq const& scalar)
    {
        auto result = Vector<Tp,Nm>{vec};

        for (size_t i = 0; i < Nm; ++i)
        {
            result[i] /= scalar;
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm>
        requires (modulo<Tp,Tq>::has() != logic::none)
    auto operator%(Vector<Tp,Nm> const& vec, Tq const& scalar)
    {
        auto result = Vector<Tp,Nm>{vec};

        for (size_t i = 0; i < Nm; ++i)
        {
            modulo<Tp,Tq>::make(result[i],scalar);
        }
        return result;
    }

} // namespace mpp

/* ************************************************************************** */
// Standard Overloads
/* ************************************************************************** */

namespace std
{

    template <typename Tp, size_t Nm>
    void swap(mpp::Vector<Tp,Nm> const& vec1, mpp::Vector<Tp,Nm> const& vec2)
    {
        vec1.swap(vec2);
    }

} // namespace mpp

#endif /* __HH_MPP_VECTOR */
