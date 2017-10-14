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

#include "catch.hpp"
#include "../src/list.hpp"

TEST_CASE("List can be initialized with arbitrary number of integers") {
    auto x = pds::list(1);
    auto y = pds::list(1, 2);
    REQUIRE(pds::len(x) == 1);
    REQUIRE(pds::len(y) == 2);
}

TEST_CASE("List items can be get with indexing") {
    auto y = pds::list(1);
    REQUIRE(y[0] == 1);

    auto x = pds::list(1, 2, 3);
    REQUIRE(pds::len(x) == 3);
    REQUIRE(x[0] == 1);
    REQUIRE(x[1] == 2);
    REQUIRE(x[2] == 3);
}

TEST_CASE("Zero-sized list should be supported") {
    auto x = pds::list();
    REQUIRE(pds::len(x) == 0);
}

TEST_CASE("Can add items to zero-length list") {
    auto x = pds::list();
    for(int i = 0 ; i < 10000 ; i++) {
        x.append(i);
    }
    REQUIRE(pds::len(x) == 10000);
    REQUIRE(x[0] == 0);
    REQUIRE(x[1234] == 1234);
    REQUIRE(x[9999] == 9999);
}