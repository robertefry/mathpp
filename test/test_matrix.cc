
#include "gtest/gtest.h"

#include <mathpp/matrix.hh>
using namespace mpp;

TEST(MPP_MATRIX, LIFETIME)
{
    {
        auto elems = std::vector<int>{0,0,0,0};
        auto mat = Matrix<int,2,2>{};                       // default
        EXPECT_EQ(mat.elements(),elems);
    }
    {
        auto elems = std::vector<int>{1,1,1,1};
        auto mat = Matrix<int,2,2>(1);                      // fill
        EXPECT_EQ(mat.elements(),elems);
    }
    {
        auto elems = std::vector<int>{1,2,3,4};
        auto mat = Matrix<int,2,2>{1,2,3,4};                // forward elements
        EXPECT_EQ(mat.elements(),elems);
    }
    {
        auto elems = std::vector<int>{1,2,3,4};
        auto mat = Matrix<int,2,2>{};
        mat = {1,2,3,4};                                    // assign initializer_list
        EXPECT_EQ(mat.elements(),elems);
    }
    {
        auto mat1 = Matrix<int,2,2>{1,2,3,4};
        auto mat2 = Matrix<int,2,2>{mat1};                  // copy construct
        EXPECT_EQ(mat1.elements(),mat2.elements());
    }
    {
        auto mat = Matrix<int,2,2>{1,2,3,4};
        auto mat1 = Matrix<int,2,2>{mat};
        auto mat2 = Matrix<int,2,2>{std::move(mat)};        // move construct
        EXPECT_EQ(mat1.elements(),mat2.elements());
    }
    {
        auto mat = Matrix<int,2,2>{1,2,3,4};
        auto mat1 = Matrix<int,2,2>{mat};
        auto mat2 = Matrix<int,2,2>{};
        mat2 = std::move(mat);                              // move assign
        EXPECT_EQ(mat1.elements(),mat2.elements());
    }
    {
        auto mat1 = Matrix<int,2,2>{1,2,3,4};
        auto mat2 = Matrix<float,2,2>{mat1};                // copy cast construct
        EXPECT_TRUE(mat1 == mat2);
    }
    {
        auto mat1 = Matrix<int,2,2>{1,2,3,4};
        auto mat2 = Matrix<float,2,2>{};
        mat2 = mat1;                                        // copy cast assign
        EXPECT_TRUE(mat1 == mat2);
    }
    {
        auto mat = Matrix<int,2,2>{1,2,3,4};
        auto mat1 = Matrix<int,2,2>{mat};
        auto mat2 = Matrix<float,2,2>{std::move(mat)};      // move cast construct
        EXPECT_TRUE(mat1 == mat2);
    }
    {
        auto mat = Matrix<int,2,2>{1,2,3,4};
        auto mat1 = Matrix<int,2,2>{mat};
        auto mat2 = Matrix<float,2,2>{};
        mat2 = std::move(mat);                              // move cast assign
        EXPECT_TRUE(mat1 == mat2);
    }
}

TEST(MPP_MATRIX, MATHPP)
{
    {
        using identity = identity<Matrix<int,2,2>,op_add>;
        auto has = identity::has();
        EXPECT_TRUE(has == logic::all);

        auto const mat = Matrix<int,2,2>{0,0,0,0};

        auto mat1 = identity::get();
        EXPECT_TRUE(mat1 == mat);

        auto mat2 = Matrix<int,2,2>{1,2,3,4};
        identity::make(mat2);
        EXPECT_TRUE(mat2 == mat);
    }
    {
        using identity = identity<Matrix<float,2,2>,op_mul>;
        auto has = identity::has();
        EXPECT_TRUE(has == logic::all);

        auto const mat = Matrix<float,2,2>{1,0,0,1};

        auto mat1 = identity::get();
        EXPECT_TRUE(mat1 == mat);

        auto mat2 = Matrix<float,2,2>{1,2,3,4};
        identity::make(mat2);
        EXPECT_TRUE(mat2 == mat);
    }
    {
        using inverse = inverse<Matrix<int,2,2>,op_add>;
        auto has = inverse::has();
        EXPECT_TRUE(has == logic::all);

        auto const mat = Matrix<int,2,2>{-1,-2,-3,-4};

        auto mat1 = Matrix<int,2,2>{1,2,3,4};
        auto can = inverse::can(mat1);
        EXPECT_TRUE(can);

        auto mat2 = inverse::get(mat1);
        EXPECT_TRUE(mat2 == mat);

        inverse::make(mat1);
        EXPECT_TRUE(mat1 == mat);
    }
    {
        using inverse = inverse<Matrix<float,2,2>,op_mul>;
        auto has = inverse::has();
        EXPECT_TRUE(has == logic::some);

        auto const mat = Matrix<float,2,2>{-2.0f,1.0f,1.5f,-0.5f};

        auto mat1 = Matrix<float,2,2>{1.0f,2.0f,3.0f,4.0f};
        auto can = inverse::can(mat1);
        EXPECT_TRUE(can);

        auto mat2 = inverse::get(mat1);
        EXPECT_TRUE(mat2 == mat);

        inverse::make(mat1);
        EXPECT_TRUE(mat2 == mat);
    }
    {
        // Matrices with zero determinant are not invertible
        using inverse = inverse<Matrix<float,3,3>,op_mul>;
        auto matrix = Matrix<float,3,3>{1,2,3,4,5,6,7,8,9};
        auto can = inverse::can(matrix);
        EXPECT_FALSE(can);
    }
    {
        // Matrices are not totally-ordered, so don't have absolutes
        using absolute = absolute<Matrix<float,2,2>,op_add>;
        auto has = absolute::has();
        EXPECT_TRUE(has == logic::none);
    }
    {
        // Matrices are not totally-ordered, so don't have absolutes
        using absolute = absolute<Matrix<float,2,2>,op_mul>;
        auto has = absolute::has();
        EXPECT_TRUE(has == logic::none);
    }
}

TEST(MPP_MATRIX, NAMESPACE)
{
    {
        auto mat = Matrix<float,3,3>{1,2,3,4,5,6,7,8,9};
        auto mat1 = Matrix<float,2,2>{1,3,7,9};
        auto mat2 = matrices::submatrix(mat,1,1);
        EXPECT_TRUE(mat2 == mat1);
    }
    {
        auto mat = Matrix<float,2,2>{1,2,3,4};
        auto det = matrices::determinant(mat);
        EXPECT_EQ(det,-2.0f);
    }
    {
        auto mat = Matrix<float,3,3>{1,2,3,4,5,6,7,8,9};
        auto det = matrices::determinant(mat);
        EXPECT_EQ(det,0.0f);
    }
}

TEST(MPP_MATRIX, MATH_SELF)
{
    auto const mat1 = Matrix<float,2,2>{1,2,3,4};
    auto const mat2 = Matrix<float,2,2>{2,4,6,8};
    auto const scalar = 2.0f;
    {
        auto elems = std::vector<float>{3,6,9,12};
        auto mat = mat2;
        mat += mat1;
        EXPECT_EQ(mat.elements(), elems);
    }
    {
        auto elems = std::vector<float>{1,2,3,4};
        auto mat = mat2;
        mat -= mat1;
        EXPECT_EQ(mat.elements(), elems);
    }
    {
        auto elems = std::vector<float>{2,4,6,8};
        auto mat = mat1;
        mat *= scalar;
        EXPECT_EQ(mat.elements(), elems);
    }
    {
        auto elems = std::vector<float>{1,2,3,4};
        auto mat = mat2;
        mat /= scalar;
        EXPECT_EQ(mat.elements(), elems);
    }
    {
        auto elems = std::vector<float>{1,0,1,0};
        auto mat = mat1;
        mat %= scalar;
        EXPECT_EQ(mat.elements(), elems);
    }
}

TEST(MPP_MATRIX, MATH_OTHER)
{
    auto const mat1 = Matrix<float,2,2>{1,2,3,4};
    auto const mat2 = Matrix<float,2,2>{2,4,6,8};
    auto const scalar = 2.0f;
    {
        auto elems = std::vector<float>{1,2,3,4};
        auto mat = +mat1;
        EXPECT_EQ(mat.elements(), elems);
    }
    {
        auto elems = std::vector<float>{-1,-2,-3,-4};
        auto mat = -mat1;
        EXPECT_EQ(mat.elements(), elems);
    }
    {
        auto elems = std::vector<float>{3,6,9,12};
        auto mat = mat2 + mat1;
        EXPECT_EQ(mat.elements(), elems);
    }
    {
        auto elems = std::vector<float>{1,2,3,4};
        auto mat = mat2 - mat1;
        EXPECT_EQ(mat.elements(), elems);
    }
    {
        auto elems = std::vector<float>{14,20,30,44};
        auto mat = mat2 * mat1;
        EXPECT_EQ(mat.elements(), elems);
    }
    {
        auto elems = std::vector<float>{2,0,0,2};
        auto mat = mat2 / mat1;
        EXPECT_EQ(mat.elements(), elems);
    }
    {
        auto elems = std::vector<float>{2,4,6,8};
        auto mat = mat1 * scalar;
        EXPECT_EQ(mat.elements(), elems);
    }
    {
        auto elems = std::vector<float>{1,2,3,4};
        auto mat = mat2 / scalar;
        EXPECT_EQ(mat.elements(), elems);
    }
    {
        auto elems = std::vector<float>{1,0,1,0};
        auto mat = mat1 % scalar;
        EXPECT_EQ(mat.elements(), elems);
    }
}
