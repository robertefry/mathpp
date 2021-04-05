
#ifndef __HH_MPP_LINALG
#define __HH_MPP_LINALG

#include <mathpp/matrix.hh>
#include <mathpp/vector.hh>

namespace mpp
{

    template <typename Tp, typename Tq, size_t Nr, size_t Nc>
    auto operator*(Matrix<Tp,Nr,Nc> const& matrix, Vector<Tq,Nc> const& vector)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        Vector<Tr,Nr> result = identity<Vector<Tr,Nr>,op_add>::get();

        for (size_t i = 0; i < Nr; ++i)
        {
            for (size_t j = 0; j < Nc; ++j)
            {
                result[i] += matrix[{i,j}] * vector[j];
            }
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nr, size_t Nc>
    auto operator*(CoVector<Tp,Nr> const& covector, Matrix<Tq,Nr,Nc> const& matrix)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        CoVector<Tr,Nc> result = identity<CoVector<Tr,Nc>,op_add>::get();

        for (size_t i = 0; i < Nr; ++i)
        {
            for (size_t j = 0; j < Nc; ++j)
            {
                result[j] += covector[i] * matrix[{i,j}];
            }
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nr, size_t Nc>
    auto operator*(Vector<Tp,Nr> const& vector, CoVector<Tq,Nc> const& covector)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        Matrix<Tr,Nr,Nc> result = identity<Matrix<Tr,Nr,Nc>,op_add>::get();

        for (size_t i = 0; i < Nr; ++i)
        {
            for (size_t j = 0; j < Nc; ++j)
            {
                result[{i,j}] += vector[i] * covector[j];
            }
        }
        return result;
    }

    template <typename Tp, typename Tq, size_t Nm>
    auto operator*(CoVector<Tp,Nm> const& covector, Vector<Tq,Nm> const& vector)
    {
        using Tr = op_mul::result<Tp,Tq>::type;
        Tr result = identity<Tr,op_add>::get();

        for (size_t i = 0; i < Nm; ++i)
        {
            result += covector[i] * vector[i];
        }
        return result;
    }

} // namespace mpp

#endif /* __HH_MPP_LINALG */
