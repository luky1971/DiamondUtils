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

#include "duMatrix.h"
#include "gtest/gtest.h"

using namespace Diamond;

TEST(MatrixMathTest, Mul1by1) {
    int m1[1][1] = { {2} };
    int m2[1][1] = { {3} };
    int out[1][1] = {0};
    MatrixMath::mul<int, 1, 1, 1>(m1, m2, out);
    EXPECT_EQ(out[0][0], 6);
}

TEST(MatrixMathTest, Mul2by2) {
    int m1[2][2] = {
        {2, 4},
        {-3, 6}
    };
    int m2[2][2] = {
        {9, -4},
        {7, 3}
    };
    int out[2][2] = {0};
    MatrixMath::mul<int, 2, 2, 2>(m1, m2, out);
    EXPECT_EQ(out[0][0], 46);
    EXPECT_EQ(out[0][1], 4);
    EXPECT_EQ(out[1][0], 15);
    EXPECT_EQ(out[1][1], 30);
}

TEST(MatrixMathTest, Mul1by2and2by2) {
    int m1[1][2] = {
        {6, 7}
    };
    int m2[2][2] = {
        {6, -5},
        {4, 9}
    };
    int out[1][2] = {0};
    MatrixMath::mul<int, 1, 2, 2>(m1, m2, out);
    EXPECT_EQ(out[0][0], 64);
    EXPECT_EQ(out[0][1], 33);
}

TEST(MatrixMathTest, Mul1by3and3by3) {
    int m1[1][3] = {
        {-9, 1, 14}
    };
    int m2[3][3] = {
        {1, 0, 6},
        {0, -9, 2},
        {4, 9, -1}
    };
    int out[1][3] = {0};
    MatrixMath::mul<int, 1, 3, 3>(m1, m2, out);
    EXPECT_EQ(out[0][0], 47);
    EXPECT_EQ(out[0][1], 117);
    EXPECT_EQ(out[0][2], -66);
}

TEST(MatrixMathTest, Mul3by3) {
    int m1[3][3] = {
        {-92, 8, -100},
        {-32, 78, -25},
        {33, 8, 10}
    };
    int m2[3][3] = {
        {56, 62, -78},
        {38, 49, -43},
        {-15, -58, 55}
    };
    int out[3][3] = {0};
    MatrixMath::mul<int, 3, 3, 3>(m1, m2, out);
    EXPECT_EQ(out[0][0], -3348);
    EXPECT_EQ(out[0][1], 488);
    EXPECT_EQ(out[0][2], 1332);
    EXPECT_EQ(out[1][0], 1547);
    EXPECT_EQ(out[1][1], 3288);
    EXPECT_EQ(out[1][2], -2233);
    EXPECT_EQ(out[2][0], 2002);
    EXPECT_EQ(out[2][1], 1858);
    EXPECT_EQ(out[2][2], -2368);
}

TEST(MatrixMathTest, VTimesMat) {
    Vector2<int> v(3, 2);
    int m[2][2] = {
        {3, -4},
        {1, 5}
    };

    Vector2<int> res = MatrixMath::mul(v, m);
    Vector2<int> exp_res(11, -2);

    EXPECT_EQ(res, exp_res);
}

TEST(MatrixMathTest, Inverse) {
    float m1[2][2] = {
        {2, 4},
        {-3, 6}
    };
    float m2[2][2];

    MatrixMath::inv(m1, m2);

    EXPECT_FLOAT_EQ(m2[0][0], 6.0/24.0);
    EXPECT_FLOAT_EQ(m2[0][1], -4.0/24.0);
    EXPECT_FLOAT_EQ(m2[1][0], 3.0/24.0);
    EXPECT_FLOAT_EQ(m2[1][1], 2.0/24.0);

    MatrixMath::sInv(m1, m2);

    EXPECT_FLOAT_EQ(m2[0][0], 6.0/24.0);
    EXPECT_FLOAT_EQ(m2[0][1], -4.0/24.0);
    EXPECT_FLOAT_EQ(m2[1][0], 3.0/24.0);
    EXPECT_FLOAT_EQ(m2[1][1], 2.0/24.0);
}

TEST(MatrixMathTest, SafeInverse) {
    int m1[2][2] = {
        {1, 1},
        {1, 1}
    };
    int m2[2][2] = {
        {9, 8},
        {7, 6}
    };

    MatrixMath::sInv(m1, m2);

    // m2 should be unmodified because m1 is not invertible
    EXPECT_EQ(m2[0][0], 9);
    EXPECT_EQ(m2[0][1], 8);
    EXPECT_EQ(m2[1][0], 7);
    EXPECT_EQ(m2[1][1], 6);
}


TEST(MatrixTest, BracketOperates) {
    Matrix<int, 2, 2> m;
    m[0][0] = 4;
    m[0][1] = 2;
    m[1][0] = 1;
    m[1][1] = 3;
    EXPECT_EQ(m[0][0], 4);
    EXPECT_EQ(m[0][1], 2);
    EXPECT_EQ(m[1][0], 1);
    EXPECT_EQ(m[1][1], 3);
}

TEST(MatrixTest, Constructs) {
    Matrix<int, 2, 2> m = {
        {
            {4, 9},
            {-6, 3}
        }
    };
    EXPECT_EQ(m[0][0], 4);
    EXPECT_EQ(m[0][1], 9);
    EXPECT_EQ(m[1][0], -6);
    EXPECT_EQ(m[1][1], 3);

    Matrix<int, 2, 2> m2 = {0};
    EXPECT_EQ(m2[0][0], 0);
    EXPECT_EQ(m2[0][1], 0);
    EXPECT_EQ(m2[1][0], 0);
    EXPECT_EQ(m2[1][1], 0);
}


TEST(MatrixTest, AddsSubtracts) {
    Matrix<int, 3, 3> m1 = {
        {
            {2, -9, 14},
            {4, 9, -6},
            {0, 2, 1}
        }
    };

    Matrix<int, 3, 3> m2 = {
        {
            {6, 9, -46},
            {2, 0, 5},
            {4, 8, 1}
        }
    };

    Matrix<int, 3, 3> m3 = m1 + m2;

    EXPECT_EQ(m3[0][0], 8);
    EXPECT_EQ(m3[0][1], 0);
    EXPECT_EQ(m3[0][2], -32);
    EXPECT_EQ(m3[1][0], 6);
    EXPECT_EQ(m3[1][1], 9);
    EXPECT_EQ(m3[1][2], -1);
    EXPECT_EQ(m3[2][0], 4);
    EXPECT_EQ(m3[2][1], 10);
    EXPECT_EQ(m3[2][2], 2);

    m3 = m1 - m2;

    EXPECT_EQ(m3[0][0], -4);
    EXPECT_EQ(m3[0][1], -18);
    EXPECT_EQ(m3[0][2], 60);
    EXPECT_EQ(m3[1][0], 2);
    EXPECT_EQ(m3[1][1], 9);
    EXPECT_EQ(m3[1][2], -11);
    EXPECT_EQ(m3[2][0], -4);
    EXPECT_EQ(m3[2][1], -6);
    EXPECT_EQ(m3[2][2], 0);

    m1 += m1 + m2 - m1;

    EXPECT_EQ(m1[0][0], 8);
    EXPECT_EQ(m1[0][1], 0);
    EXPECT_EQ(m1[0][2], -32);
    EXPECT_EQ(m1[1][0], 6);
    EXPECT_EQ(m1[1][1], 9);
    EXPECT_EQ(m1[1][2], -1);
    EXPECT_EQ(m1[2][0], 4);
    EXPECT_EQ(m1[2][1], 10);
    EXPECT_EQ(m1[2][2], 2);

    m2 -= m2;

    EXPECT_EQ(m2[0][0], 0);
    EXPECT_EQ(m2[0][1], 0);
    EXPECT_EQ(m2[0][2], 0);
    EXPECT_EQ(m2[1][0], 0);
    EXPECT_EQ(m2[1][1], 0);
    EXPECT_EQ(m2[1][2], 0);
    EXPECT_EQ(m2[2][0], 0);
    EXPECT_EQ(m2[2][1], 0);
    EXPECT_EQ(m2[2][2], 0);
}

TEST(MatrixTest, ScalarMultiplies) {
    Matrix<int, 3, 3> m1 = {
        {
            {2, -9, 14},
            {4, 9, -6},
            {0, 2, 1}
        }
    };

    Matrix<int, 3, 3> m2 = 2 * m1;

    EXPECT_EQ(m2[0][0], 4);
    EXPECT_EQ(m2[0][1], -18);
    EXPECT_EQ(m2[0][2], 28);
    EXPECT_EQ(m2[1][0], 8);
    EXPECT_EQ(m2[1][1], 18);
    EXPECT_EQ(m2[1][2], -12);
    EXPECT_EQ(m2[2][0], 0);
    EXPECT_EQ(m2[2][1], 4);
    EXPECT_EQ(m2[2][2], 2);

    m2 = -2 * m2;

    EXPECT_EQ(m2[0][0], -8);
    EXPECT_EQ(m2[0][1], 36);
    EXPECT_EQ(m2[0][2], -56);
    EXPECT_EQ(m2[1][0], -16);
    EXPECT_EQ(m2[1][1], -36);
    EXPECT_EQ(m2[1][2], 24);
    EXPECT_EQ(m2[2][0], 0);
    EXPECT_EQ(m2[2][1], -8);
    EXPECT_EQ(m2[2][2], -4);
}

TEST(MatrixTest, Multiplies) {
    Matrix<int, 3, 3> m1 = {
        {
            {2, -9, 14},
            {4, 9, -6},
            {0, 2, 1}
        }
    };

    Matrix<int, 3, 3> m2 = {
        {
            {6, 9, -46},
            {2, 0, 5},
            {4, 8, 1}
        }
    };

    Matrix<int, 3, 3> m3 = m1.mul(m2);

    EXPECT_EQ(m3[0][0], 50);
    EXPECT_EQ(m3[0][1], 130);
    EXPECT_EQ(m3[0][2], -123);
    EXPECT_EQ(m3[1][0], 18);
    EXPECT_EQ(m3[1][1], -12);
    EXPECT_EQ(m3[1][2], -145);
    EXPECT_EQ(m3[2][0], 8);
    EXPECT_EQ(m3[2][1], 8);
    EXPECT_EQ(m3[2][2], 11);

    m3 = m2.mul(m1);

    EXPECT_EQ(m3[0][0], 48);
    EXPECT_EQ(m3[0][1], -65);
    EXPECT_EQ(m3[0][2], -16);
    EXPECT_EQ(m3[1][0], 4);
    EXPECT_EQ(m3[1][1], -8);
    EXPECT_EQ(m3[1][2], 33);
    EXPECT_EQ(m3[2][0], 40);
    EXPECT_EQ(m3[2][1], 38);
    EXPECT_EQ(m3[2][2], 9);
}

TEST(MatrixTest, Inverse) {
    Matrix<float, 2, 2> m1 = {
        {
            {2, 4},
            {-3, 6}
        }
    };
    Matrix<float, 2, 2> m2;

    m2 = m1.inv();

    EXPECT_FLOAT_EQ(m2[0][0], 6.0/24.0);
    EXPECT_FLOAT_EQ(m2[0][1], -4.0/24.0);
    EXPECT_FLOAT_EQ(m2[1][0], 3.0/24.0);
    EXPECT_FLOAT_EQ(m2[1][1], 2.0/24.0);

    m2 = m1.sInv();

    EXPECT_FLOAT_EQ(m2[0][0], 6.0/24.0);
    EXPECT_FLOAT_EQ(m2[0][1], -4.0/24.0);
    EXPECT_FLOAT_EQ(m2[1][0], 3.0/24.0);
    EXPECT_FLOAT_EQ(m2[1][1], 2.0/24.0);
}

TEST(MatrixTest, SafeInverse) {
    Matrix<int, 2, 2> m1 = {
        {
            {1, 1},
            {1, 1}
        }
    };

    Matrix<int, 2, 2> m2 = m1.sInv();

    // m2 should be zero matrix because m1 is not invertible
    EXPECT_EQ(m2[0][0], 0);
    EXPECT_EQ(m2[0][1], 0);
    EXPECT_EQ(m2[1][0], 0);
    EXPECT_EQ(m2[1][1], 0);
}

TEST(VectorMatrixTest, MatTimesV) {
    Matrix<int, 2, 2> m = {
        {
            {3, -4},
            {1, 5}
        }
    };
    Vector2<int> v(3, 2);

    Vector2<int> res = MatrixMath::mul(m.m, v);
    Vector2<int> exp_res(1, 13);

    EXPECT_EQ(res, exp_res);

    res = m.mul(v);
    EXPECT_EQ(res, exp_res);
}
