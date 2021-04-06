
#ifndef __HH_MPP_MATRIX
#define __HH_MPP_MATRIX

#include "mathpp/mathpp.hh"

#include <vector>
#include <array>
#include <span>
#include <stdexcept>

/* ************************************************************************** */
// Definitions
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp, size_t Nr, size_t Nc>
    class Matrix
    {
    public:
        explicit Matrix();
        explicit Matrix(Tp const&);
        explicit Matrix(std::span<Tp,Nr*Nc> const&);
        virtual ~Matrix() = default;

        template <typename... Args>
            requires (std::conjunction<std::is_convertible<Args,Tp>...>::value)
                && (sizeof...(Args) == Nr*Nc)
        Matrix(Args const&... args);

        template <typename... Args>
            requires (std::conjunction<std::is_same<Args,Tp>...>::value)
                && (sizeof...(Args) == Nr*Nc)
        Matrix(Args&&... args);

        template <typename Tq>
            requires (std::is_convertible<Tq,Tp>::value)
        Matrix(Matrix<Tq,Nr,Nc> const&);

        template <typename Tq>
            requires (std::is_convertible<Tq,Tp>::value)
        Matrix<Tp,Nr,Nc>& operator=(Matrix<Tq,Nr,Nc> const&);

        void swap(Matrix<Tp,Nr,Nc>&);

    public:
        constexpr static size_t index(std::array<size_t,2> const&);
        constexpr static auto rows() { return Nr; }
        constexpr static auto cols() { return Nc; }
        auto elements() const -> std::vector<Tp> const& { return m_Elements; }

        void assign(Tp const&);

        template <typename... Args>
            requires (std::conjunction<std::is_convertible<Args,Tp>...>::value)
                && (sizeof...(Args) == Nr*Nc)
        void assign(Args&&... args);

        Tp determinant() const requires (Nr == Nc != 0);
        Tp trace() const requires (Nr == Nc != 0);

        auto operator[](size_t index) const -> Tp const& { return m_Elements[index]; }
        auto operator[](size_t index) -> Tp& { return m_Elements[index]; }
        auto at(size_t index) const -> Tp const& { return m_Elements.at(index); }
        auto at(size_t index) -> Tp& { return m_Elements.at(index); }

        auto operator[](std::array<size_t,2> const&) const -> Tp const&;
        auto operator[](std::array<size_t,2> const&) -> Tp&;
        auto at(std::array<size_t,2> const&) const -> Tp const&;
        auto at(std::array<size_t,2> const&) -> Tp&;

        template <typename Tq>
        Matrix<Tp,Nr,Nc>& operator+=(Matrix<Tq,Nr,Nc> const&);
        template <typename Tq>
        Matrix<Tp,Nr,Nc>& operator-=(Matrix<Tq,Nr,Nc> const&);
        template <typename Tq>
        Matrix<Tp,Nr,Nc>& operator*=(Tq const&);
        template <typename Tq>
        Matrix<Tp,Nr,Nc>& operator/=(Tq const&);
        template <typename Tq>
        Matrix<Tp,Nr,Nc>& operator%=(Tq const&);

    private:
        std::vector<Tp> m_Elements{};
    };

    namespace matrices
    {

        template <typename Tp, size_t Nr, size_t Nc>
        auto submatrix(Matrix<Tp,Nr,Nc> const&, size_t, size_t);

        template <typename Tp, size_t Nm>
        auto determinant(Matrix<Tp,Nm,Nm> const&) -> Tp;

        template <typename Tp, size_t Nm>
        auto trace(Matrix<Tp,Nm,Nm> const&) -> Tp;

    } // namespace matrices

} // namespace mpp

/* ************************************************************************** */
// MathPP Specialisations
/* ************************************************************************** */

namespace mpp
{

    // identity

    template <typename Tp, size_t Nr, size_t Nc>
    struct identity<Matrix<Tp,Nr,Nc>,op_add>
    {
        constexpr static tristate has()
        {
            return identity<Tp,op_add>::has();
        }
        constexpr static Matrix<Tp,Nr,Nc> get()
        {
            return Matrix<Tp,Nr,Nc>{identity<Tp,op_add>::get()};
        }
        constexpr static Matrix<Tp,Nr,Nc>& make(Matrix<Tp,Nr,Nc>& mat)
        {
            return mat = get();
        }
    };

    template <typename Tp, size_t Nm>
    struct identity<Matrix<Tp,Nm,Nm>,op_mul>
    {
        constexpr static tristate has()
        {
            return identity<Tp,op_mul>::has();
        }
        constexpr static Matrix<Tp,Nm,Nm> get()
        {
            auto mat = identity<Matrix<Tp,Nm,Nm>,op_add>::get();
            for (size_t i = 0; i < Nm; ++i) {
                mat[{i,i}] = identity<Tp,op_mul>::get();
            }
            return mat;
        }
        constexpr static Matrix<Tp,Nm,Nm>& make(Matrix<Tp,Nm,Nm>& mat)
        {
            return mat = get();
        }
    };

    // inverse

    template <typename Tp, size_t Nr, size_t Nc>
        requires (inverse<Tp,op_add>::has() != logic::none)
    struct inverse<Matrix<Tp,Nr,Nc>,op_add>
    {
        constexpr static tristate has()
        {
            return inverse<Tp,op_add>::has();
        }
        constexpr static bool can(Matrix<Tp,Nr,Nc> const&)
        {
            return true;
        }
        constexpr static Matrix<Tp,Nr,Nc> get(Matrix<Tp,Nr,Nc> const& matrix)
        {
            auto const ident = identity<Matrix<Tp,Nr,Nc>,op_add>::get();
            return ident - matrix;
        }
        constexpr static Matrix<Tp,Nr,Nc>& make(Matrix<Tp,Nr,Nc>& matrix)
        {
            return matrix = get(matrix);
        }
    };

    template <typename Tp, size_t Nm>
        requires (inverse<Tp,op_mul>::has() != logic::none)
    struct inverse<Matrix<Tp,Nm,Nm>,op_mul>
    {
        constexpr static tristate has()
        {
            return logic::some;
        }
        constexpr static bool can(Matrix<Tp,Nm,Nm> const& matrix)
        {
            return matrices::determinant(matrix) != identity<Tp,op_add>::get();
        }
        constexpr static Matrix<Tp,Nm,Nm> get(Matrix<Tp,Nm,Nm> const& matrix)
        {
            Matrix<Tp,Nm,Nm> source = matrix;
            Matrix<Tp,Nm,Nm> result = identity<Matrix<Tp,Nm,Nm>,op_mul>::get();
            Tp multiplier;

            // gaussian elimination
            for (size_t k = 0; k < Nm; ++k)
            {
                multiplier = inverse<Tp,op_mul>::get(source[{k,k}]);
                for (size_t j = Nm-1; j < Nm; --j)
                {
                    source[{k,j}] *= multiplier;
                    result[{k,j}] *= multiplier;
                }
                for (size_t i = k+1; i < Nm; ++i)
                {
                    multiplier = source[{i,k}];
                    for (size_t j = Nm-1; j < Nm; --j)
                    {
                        source[{i,j}] -= multiplier * source[{k,j}];
                        result[{i,j}] -= multiplier * result[{k,j}];
                    }
                }
            }

            // jordan elimination
            for (size_t k = Nm-1; k < Nm; --k)
            {
                multiplier = inverse<Tp,op_mul>::get(source[{k,k}]);
                for (size_t j = 0; j < Nm; ++j)
                {
                    source[{k,j}] *= multiplier;
                    result[{k,j}] *= multiplier;
                }
                for (size_t i = k-1; i < Nm; --i)
                {
                    multiplier = source[{i,k}];
                    for (size_t j = 0; j < Nm; ++j)
                    {
                        source[{i,j}] -= multiplier * source[{k,j}];
                        result[{i,j}] -= multiplier * result[{k,j}];
                    }
                }
            }

            return result;
        }
        constexpr static Matrix<Tp,Nm,Nm>& make(Matrix<Tp,Nm,Nm>& matrix)
        {
            return matrix = get(matrix);
        }
    };

} // namespace mpp

/* ************************************************************************** */
// Namespace Functions
/* ************************************************************************** */

namespace mpp
{

    namespace matrices
    {

        template <typename Tp, size_t Nr, size_t Nc>
        auto submatrix(Matrix<Tp,Nr,Nc> const& matrix, size_t i, size_t j)
        {
            constexpr auto size = (Nr-1)*(Nc-1);
            std::vector<Tp> elements;
            elements.reserve(size);

            for (size_t m = 0; m < Nr; ++m) {
                if (m == i) continue;
                for (size_t n = 0; n < Nc; ++n) {
                    if (n == j) continue;
                    elements.push_back(matrix[{m,n}]);
                }
            }
            auto const span = std::span<Tp,size>{elements.begin(),elements.end()};
            return Matrix<Tp,Nr-1,Nc-1>{span};
        }

        template <typename Tp, size_t Nm>
        auto determinant(Matrix<Tp,Nm,Nm> const& matrix) -> Tp
        {
            if constexpr (Nm == 1)
            {
                return matrix[0];
            }
            else
            {
                Tp result = identity<Tp,op_add>::get();

                auto multiplier = identity<Tp,op_mul>::get();
                for (size_t i = 0; i < Nm; ++i)
                {
                    auto const det = determinant(submatrix(matrix,i,0));
                    result += multiplier * det * matrix[{i,0}];
                    inverse<Tp,op_add>::make(multiplier);
                }
                return result;
            }
        }

        template <typename Tp, size_t Nm>
        auto trace(Matrix<Tp,Nm,Nm> const& matrix) -> Tp
        {
            Tp result = identity<Tp,op_add>::get();

            for (size_t i = 0; i < Nm; ++i)
            {
                result += matrix[{i,i}];
            }
            return result;
        }

    } // namespace matrices

} // namespace mpp

/* ************************************************************************** */
// Implementation
/* ************************************************************************** */

namespace mpp
{

    template <typename Tp, size_t Nr, size_t Nc>
    Matrix<Tp,Nr,Nc>::Matrix()
        : m_Elements(Nr*Nc)
    {
    }

    template <typename Tp, size_t Nr, size_t Nc>
    Matrix<Tp,Nr,Nc>::Matrix(Tp const& value)
        : m_Elements(Nr*Nc,value)
    {
    }

    template <typename Tp, size_t Nr, size_t Nc>
    Matrix<Tp,Nr,Nc>::Matrix(std::span<Tp,Nr*Nc> const& span)
        : m_Elements(span.begin(),span.end())
    {
    }

    template <typename Tp, size_t Nr, size_t Nc>
    template <typename... Args>
        requires (std::conjunction<std::is_convertible<Args,Tp>...>::value)
            && (sizeof...(Args) == Nr*Nc)
    Matrix<Tp,Nr,Nc>::Matrix(Args const&... args)
        : m_Elements{static_cast<Tp>(args)...}
    {
    }

    template <typename Tp, size_t Nr, size_t Nc>
    template <typename... Args>
        requires (std::conjunction<std::is_same<Args,Tp>...>::value)
            && (sizeof...(Args) == Nr*Nc)
    Matrix<Tp,Nr,Nc>::Matrix(Args&&... args)
        : m_Elements{std::forward<Args>(args)...}
    {
    }

    template <typename Tp, size_t Nr, size_t Nc>
    template <typename Tq>
        requires (std::is_convertible<Tq,Tp>::value)
    Matrix<Tp,Nr,Nc>::Matrix(Matrix<Tq,Nr,Nc> const& other)
    {
        m_Elements.reserve(Nr*Nc);

        for (size_t i = 0; i < Nr; ++i)
        {
            for (size_t j = 0; j < Nc; ++j)
            {
                m_Elements.push_back(static_cast<Tp>(other[{i,j}]));
            }
        }
    }

    template <typename Tp, size_t Nr, size_t Nc>
    template <typename Tq>
        requires (std::is_convertible<Tq,Tp>::value)
    Matrix<Tp,Nr,Nc>& Matrix<Tp,Nr,Nc>::operator=(Matrix<Tq,Nr,Nc> const& other)
    {
        for (size_t i = 0; i < Nr; ++i)
        {
            for (size_t j = 0; j < Nc; ++j)
            {
                (*this)[{i,j}] = static_cast<Tp>(other[{i,j}]);
            }
        }
        return *this;
    }

    template <typename Tp, size_t Nr, size_t Nc>
    void Matrix<Tp,Nr,Nc>::swap(Matrix<Tp,Nr,Nc>& other)
    {
        std::swap(m_Elements,other.m_Elements);
    }

    template <typename Tp, size_t Nr, size_t Nc>
    constexpr size_t Matrix<Tp,Nr,Nc>::index(std::array<size_t,2> const& indices)
    {
        size_t const index = indices[0] * Nc + indices[1];
        return index;
    }

    template <typename Tp, size_t Nr, size_t Nc>
    void Matrix<Tp,Nr,Nc>::assign(Tp const& value)
    {
        for (size_t i = 0; i < Nr; ++i)
        {
            for (size_t j = 0; j < Nc; ++j)
            {
                (*this)[{i,j}] = value;
            }
        }
    }

    template <typename Tp, size_t Nr, size_t Nc>
    template <typename... Args>
        requires (std::conjunction<std::is_convertible<Args,Tp>...>::value)
            && (sizeof...(Args) == Nr*Nc)
    void Matrix<Tp,Nr,Nc>::assign(Args&&... args)
    {
        m_Elements = {std::forward<Args>(args)...};
    }

    template <typename Tp, size_t Nr, size_t Nc>
    auto Matrix<Tp,Nr,Nc>::operator[](std::array<size_t,2> const& indices) const -> Tp const&
    {
        return m_Elements[index(indices)];
    }

    template <typename Tp, size_t Nr, size_t Nc>
    auto Matrix<Tp,Nr,Nc>::operator[](std::array<size_t,2> const& indices) -> Tp&
    {
        return m_Elements[index(indices)];
    }

    template <typename Tp, size_t Nr, size_t Nc>
    auto Matrix<Tp,Nr,Nc>::at(std::array<size_t,2> const& indices) const -> Tp const&
    {
        return m_Elements.at(index(indices));
    }

    template <typename Tp, size_t Nr, size_t Nc>
    auto Matrix<Tp,Nr,Nc>::at(std::array<size_t,2> const& indices) -> Tp&
    {
        return m_Elements.at(index(indices));
    }

    template <typename Tp, size_t Nr, size_t Nc>
    template <typename Tq>
    Matrix<Tp,Nr,Nc>& Matrix<Tp,Nr,Nc>::operator+=(Matrix<Tq,Nr,Nc> const& other)
    {
        for (size_t i = 0; i < Nr*Nc; ++i)
        {
            m_Elements[i] += other[i];
        }
        return *this;
    }

    template <typename Tp, size_t Nr, size_t Nc>
    template <typename Tq>
    Matrix<Tp,Nr,Nc>& Matrix<Tp,Nr,Nc>::operator-=(Matrix<Tq,Nr,Nc> const& other)
    {
        for (size_t i = 0; i < Nr*Nc; ++i)
        {
            m_Elements[i] -= other[i];
        }
        return *this;
    }

    template <typename Tp, size_t Nr, size_t Nc>
    template <typename Tq>
    Matrix<Tp,Nr,Nc>& Matrix<Tp,Nr,Nc>::operator*=(Tq const& scalar)
    {
        for (size_t i = 0; i < Nr*Nc; ++i)
        {
            m_Elements[i] *= scalar;
        }
        return *this;
    }

    template <typename Tp, size_t Nr, size_t Nc>
    template <typename Tq>
    Matrix<Tp,Nr,Nc>& Matrix<Tp,Nr,Nc>::operator/=(Tq const& scalar)
    {
        for (size_t i = 0; i < Nr*Nc; ++i)
        {
            m_Elements[i] /= scalar;
        }
        return *this;
    }

    template <typename Tp, size_t Nr, size_t Nc>
    template <typename Tq>
    Matrix<Tp,Nr,Nc>& Matrix<Tp,Nr,Nc>::operator%=(Tq const& scalar)
    {
        for (size_t i = 0; i < Nr*Nc; ++i)
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

    template <typename Tp1, size_t Nr1, size_t Nc1, typename Tp2, size_t Nr2, size_t Nc2>
        requires requires (Tp1 a, Tp2 b) { a != b; }
    bool operator==(Matrix<Tp1,Nr1,Nc1> const& matrix1, Matrix<Tp2,Nr2,Nc2> const& matrix2)
    {
        if constexpr (Nr1 != Nr2 || Nc1 != Nc2) {
            return false;
        }
        for (size_t i = 0; i < Nr1*Nc1; ++i)
        {
            if (matrix1[i] != matrix2[i]) return false;
        }
        return true;
    }

    template <typename Tp, size_t Nr, size_t Nc>
        requires (identity<Matrix<Tp,Nr,Nc>,op_add>::has() != logic::none)
    auto operator+(Matrix<Tp,Nr,Nc> const& matrix)
    {
        Matrix<Tp,Nr,Nc> result = identity<Matrix<Tp,Nr,Nc>,op_add>::get();
        return result += matrix;
    }

    template <typename Tp, size_t Nr, size_t Nc>
        requires (identity<Matrix<Tp,Nr,Nc>,op_add>::has() != logic::none)
    auto operator-(Matrix<Tp,Nr,Nc> const& matrix)
    {
        Matrix<Tp,Nr,Nc> result = identity<Matrix<Tp,Nr,Nc>,op_add>::get();
        return result -= matrix;
    }

    template <typename Tp, typename Tq, size_t Nr, size_t Nc>
        requires requires (Tp a, Tq b) { a + b; }
    auto operator+(Matrix<Tp,Nr,Nc> const& matrix1, Matrix<Tq,Nr,Nc> const& matrix2)
    {
        using Tr = op_add::result<Tp,Tq>::type;
        Matrix<Tr,Nr,Nc> result = matrix1;
        return result += matrix2;
    }

    template <typename Tp, typename Tq, size_t Nr, size_t Nc>
        requires requires (Tp a, Tq b) { a - b; }
    auto operator-(Matrix<Tp,Nr,Nc> const& matrix1, Matrix<Tq,Nr,Nc> const& matrix2)
    {
        using Tr = op_add::result<Tp,Tq>::type;
        Matrix<Tr,Nr,Nc> result = matrix1;
        return result -= matrix2;
    }

    template <typename Tp, typename Tq, size_t Nr, size_t Nc, size_t Nz>
        requires requires (Tp a, Tq b) { a * b; }
    auto operator*(Matrix<Tp,Nr,Nc> const& matrix1, Matrix<Tq,Nc,Nz> const& matrix2)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        Matrix<Tr,Nr,Nz> result {identity<Tr,op_add>::get()};

        for (size_t i = 0; i < Nr; ++i)
        {
            for (size_t j = 0; j < Nz; ++j)
            {
                for (size_t k = 0; k < Nc; ++k)
                {
                    result[{i,j}] += matrix1[{i,k}] * matrix2[{k,j}];
                }
            }
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nr, size_t Nc>
        requires requires (Tp a, Tq b) { a * b; }
    auto operator*(Matrix<Tp,Nr,Nc> const& matrix, Tq const& scalar)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        std::vector<Tr> elements;
        elements.reserve(Nr*Nc);

        for (size_t i = 0; i < Nr*Nc; ++i)
        {
            auto element = matrix[i] * scalar;
            elements.push_back(std::move(element));
        }
        std::span<Tr,Nr*Nc> span {elements.begin(),elements.end()};
        return Matrix<Tr,Nr,Nc>{span};
    }

    template <typename Tp, typename Tq, size_t Nr, size_t Nc>
        requires requires (Tp a, Tq b) { b * a; }
    auto operator*(Tq const& scalar, Matrix<Tp,Nr,Nc> const& matrix)
    {
        using Tr = op_mul::result<Tq,Tp>::type;
        std::vector<Tr> elements;
        elements.reserve(Nr*Nc);

        for (size_t i = 0; i < Nr*Nc; ++i)
        {
            auto element = scalar * matrix[i];
            elements.push_back(std::move(element));
        }
        std::span<Tr,Nr*Nc> span {elements.begin(),elements.end()};
        return Matrix<Tr,Nr,Nc>{span};
    }

    template <typename Tp, typename Tq, size_t Nr, size_t Nc>
        requires (inverse<Matrix<Tq,Nc,Nc>,op_mul>::has() != logic::none)
    auto operator/(Matrix<Tp,Nr,Nc> const& matrix1, Matrix<Tq,Nc,Nc> const& matrix2)
    {
        using inverse = inverse<Matrix<Tq,Nc,Nc>,op_mul>;
        return matrix1 * inverse::get(matrix2);
    }

    template <typename Tp, typename Tq, size_t Nm>
        requires (inverse<Matrix<Tq,Nm,Nm>,op_mul>::has() != logic::none)
    auto operator/(Tp const& object, Matrix<Tq,Nm,Nm> const& matrix)
    {
        using inverse = inverse<Matrix<Tq,Nm,Nm>,op_mul>;
        return object * inverse::get(matrix);
    }

    template <typename Tp, typename Tq, size_t Nr, size_t Nc>
        requires requires (Tp a, Tq b) { a / b; }
    auto operator/(Matrix<Tp,Nr,Nc> const& matrix, Tq const& scalar)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        std::vector<Tr> elements;
        elements.reserve(Nr*Nc);

        for (size_t i = 0; i < Nr*Nc; ++i)
        {
            auto element = matrix[i] / scalar;
            elements.push_back(std::move(element));
        }
        std::span<Tr,Nr*Nc> span {elements.begin(),elements.end()};
        return Matrix<Tr,Nr,Nc>{span};
    }

    template <typename Tp, typename Tq, size_t Nr, size_t Nc>
        requires (modulo<Tp,Tq>::has() != logic::none)
    auto operator%(Matrix<Tp,Nr,Nc> const& matrix, Tq const& scalar)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        std::vector<Tr> elements;
        elements.reserve(Nr*Nc);

        for (size_t i = 0; i < Nr*Nc; ++i)
        {
            auto element = modulo<Tp,Tq>::get(matrix[i],scalar);
            elements.push_back(std::move(element));
        }
        std::span<Tr,Nr*Nc> span {elements.begin(),elements.end()};
        return Matrix<Tr,Nr,Nc>{span};
    }

} // namespace mpp

/* ************************************************************************** */
// Standard Overloads
/* ************************************************************************** */

namespace std
{

    template <typename Tp, size_t Nr, size_t Nc>
    void swap(mpp::Matrix<Tp,Nr,Nc>& matrix1, mpp::Matrix<Tp,Nr,Nc>& matrix2)
    {
        return matrix1.swap(matrix2);
    }

} // namespace mpp

#endif /* __HH_MPP_MATRIX */
