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

#include <list>
#include "catch.hpp"
#include "../src/list.hpp"

using namespace pds;

TEST_CASE("List can be initialized with arbitrary number of integers") {
    auto x = list(1);
    auto y = list(1, 2);
    REQUIRE(len(x) == 1);
    REQUIRE(len(y) == 2);
}

TEST_CASE("List items can be get with indexing") {
    auto y = list(1);
    REQUIRE(y[0] == 1);

    auto x = list(1, 2, 3);
    REQUIRE(len(x) == 3);
    REQUIRE(x[0] == 1);
    REQUIRE(x[1] == 2);
    REQUIRE(x[2] == 3);
}

TEST_CASE("Zero-sized list should be supported") {
    auto x = list<int>();
    REQUIRE(len(x) == 0);
}

TEST_CASE("Can add items to zero-length list") {
    auto x = list<int>();
    for(int i = 0 ; i < 10000 ; i++) {
        x.append(i);
    }
    REQUIRE(len(x) == 10000);
    REQUIRE(x[0] == 0);
    REQUIRE(x[1234] == 1234);
    REQUIRE(x[9999] == 9999);
}

TEST_CASE("Testing for python methods") {
    SECTION("Test for insert, remove, pop, clear method") {
        auto x = list(1, 2, 3, 4, 5);
        x.insert(1, 4);
        REQUIRE(x == list(1, 4, 2, 3, 4, 5));

        x.remove(2);
        REQUIRE(x == list(1, 4, 3, 4, 5));

        auto y1 = x.pop();
        REQUIRE(y1 == 5);
        REQUIRE(x == list(1, 4, 3, 4));

        auto y2 = x.pop(2);
        REQUIRE(y2 == 3);
        REQUIRE(x == list(1, 4, 4));

        x.clear();
        REQUIRE(len(x) == 0);
    }

    SECTION("Test for index method") {
        auto x = list(1, 2, 3, 4, 5);
        REQUIRE(x.index(3) == 2);
        REQUIRE(x.index(6) == -1);  // For non-existing

        // index() with start
        REQUIRE(x.index(3, 3) == -1);
        REQUIRE(x.index(3, 2) == 2);

        // Three-parameter index
        REQUIRE(x.index(1, 1, 4) == -1);  // Value outside region
        REQUIRE(x.index(3, 1, 4) == 2);
        REQUIRE(x.index(3, 0, 5) == 2);
        REQUIRE(x.index(3, 0, 1) == -1);  // Value outside region

        REQUIRE(x.index(3, 0, -1) == 2);  // Negative index slicing
        REQUIRE(x.index(3, -4, -1) == 2); // Negative index
        REQUIRE(x.index(3, -4, -3) == -1); // Negative index

        // For class with non-trivial equivalence
        auto y = list<std::string>("a", "b", "c", "d", "e");
        REQUIRE(y.index("c") == 2);
    }

    SECTION("Test for count method") {
        auto x = list(1, 2, 3, 1, 2, 4, 1, 2, 5);
        REQUIRE(x.count(1) == 3);
        REQUIRE(x.count(3) == 1);
        REQUIRE(x.count(7) == 0);
    }

    SECTION("Test for sort method") {
        auto x1 = list(1, 7, 3, 5, 5, 2, 9, 4);
        x1.sort();
        REQUIRE(x1 == list(1, 2, 3, 4, 5, 5, 7, 9));

        auto x2 = list(1, 7, 3, 5, 5, 2, 9, 4);
        x2.sort([](int x) { return -x; });
        REQUIRE(x2 == list(9, 7, 5, 5, 4, 3, 2, 1));
    }

    SECTION("Test for reverse and copy method") {
        auto x = list(1, 2, 3, 4, 5);
        x.reverse();
        REQUIRE(x == list(5, 4, 3, 2, 1));

        // Create a copy
        auto y = x.copy();
        REQUIRE(x == y);

        // Assure that this is indeed a copy, not just a reference
        y[0] = 4;
        REQUIRE(y == list(4, 4, 3, 2, 1));
        REQUIRE(x == list(5, 4, 3, 2, 1));
        REQUIRE(x != y);
    }
}


TEST_CASE("Basic list slice getter") {
    auto l = list(0, 1, 2, 3, 4, 5, 6);
    CHECK(l[3] == 3);  // Single item by function call
    CHECK(l[-1] == 6); // Negative indexing


    // Binary slicing
    CHECK(l(2, 5) == list(2, 3, 4));  // Simple slicing
    CHECK(l(2, -2) == list(2, 3, 4));  // Slicing with negative index
    CHECK(l(4, 1) == list<int>());  // Slicing backward not supported

    // Ternary slicing
    CHECK(l(2, 5, 2) == list(2, 4));
    CHECK(l(4, 1, -2) == list(4, 2));

    // Clamping
    CHECK(l(-9, 9).size() == 7);
    CHECK(l(-6, 2).size() == 1);

    // Clamping with placeholders
    CHECK(l(_, _) == l);
    CHECK(l(4, _) == list(4, 5, 6));
    CHECK(l(_, 2) == list(0, 1));
    CHECK(l(-3, _) == list(4, 5, 6));
}

TEST_CASE("Basic list slice setter") {
    // Binary slicing assignment
    auto l = list(0, 1, 2, 3, 4, 5, 6);
    l.set(1, 3, list(4));
    REQUIRE(l == list(0, 4, 3, 4, 5, 6));

    // Extension
    auto l2 = list("Test.bmp");
    // Lengthen
    l2.set(-4, _, list(".jpeg"));
    REQUIRE(l2 == list("Test.jpeg"));
    // Shrink
    l2.set(-5, _, list(".qt"));
    REQUIRE(l2 == list("Test.qt"));
    // Equal
    l2.set(-3, _, list(".py"));
    REQUIRE(l2 == list("Test.py"));
}

TEST_CASE("Operators") {
    // Multiplication
    auto l = list(0) * 10;
    CHECK(l == list(0, 0, 0, 0, 0, 0, 0, 0, 0, 0));

    l += list(1);
    CHECK(l == list(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));

}

// Code below is taken from original python unit test for lists.

TEST_CASE("test_basic", "[python-unittest]") {
    SECTION("List can be copied from other iterable") {
        auto l0_3 = list(0, 1, 2, 3);
        auto l0_3_bis = list(l0_3);
        CHECK(l0_3 == l0_3_bis);
    }

    SECTION("Copy construct from other lists") {
        auto empty = list<int>();
        auto a1 = list(std::vector<int>{});
        auto a2 = list(std::list<int>{});
        CHECK(empty == a1);
        CHECK(empty == a2);
    }

    SECTION("Comparing empty char list") {
        auto empty1 = list<char>();
        auto empty2 = list(std::string(""));
        auto empty3 = list("");
        CHECK(empty1 == empty2);
        CHECK(empty1 == empty3);
    }

    SECTION("Initialization from string") {
        auto l1 = list("spam");
        auto l2 = list({'s', 'p', 'a', 'm'});
        CHECK(l1 == l2);
    }
}


TEST_CASE("test_truth", "[python-unittest]") {
    auto l1 = list<int>();
    CHECK(!l1);

    auto l2 = list(42);
    CHECK(l2);
}

TEST_CASE("test_len", "[python-unittest]") {
    CHECK(len(list<int>()) == 0);
    CHECK(len(list(0)) == 1);
    CHECK(len(list(0, 1, 2)) == 3);
}

/// TODO : test_repr_large
