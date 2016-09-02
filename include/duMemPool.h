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
            : m_data(nullptr), 
              m_freeHead(nullptr),
              m_chunkSize(chunkSize), 
              m_allocator(allocator) {
            
            m_data = allocateChunk(m_chunkSize);
            if (m_data)
                initChunk(m_data, m_chunkSize);

            m_freeHead = m_data; // first element of free list is beginning of data chunk
        }

        ~MemPool() {
            ElemType *p;

            // delete all memory chunks
            while (m_data) {
                p = getNextChunk(m_data, m_chunkSize);
                deallocateChunk(m_data, m_chunkSize);
                m_data = p;
            }
        }


        template <typename... Args>
        ElemType *make(Args&&... args){
            ElemType *ret = m_freeHead;
            
            if (m_freeHead) {
                // If a free space was available, move the free list forward
                m_freeHead = (ElemType*)*m_freeHead;
            }
            else if (m_data) {
                // If no more free space available, allocate a new chunk
                // and point the last chunk to it.
                ElemType *newChunk = allocateChunk(m_chunkSize);
                if (newChunk) {
                    (ElemType*)*(getLastChunk(m_data, m_chunkSize) + m_chunkSize) = newChunk;
                    initChunk(newChunk, m_chunkSize);

                    m_freeHead = (ElemType*)*newChunk;
                }

                ret = newChunk;
            }

            if (ret)
                m_allocator.construct(ret, std::forward<Args>(args)...);

            return ret;
        }


        void free(ElemType *ptr) {
            if (ptr) {
                ptr->~ElemType();

                // The current head of the free list becomes the second element,
                // and the freed pointer becomes the new head
                (ElemType*)*ptr = m_freeHead;
                m_freeHead = ptr;
            }
        }

    private:
        ElemType *allocateChunk(size_t chunkSize) {
            return m_allocator.allocate(chunkSize + 1); // + 1 to hold pointer to next chunk
        }

        void deallocateChunk(ElemType *chunk, size_t chunkSize) {
            m_allocator.deallocate(chunk, chunkSize + 1);
        }

        void initChunk(ElemType *chunk, size_t chunkSize) {
            // point all free list elements to the next adjacent space in the chunk
            ElemType *p = chunk;
            while (p < chunk + chunkSize) {
                (ElemType*)*p = p + 1;
                ++p;
            }

            // the last element does not have a next
            (ElemType*)*p = nullptr;
        }

        // Get pointer to the next chunk in a series of chunks
        ElemType *getNextChunk(ElemType *startChunk, size_t chunkSize) {
            return reinterpret_cast<ElemType*>(*(startChunk + chunkSize));
        }

        // Get pointer to the last chunk in the given series of chunks 
        ElemType *getLastChunk(ElemType *startChunk, size_t chunkSize) {
            ElemType *ret = startChunk, n = getNextChunk(ret, chunkSize);

            while (n) {
                ret = n;
                n = getNextChunk(ret, chunkSize);
            }

            return ret;
        }


        ElemType *m_data; // Pointer to first memory chunk
        ElemType *m_freeHead; // Pointer to first element of free list

        size_t m_chunkSize;
        Allocator m_allocator;
    };
}

#endif // DU_MEM_POOL_H
