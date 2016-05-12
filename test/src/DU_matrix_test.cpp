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

#include "D_Matrix.h"
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

    m2 *= -2;

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
