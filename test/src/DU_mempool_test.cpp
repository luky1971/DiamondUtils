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

#include <vector>
#include "duMemPool.h"
#include "duVector2.h"
#include "gtest/gtest.h"

using namespace Diamond;

// TODO: test internal functions like destructor, getNextChunk, etc
// and ensure that there are no memory leaks

TEST(MemPoolTest, Constructs) {
    MemPool<Vector2<int> > vectorPool;
}

TEST(MemPoolTest, Allocates) {
    MemPool<Vector2<int> > vectorPool(10);

    Vector2<int> *v1 = vectorPool.make(2, 3);

    ASSERT_NE(v1, nullptr);

    EXPECT_EQ(v1->x, 2);
    EXPECT_EQ(v1->y, 3);

    v1->x = 45;
    v1->y = 9;

    EXPECT_EQ(v1->x, 45);
    EXPECT_EQ(v1->y, 9);
}

TEST(MemPoolTest, Frees) {
    MemPool<Vector2<int> > vectorPool(10);

    Vector2<int> *v1 = vectorPool.make(2, 3);

    ASSERT_NE(v1, nullptr);

    EXPECT_EQ(v1->x, 2);
    EXPECT_EQ(v1->y, 3);

    vectorPool.free(v1);

    Vector2<int> *v2 = vectorPool.make();

    // v2 should've been given the same memory location
    // as the last pointer that was freed (v1)
    EXPECT_EQ(v1, v2);
}

TEST(MemPoolTest, AllocatesAndFrees) {
    MemPool<Vector2<int> > vectorPool(10);

    // Check an allocation after a free

    Vector2<int> *v1 = vectorPool.make(2, 3);

    ASSERT_NE(v1, nullptr);

    EXPECT_EQ(v1->x, 2);
    EXPECT_EQ(v1->y, 3);

    vectorPool.free(v1);


    Vector2<int> *v2 = vectorPool.make(4, 9);

    ASSERT_NE(v2, nullptr);

    EXPECT_EQ(v2->x, 4);
    EXPECT_EQ(v2->y, 9);

    EXPECT_EQ(v1, v2);

    // More allocations

    Vector2<int> *v3 = vectorPool.make(-4, -9);

    ASSERT_NE(v3, nullptr);

    EXPECT_EQ(v3->x, -4);
    EXPECT_EQ(v3->y, -9);


    Vector2<int> *v4 = vectorPool.make(13, 5);

    ASSERT_NE(v4, nullptr);

    EXPECT_EQ(v4->x, 13);
    EXPECT_EQ(v4->y, 5);


    // check that data stays consistent after freeing

    vectorPool.free(v3);
    vectorPool.free(v2);

    EXPECT_EQ(v4->x, 13);
    EXPECT_EQ(v4->y, 5);


    // allocate more then check data consistency again

    Vector2<int> *v5 = vectorPool.make(12, 19);

    EXPECT_EQ(v5, v2);

    EXPECT_EQ(v5->x, 12);
    EXPECT_EQ(v5->y, 19);

    EXPECT_EQ(v4->x, 13);
    EXPECT_EQ(v4->y, 5);


    Vector2<int> *v6 = vectorPool.make(1, 1);

    EXPECT_EQ(v6, v3);

    EXPECT_EQ(v6->x, 1);
    EXPECT_EQ(v6->y, 1);

    EXPECT_EQ(v5->x, 12);
    EXPECT_EQ(v5->y, 19);

    EXPECT_EQ(v4->x, 13);
    EXPECT_EQ(v4->y, 5);
}


TEST(MemPoolTest, AllocatesChunks) {
    const int chunkSize = 20;

    MemPool<Vector2<int> > vectorPool(chunkSize);

    std::vector<Vector2<int>* > vecPtrs;
    vecPtrs.push_back(vectorPool.make(0, 0));

    // Use up the first chunk
    for (int i = 1; i < chunkSize; ++i) {
        vecPtrs.push_back(vectorPool.make(i, -i));
        EXPECT_EQ(vecPtrs[i]->x, i);
        EXPECT_EQ(vecPtrs[i]->y, -i);
        EXPECT_TRUE(vecPtrs[i] > vecPtrs[i - 1]);
    }

    // this should allocate a new chunk
    Vector2<int> *next = vectorPool.make(chunkSize, -chunkSize);

    for (auto p : vecPtrs)
        EXPECT_NE(next, p);

    vecPtrs.push_back(next);

    // Use up the second chunk
    for (int i = chunkSize + 1; i < 2 * chunkSize; ++i) {
        vecPtrs.push_back(vectorPool.make(i, -i));
        EXPECT_EQ(vecPtrs[i]->x, i);
        EXPECT_EQ(vecPtrs[i]->y, -i);
        EXPECT_TRUE(vecPtrs[i] > vecPtrs[i - 1]);

        // check that the newly allocated pointer
        // is different from all previously allocated ones
        for (int j = 0; j < i - 1; ++j)
            EXPECT_NE(vecPtrs[j], vecPtrs[i]);
    }

    // this should allocate a new chunk
    next = vectorPool.make();

    for (auto p : vecPtrs)
        EXPECT_NE(next, p);

    vecPtrs.push_back(next);

    // check that all data is still valid
    for (int i = 0; i < chunkSize * 2; ++i) {
        EXPECT_EQ(vecPtrs[i]->x, i);
        EXPECT_EQ(vecPtrs[i]->y, -i);
    }

    // free every other vector and check that other data is valid
    for (int i = 1; i < chunkSize * 2; ++i) {
        if (i % 2 == 1)
            vectorPool.free(vecPtrs[i]);
    }
    for (int i = 0; i < chunkSize * 2; ++i) {
        if (i % 2 == 0) {
            EXPECT_EQ(vecPtrs[i]->x, i);
            EXPECT_EQ(vecPtrs[i]->y, -i);
        }
    }

    // Use up the third chunk
    // in order to force a new chunk to be allocated, 
    // we are allocating twice as much as chunkSize this time
    // because a bunch of stuff was just freed
    for (int i = 2 * chunkSize + 1; i < 4 * chunkSize; ++i) {
        vecPtrs.push_back(vectorPool.make(i, -i));
        EXPECT_EQ(vecPtrs[i]->x, i);
        EXPECT_EQ(vecPtrs[i]->y, -i);
    }

    // check data validity again
    for (int i = 0; i < chunkSize * 2; ++i) {
        if (i % 2 == 0) {
            EXPECT_EQ(vecPtrs[i]->x, i);
            EXPECT_EQ(vecPtrs[i]->y, -i);
        }
    }
}
