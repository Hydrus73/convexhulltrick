#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <bits/stdc++.h>

#include "ConvexHullTrick.h"
#include "doctest.h"
#define ERROR 0.0000000001L
using namespace std;
TEST_CASE("Testing the Line class") {
    Line line1(1, 10);
    for (int val = 0; val < 100; val++) {
        CHECK(line1.value_at(val) == val + 10);
        CHECK(line1.inverse(val) == val - 10);
    }
    Line line2(-2, 5);
    for (int val = 0; val < 100; val++) {
        CHECK(line2.value_at(val) == -2 * val + 5);
        CHECK(line2.inverse(val) == (long double)(val - 5) / -2);
    }
    long double intersection1 = -5.0 / 3.0;
    CHECK(abs(line1.intersection(line2) - intersection1) <= ERROR);
    CHECK(abs(line2.intersection(line1) - intersection1) <= ERROR);
    Line line3(5.33546654, -6.2982374);
    long double intersection2 = 16.2982374 / 4.33546654;
    CHECK(abs(line1.intersection(line3) - intersection2) <= ERROR);
    CHECK(abs(line3.intersection(line1) - intersection2) <= ERROR);
    CHECK(line1.is_redundant(line2, line3) == false);
    Line line4(1, 5);
    Line line5(2, 5);
    Line line6(3, 5);
    CHECK(line5.is_redundant(line6, line4) == true);
}
TEST_CASE("Testing the convex hull trick algorithm") {
    SUBCASE("Testing the dynamic convex hull trick") {
        ConvexHullTrick::DynamicOrder cht;
        for (long double i = 1; i <= 100; i++) {
            cht.add_line({i, 0});
        }
        REQUIRE(cht.lines.size() == 2);
        Line lowest = *cht.lines.begin();
        Line highest = *++cht.lines.begin();
        CHECK(lowest.slope == 1);
        CHECK(lowest.y_intercept == 0);
        CHECK(highest.slope == 100);
        CHECK(highest.y_intercept == 0);
        CHECK(abs(cht.min_value(-2) + 200) <= ERROR);
        CHECK(abs(cht.min_value(3) - 3) <= ERROR);
        ConvexHullTrick::DynamicOrder cht2;
        for (long double i = 1; i <= 100; i++) {
            cht2.add_line({i, i * i});
        }
        REQUIRE(cht2.lines.size() == 100);
        for (Line line : cht2.lines) {
            long double correct = line.slope * line.slope;
            CHECK(correct == line.y_intercept);
        }
        CHECK(abs(cht2.min_value(-5.8) + 8.4) <= ERROR);
        REQUIRE(cht2.lines.size() == 3);
        CHECK(abs(cht2.min_value(-4) + 4) <= ERROR);
        REQUIRE(cht2.lines.size() == 2);
        CHECK(abs(cht2.min_value(10) - 11) <= ERROR);
        REQUIRE(cht2.lines.size() == 1);
        CHECK(abs(cht2.min_value(-1000) + 999) <= ERROR);
    }
    SUBCASE("Testing the decreasing convex hull trick") {
        ConvexHullTrick::DecreasingOrder cht;
        for (long double i = 100; i >= 1; i--) {
            cht.add_line({i, 0});
        }
        REQUIRE(cht.lines.size() == 2);
        Line highest = *cht.lines.begin();
        Line lowest = *++cht.lines.begin();
        CHECK(lowest.slope == 1);
        CHECK(lowest.y_intercept == 0);
        CHECK(highest.slope == 100);
        CHECK(highest.y_intercept == 0);
        CHECK(abs(cht.min_value(-2) + 200) <= ERROR);
        CHECK(abs(cht.min_value(3) - 3) <= ERROR);
        ConvexHullTrick::DecreasingOrder cht2;
        for (long double i = 100; i >= 1; i--) {
            cht2.add_line({i, i * i});
        }
        REQUIRE(cht2.lines.size() == 100);
        for (Line line : cht2.lines) {
            long double correct = line.slope * line.slope;
            CHECK(correct == line.y_intercept);
        }
        CHECK(abs(cht2.min_value(-5.8) + 8.4) <= ERROR);
        REQUIRE(cht2.lines.size() == 3);
        CHECK(abs(cht2.min_value(-4) + 4) <= ERROR);
        REQUIRE(cht2.lines.size() == 2);
        CHECK(abs(cht2.min_value(10) - 11) <= ERROR);
        REQUIRE(cht2.lines.size() == 1);
        CHECK(abs(cht2.min_value(-1000) + 999) <= ERROR);
    }
}
