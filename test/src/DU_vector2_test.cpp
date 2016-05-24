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

#include "duVector2.h"
#include "gtest/gtest.h"

using namespace Diamond;

TEST(VectorTest, MultipliesMat) {
    Vector2<int> v(3, 2);
    int m[2][2] = {
        {3, -4},
        {1, 5}
    };

    Vector2<int> res = v.mul(m);
    Vector2<int> exp_res(11, -2);

    EXPECT_EQ(res, exp_res);
}
