/*
    Copyright 2017 Ahnaf Siddiqui

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

#include "duDumbPtr.h"
#include "gtest/gtest.h"
using namespace Diamond;

class Deleter {
public:
    Deleter(bool *deleted) : deleted(deleted) {}

    template <typename T>
    void operator()(T *ptr) {
        delete ptr;
        *deleted = true;
    }

private:
    bool *deleted;
};

class DumbSubDeleter : public DumbDeleter {
public:
    DumbSubDeleter(bool *deleted) : deleted(deleted) {}

    void free(void *ptr) const override {
        delete (int*)ptr;
        *deleted = true;
    }

private:
    bool *deleted;
};

TEST(DumbPtrTest, Dereferences) {
    DumbPtr<int> p(new int(5));

    EXPECT_EQ(*(p.get()), 5);
    EXPECT_EQ(*p, 5);

    p.free();
}

struct Swag {
    int a;
    int b;
};

TEST(DumbPtrTest, StructDereferences) {
    Swag *bag = new Swag();
    bag->a = 0;
    bag->b = 0;

    DumbPtr<Swag> p(bag);

    p->a = 2;
    p->b = 3;

    EXPECT_EQ(p->a, 2);
    EXPECT_EQ(p->b, 3);

    p.free();
}

TEST(DumbPtrTest, Frees) {
    bool deleted = false;
    DumbSubDeleter deleter(&deleted);
    DumbPtr<int> p(new int(1), &deleter);

    EXPECT_FALSE(deleted);
    EXPECT_TRUE(p);

    p.free();

    EXPECT_TRUE(deleted);
    EXPECT_FALSE(p);
}

TEST(DumbPtrTest, Bool) {
    DumbPtr<int> p1(nullptr);
    DumbPtr<int> p2(new int(5));

    EXPECT_FALSE(p1);
    EXPECT_TRUE(p2);

    p1.free();
    p2.free();
}

TEST(DumbPtrTest, Compares) {
    int *a = new int(1);
    int *b = new int(2);

    DumbPtr<int> p1(a);
    DumbPtr<int> p2(a);
    DumbPtr<int> p3(b);

    EXPECT_TRUE(p1 == p1);
    EXPECT_TRUE(p3 == p3);
    EXPECT_TRUE(p1 != p3);
    EXPECT_TRUE(p1 == p2);
    EXPECT_EQ(p1 < p3, a < b);
    EXPECT_EQ(p1 <= p3, a <= b);
    EXPECT_EQ(p1 > p3, a > b);
    EXPECT_EQ(p1 >= p3, a >= b);

    p1.free();
    p3.free();
}

TEST(DumbPtrTest, Converts) {
    DumbPtr<int> a = nullptr;
    EXPECT_FALSE(a);

    DumbPtr<int> b(new int(1));
    int *bp = b;
    EXPECT_EQ(*bp, 1);

    DumbPtr<int> c = bp;
    EXPECT_EQ(b, c);
}
