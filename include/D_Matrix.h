/*
    Copyright 2016 Ahnaf Siddiqui

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef D_MATRIX_H
#define D_MATRIX_H

namespace Diamond {
    namespace MatrixMath {
        
        /**
         Multiplies two matrices and stores the result in out.
         Out should be initialized to zero!
        */
        template <typename M, int dim1, int dim2, int dim3>
        inline void mul(const M m1[dim1][dim2], const M m2[dim2][dim3], M out[dim1][dim3]) {
            for (int r = 0; r < dim1; ++r) {
                for (int r2 = 0; r2 < dim2; ++r2) {
                    for (int c = 0; c < dim3; ++c) {
                        out[r][c] += m1[r][r2] * m2[r2][c];
                    }
                }
            }
        }

    }

    template <typename T, int dim1, int dim2>
    class Matrix {
    public:
        T m[dim1][dim2];

        // operator overloads

        T *operator[](int index) {
            return m[index];
        }

        const T *operator[](int index) const {
            return m[index];
        }

        Matrix operator+(const Matrix &m2) const {
            Matrix res;
            for (int r = 0; r < dim1; ++r) {
                for (int c = 0; c < dim2; ++c) {
                    res[r][c] = m[r][c] + m2[r][c];
                }
            }
            return res;
        }

        Matrix operator-(const Matrix &m2) const {
            Matrix res;
            for (int r = 0; r < dim1; ++r) {
                for (int c = 0; c < dim2; ++c) {
                    res[r][c] = m[r][c] - m2[r][c];
                }
            }
            return res;
        }

        Matrix &operator+=(const Matrix &m2) {
            for (int r = 0; r < dim1; ++r) {
                for (int c = 0; c < dim2; ++c) {
                    m[r][c] += m2[r][c];
                }
            }
            return *this;
        }

        Matrix &operator-=(const Matrix &m2) {
            for (int r = 0; r < dim1; ++r) {
                for (int c = 0; c < dim2; ++c) {
                    m[r][c] -= m2[r][c];
                }
            }
            return *this;
        }

        template <int dim3>
        Matrix operator*(const Matrix<T, dim2, dim3> &m2) const {
            Matrix<T, dim1, dim3> res = { 0 };
            MatrixMath::mul<T, dim1, dim2, dim3>(m, m2.m, res.m);
            return res;
        }

        template <typename S>
        Matrix &operator*=(const S scalar) {
            for (int r = 0; r < dim1; ++r) {
                for (int c = 0; c < dim2; ++c) {
                    m[r][c] *= scalar;
                }
            }
            return *this;
        }
    };

    // Scalar multiplication
    template <typename S, typename M, int dim1, int dim2>
    inline Matrix<M, dim1, dim2> operator*(const S scalar, Matrix<M, dim1, dim2> &m) {
        Matrix<M, dim1, dim2> m2;
        for (int r = 0; r < dim1; ++r) {
            for (int c = 0; c < dim2; ++c) {
                m2[r][c] = scalar * m[r][c];
            }
        }
        return m2;
    }
}

#endif // D_MATRIX_H
