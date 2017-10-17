// Copyright 2017 Hyun Woo Park
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <algorithm>
#include <limits>
#include "common.hpp"

namespace pds {
    template<typename T>
    class _list {
    public:
        using value_type = T;

        _list() = default;

        _list(std::initializer_list<T> arguments) : impl(arguments) {}

        _list(const _list<T> &other) : impl(other.impl) {}

        /* Get container size */
        int size() const {
            return static_cast<int>(impl.size());
        }

        /* Equality check */
        bool operator==(const _list<T> &other) {
            return other.impl == impl;
        }

        bool operator!=(const _list<T> &other) {
            return !(*this == other);
        }

        void reserve(size_t newSize) {
            impl.reserve(newSize);
        }


    private:
        void wrapIndex(int &index) const {
            if (index < 0) index += impl.size();
        }

        void wrapAndCheckIndex(int &index) const {
            wrapIndex(index);
            if (index < 0 || index >= impl.size()) {
                throw std::runtime_error("Out of bounds");
            }
        }

        void wrapAndClampIndex(int &index) const {
            wrapIndex(index);
            if (index < 0) index = 0;
            else if (index > impl.size()) index = static_cast<int>(impl.size());
        }

    public:

        /* Indexing operations */
        const T &operator[](int index) const {
            wrapAndCheckIndex(index);
            return impl[index];
        }

        T &operator[](int index) {
            wrapAndCheckIndex(index);
            return impl[index];
        }

        _list<T> operator()(int begin, int end) {
            wrapAndClampIndex(begin);
            wrapAndClampIndex(end);

            _list l;
            l.impl.insert(l.impl.begin(), this->begin() + begin, this->begin() + end);
            return l;
        }

        _list<T> operator()(int begin, int end, int step) {
            wrapAndClampIndex(begin);
            wrapAndClampIndex(end);

            if (step > 0) {
                _list l;
                int expectedSize = (end - begin) / step;
                if (expectedSize < 0) return l;

                l.reserve(static_cast<size_t>(expectedSize));
                for (int i = begin; i < end; i += step) {
                    l.append(impl[i]);
                }
                return l;
            } else if (step < 0) {
                _list l;
                int expectedSize = (begin - end) / (-step);
                if (expectedSize < 0) return l;

                l.reserve(static_cast<size_t>(expectedSize));
                for (int i = begin; i > end; i += step) {
                    l.append(impl[i]);
                }
                return l;
            } else throw std::runtime_error("slice step cannot be zero");
        }

    public:
        // PYTHON API
        void append(const T &item) { impl.push_back(item); }

        /* list.extend(iterable) */
        template<typename Container>
        void extend(Container container) {
            impl.insert(impl.end(), std::begin(container), std::end(container));
        }

        /* list.insert(i, x) */
        void insert(int index, const T &item) {
            impl.insert(impl.begin() + index, item);
        }

        /* list.remove(x) */
        void remove(int index) {
            impl.erase(impl.begin() + index);
        }

        /* list.pop([i]) */
        T pop() {
            return pop(size() - 1);
        }

        T pop(int index) {
            T ret = impl[index];
            remove(index);
            return ret;
        }

        /* list.clear() */
        void clear() {
            impl.clear();
        }

        /* list.index(x) */
        int index(const T &item) {
            return index(item, 0, size() - 1);
        }

        /* list.index(x, start) */
        int index(const T &item, int start) {
            return index(item, start, size() - 1);
        }

        /* list.index(x[, start[, end]]) */
        int index(const T &item, int start, int end) {
            wrapAndClampIndex(start);
            wrapAndClampIndex(end);
            for (int i = start; i < end; i++) {
                if (impl[i] == item) return i;
            }
            return -1;
        }

        /* list.count(x) */
        int count(const T &item) {
            return static_cast<int>(std::count(impl.begin(), impl.end(), item));
        }

        /* list.sort(key=None, reverse=False) */
        void sort() {
            std::sort(begin(), end());
        }

        template<typename Keyfunc>
        void sort(Keyfunc keyf) {
            std::sort(begin(), end(), [&keyf](const T &a1, const T &a2) {
                return keyf(a1) < keyf(a2);
            });
        }

        /* list.reverse() */
        void reverse() {
            std::reverse(impl.begin(), impl.end());
        }

        /* list.copy() */
        _list<T> copy() {
            return *this;
        }


    public:
        // For C++-style range-based for support
        auto begin() { return impl.begin(); }

        auto end() { return impl.end(); }

        auto begin() const { return impl.begin(); }

        auto end() const { return impl.end(); }

    private:
        std::vector<T> impl;
    };

    /* List constructor from iterable */
    template<typename Iterable>
    static _list<typename Iterable::value_type> list(const Iterable &container) {
        _list<typename Iterable::value_type> list;
        list.extend(container);
        return list;
    }

    /* List constructor for array */
    template<typename T, size_t N>
    static _list<T> list(const T(&array)[N]) {
        _list<T> list;
        list.reserve(N);
        for (const auto &x: array) {
            list.append(x);
        }
        return list;
    }

    /* List constructor for c string */
    static _list<char> list(const char *string) {
        _list<char> list{};
        for (const char *p = string; *p; p++) {
            list.append(*p);
        }
        return list;
    }

    /* Variadic constructor for list */
    template<typename T, typename... Args>
    static _list<T> list(const T &arg, Args... args) {
        // For automatic type deduction, first argument is typed as T.
        return _list<T>({arg, args...});
    }

    /* Empty constructor **/
    template<typename T>
    static _list<T> list() {
        return _list<T>();
    }
}
