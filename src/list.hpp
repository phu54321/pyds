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
    template <typename T>
    class _list {
    public:
        using value_type = T;

        /**
         * Constructor with nothing.
         */
        explicit _list() {
            resizeTo(1);
        }

        /**
         * Constructor with multiple parameters
         *
         * @tparam Types  For variadic arguments
         * @param args  Initializer
         */
        explicit _list(std::initializer_list<T> arguments) {
            resizeTo(arguments.size());
            extend(arguments);
        }

        /**
         * Get value by index
         * @param index
         * @return
         */
        const T& operator[](size_t index) const {
            if(index >= size()) throw std::runtime_error("list index out of range");
            return buffer[index];
        }

        /**
         * Append object to end
         * @param item  Item to put on
         */
        void append(const T& item) {
            checkCapacity(cursor + 1);
            buffer[cursor] = item;
            cursor++;
        }

        /** Extend object by some items */
        void extend(std::initializer_list<T> arguments) {
            checkCapacity(cursor + arguments.size());
            for(const auto& x: arguments) append(x);
        }

        /** Get container size */
        size_t size() const {
            return cursor;
        }

        /** Equality check */
        bool operator==(const _list<T>& other) {
            if(size() != other.size()) return false;
            return std::equal(buffer, buffer + capacity, other.buffer);
        }

    public:
        // For C++-style range-based for support
        T* begin() { return buffer; }
        T* end() { return buffer + cursor; }
        const T* begin() const { return buffer; }
        const T* end() const { return buffer + cursor; }

    private:
        /**
         * Resize capacity to
         * @param newCapacity
         */
        void resizeTo(size_t newCapacity) {
            if(newCapacity < capacity) {
                throw std::runtime_error("Bad things always happen");
            }

            auto newBuffer = new T[newCapacity];
            std::copy(buffer, buffer + capacity, newBuffer);
            capacity = newCapacity;
            delete[] buffer;
            buffer = newBuffer;
        }

        /**
         * Automatically resize if needed
         * @param requiredSize Resize!
         */
        void checkCapacity(size_t requiredSize) {
            if(capacity < requiredSize) {
                size_t newCapacity = capacity;
                while(newCapacity < requiredSize) {
                    newCapacity *= 2;
                }
                resizeTo(newCapacity);
            }
        }

    private:
        size_t cursor = 0;
        size_t capacity = 0;
        T* buffer = nullptr;
    };

    /** List constructor from iterable */
    template <typename Iterable>
    static _list<typename Iterable::value_type> list(const Iterable& container) {
        _list<typename Iterable::value_type> list;
        for(const auto& x: container) {
            list.append(x);
        }
        return list;
    }

    /** List constructor for array */
    template <typename T, size_t N>
    static _list<T> list(const T(&array)[N]) {
        _list<T> list;
        for(const auto& x: array) {
            list.append(x);
        }
        return list;
    }

    /** List constructor for c string */
    static _list<char> list(const char* string) {
        _list<char> list;
        for(const char* p = string ; *p ; p++) {
            list.append(*p);
        }
        return list;
    }

    /**
     * General constructor for list
     *
     * Note) C++ doesn't support template type deduction for class,
     *       so we need an helper function.
     */
    template <typename T, typename... Args>
    static _list<T> list(const T& arg, Args... args) {
        return _list<T>({arg, args...});
    }

    /** Empty constructor **/
    template <typename T>
    static _list<T> list() {
        return _list<T>();
    }


    /**
     * General length getter
     * @param l
     * @return
     */
    template <typename Container>
    static size_t len(const Container& l) {
        return l.size();
    }
}
