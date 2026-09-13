#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "mathutil.h"

TEST_CASE("fib 迭代斐波那契") {
    CHECK(tri_fib(0) == 0);
    CHECK(tri_fib(1) == 1);
    CHECK(tri_fib(2) == 1);
    CHECK(tri_fib(10) == 55);
    CHECK(tri_fib(20) == 6765);
}

TEST_CASE("gcd 辗转相除") {
    CHECK(tri_gcd(1071, 462) == 21);
    CHECK(tri_gcd(17, 5) == 1);
    CHECK(tri_gcd(12, 18) == 6);
    CHECK(tri_gcd(0, 7) == 7);
}
