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

#ifndef DU_MEM_POOL_H
#define DU_MEM_POOL_H

#include <memory>
#include <stdint.h>

namespace Diamond {
    /**
     A type-aware memory pool that allocates memory chunks of a fixed chunk size.
     
     Note: this class does not call all destructors of the elements in its memory pool
     when it goes out of scope. It does free the memory, but the user has the responsibility
     of calling MemPool::free on every pool object that needs to be destroyed.
    */

    // TODO: memory align?
    template <typename ElemType, class Allocator = std::allocator<ElemType> >
    class MemPool {
    public:
        MemPool(size_t chunkSize = 10, Allocator allocator = Allocator()) 
            : data(nullptr), 
              freeHead(nullptr),
              chunkSize(chunkSize), 
              allocator(allocator) {
            
            data = allocateChunk(chunkSize);
            if (data)
                initChunk(data, chunkSize);

            freeHead = data; // first element of free list is beginning of data chunk
        }

        ~MemPool() {
            ElemType *p;

            // delete all memory chunks
            while (data) {
                p = getNextChunk(data, chunkSize);
                allocator.deallocate(data);
                data = p;
            }
        }


        template <typename... Args>
        ElemType *make(Args&&... args){
            ElemType *ret = freeHead;
            
            if (freeHead) {
                // If a free space was available, move the free list forward
                freeHead = (ElemType*)*freeHead;
            }
            else if (data) {
                // If no more free space available, allocate a new chunk
                // and point the last chunk to it.
                ElemType *newChunk = allocateChunk(chunkSize);
                if (newChunk) {
                    (ElemType*)*(getLastChunk(data, chunkSize) + chunkSize) = newChunk;
                    initChunk(newChunk, chunkSize);

                    freeHead = (ElemType*)*newChunk;
                }

                ret = newChunk;
            }

            if (ret)
                allocator.construct(ret, std::forward<Args>(args)...);

            return ret;
        }


        void free(ElemType *ptr) {
            if (ptr) {
                ptr->~ElemType();

                // The current head of the free list becomes the second element,
                // and the freed pointer becomes the new head
                (ElemType*)*ptr = freeHead;
                freeHead = ptr;
            }
        }

    private:
        ElemType *allocateChunk(size_t size) {
            return allocator.allocate(chunkSize + 1); // + 1 to hold pointer to next chunk
        }

        void initChunk(ElemType *chunk, size_t size) {
            // point all free list elements to the next adjacent space in the chunk
            ElemType *p = chunk;
            while (p < chunk + size) {
                (ElemType*)*p = p + 1;
                ++p;
            }

            // the last element does not have a next
            (ElemType*)*p = nullptr;
        }

        // Get pointer to the next chunk in a series of chunks
        ElemType *getNextChunk(ElemType *startChunk, size_t chSize) {
            return *(startChunk + chSize);
        }

        // Get pointer to the last chunk in the given series of chunks 
        ElemType *getLastChunk(ElemType *startChunk, size_t chSize) {
            ElemType *ret = startChunk, n = getNextChunk(ret, chSize);

            while (n) {
                ret = n;
                n = getNextChunk(ret, chSize);
            }

            return ret;
        }


        ElemType *data; // Pointer to first memory chunk
        ElemType *freeHead; // Pointer to first element of free list

        size_t chunkSize;
        Allocator allocator;
    };
}

#endif // DU_MEM_POOL_H
