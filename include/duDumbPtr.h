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

#ifndef D_DUMB_PTR_H
#define D_DUMB_PTR_H

#include <memory>

namespace Diamond {
    /**
     * Wraps a pointer and a deleter. Does not automatically delete the pointer.
     * You have to free the pointer manually by calling dumbPtr.free().
     * Safe to copy and assign.
     */
    template <typename T, class Deleter = std::default_delete<T> >
    class DumbPtr {
    public:
        DumbPtr() : ptr(nullptr) {}

        DumbPtr(std::nullptr_t p) : ptr(p) {}

        DumbPtr(T *ptr, Deleter d = Deleter()) : ptr(ptr), d(d) {}

        T *get() const { return ptr; }

        Deleter &get_deleter() { return d; }
        const Deleter &get_deleter() const { return d; }

        T &operator*() const { return *ptr; }

        T *operator->() const { return ptr; }

        void free() { d(ptr); ptr = nullptr; }

        explicit operator bool() const { return ptr != nullptr; }

    private:
        T *ptr;
        Deleter d;
    };

    // Casting

    template <typename T, typename TD, typename U, typename UD>
    DumbPtr<T, TD> static_pointer_cast(const DumbPtr<U, UD> &ptr) noexcept {
        return DumbPtr<T, TD>(static_cast<T*>(ptr.get()), ptr.get_deleter);
    }

    template <typename T, typename TD, typename U, typename UD>
    DumbPtr<T, TD> dynamic_pointer_cast(const DumbPtr<U, UD> &ptr) noexcept {
        return DumbPtr<T, TD>(dynamic_cast<T*>(ptr.get()), ptr.get_deleter);
    }

    template <typename T, typename TD, typename U, typename UD>
    DumbPtr<T, TD> const_pointer_cast(const DumbPtr<U, UD> &ptr) noexcept {
        return DumbPtr<T, TD>(const_cast<T*>(ptr.get()), ptr.get_deleter);
    }

    template <typename T, typename TD, typename U, typename UD>
    DumbPtr<T, TD> reinterpret_pointer_cast(const DumbPtr<U, UD> &ptr) noexcept {
        return DumbPtr<T, TD>(reinterpret_cast<T*>(ptr.get()), ptr.get_deleter);
    }

    // Comparison operators

    template <typename T, typename TD, typename U, typename UD>
    bool operator==(const DumbPtr<T,TD> &a, const DumbPtr<U,UD> &b) {
        return a.get() == b.get();
    }

    template <typename T, typename TD, typename U, typename UD>
    bool operator!=(const DumbPtr<T,TD> &a, const DumbPtr<U,UD> &b) {
        return a.get() != b.get();
    }

    template <typename T, typename TD, typename U, typename UD>
    bool operator<(const DumbPtr<T,TD> &a, const DumbPtr<U,UD> &b) {
        return a.get() < b.get();
    }

    template <typename T, typename TD, typename U, typename UD>
    bool operator>(const DumbPtr<T,TD> &a, const DumbPtr<U,UD> &b) {
        return a.get() > b.get();
    }

    template <typename T, typename TD, typename U, typename UD>
    bool operator<=(const DumbPtr<T,TD> &a, const DumbPtr<U,UD> &b) {
        return a.get() <= b.get();
    }

    template <typename T, typename TD, typename U, typename UD>
    bool operator>=(const DumbPtr<T,TD> &a, const DumbPtr<U,UD> &b) {
        return a.get() >= b.get();
    }


    template <typename T, typename TD>
    bool operator==(const DumbPtr<T,TD> &a, std::nullptr_t b) {
        return a.get() == b;
    }
    template <typename T, typename TD>
    bool operator==(std::nullptr_t a, const DumbPtr<T,TD> &b) {
        return a == b.get();
    }

    template <typename T, typename TD>
    bool operator!=(const DumbPtr<T,TD> &a, std::nullptr_t b) {
        return a.get() != b;
    }
    template <typename T, typename TD>
    bool operator!=(std::nullptr_t a, const DumbPtr<T,TD> &b) {
        return a != b.get();
    }

    template <typename T, typename TD>
    bool operator<(const DumbPtr<T,TD> &a, std::nullptr_t b) {
        return a.get() < b;
    }
    template <typename T, typename TD>
    bool operator<(std::nullptr_t a, const DumbPtr<T,TD> &b) {
        return a < b.get();
    }

    template <typename T, typename TD>
    bool operator>(const DumbPtr<T,TD> &a, std::nullptr_t b) {
        return a.get() > b;
    }
    template <typename T, typename TD>
    bool operator>(std::nullptr_t a, const DumbPtr<T,TD> &b) {
        return a > b.get();
    }

    template <typename T, typename TD>
    bool operator<=(const DumbPtr<T,TD> &a, std::nullptr_t b) {
        return a.get() <= b;
    }
    template <typename T, typename TD>
    bool operator<=(std::nullptr_t a, const DumbPtr<T,TD> &b) {
        return a <= b.get();
    }

    template <typename T, typename TD>
    bool operator>=(const DumbPtr<T,TD> &a, std::nullptr_t b) {
        return a.get() >= b;
    }
    template <typename T, typename TD>
    bool operator>=(std::nullptr_t a, const DumbPtr<T,TD> &b) {
        return a >= b.get();
    }
}

#endif // D_DUMB_PTR_H
