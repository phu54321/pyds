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

namespace pds {
    template<typename T>
    class _list {
    public:
        using value_type = T;

        _list() = default;

        _list(std::initializer_list<T> arguments) {
            extend(arguments);
        }

        // Indexing operatrions
        const T &operator[](size_t index) const { return impl[index]; }

        T &operator[](size_t index) { return impl[index]; }

        /** Get container size */
        size_t size() const {
            return impl.size();
        }

        /** Equality check */
        bool operator==(const _list<T> &other) {
            return other.impl == impl;
        }

    public:
        // PYTHON API
        void append(const T &item) { impl.push_back(item); }

        template<typename Container>
        void extend(Container container) {
            impl.insert(impl.end(), std::begin(container), std::end(container));
        }

        void reserve(size_t newSize) {
            impl.reserve(newSize);
        }

    public:
        // For C++-style range-based for support
        typename std::vector<T>::iterator begin() { return impl.begin(); }

        typename std::vector<T>::iterator end() { return impl.end(); }

        typename std::vector<T>::const_iterator begin() const { return impl.begin(); }

        typename std::vector<T>::const_iterator end() const { return impl.end(); }

    private:
        std::vector<T> impl;
    };

    /** List constructor from iterable */
    template<typename Iterable>
    static _list<typename Iterable::value_type> list(const Iterable &container) {
        _list<typename Iterable::value_type> list;
        list.extend(container);
        return list;
    }

    /** List constructor for array */
    template<typename T, size_t N>
    static _list<T> list(const T(&array)[N]) {
        _list<T> list;
        list.reserve(N);
        for (const auto &x: array) {
            list.append(x);
        }
        return list;
    }

    /** List constructor for c string */
    static _list<char> list(const char *string) {
        _list<char> list;
        for (const char *p = string; *p; p++) {
            list.append(*p);
        }
        return list;
    }

    /** Variadic constructor for list */
    template<typename T, typename... Args>
    static _list<T> list(const T &arg, Args... args) {
        // For automatic type deduction, first argument is typed as T.
        return _list<T>({arg, args...});
    }

    /** Empty constructor **/
    template<typename T>
    static _list<T> list() {
        return _list<T>();
    }


    /** General length getter */
    template<typename Container>
    static size_t len(const Container &l) {
        return l.size();
    }
}
