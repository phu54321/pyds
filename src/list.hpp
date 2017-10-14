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

namespace pds {
    class list {
    public:
        /**
         * Constructor with multiple parameters
         *
         * @tparam Types  For variadic arguments
         * @param args  Initializer
         */
        template <class... Types> explicit list(Types... args) {
            buffer = new int[sizeof...(args)];
            extend({args...});
        }

        /**
         * Get value by index
         * @param index
         * @return
         */
        int operator[](size_t index) const {
            return buffer[index];
        }

        /**
         * Append object to end
         * @param item  Item to put on
         */
        void append(int item) {
            buffer[cursor] = item;
            cursor++;
        }

        /**
         * Extend object by some arguments
         * @param arguments
         */
        void extend(std::initializer_list<int> arguments) {
            for(const auto& x: arguments) append(x);
        }

        /**
         * Get component size
         * @return
         */
        size_t size() const {
            return cursor;
        }

    private:
        size_t cursor = 0;
        int* buffer;
    };

    /**
     * General length getter
     * @param l
     * @return
     */
    static size_t len(const list& l) {
        return l.size();
    }
}
