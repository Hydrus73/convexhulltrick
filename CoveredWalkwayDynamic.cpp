/**
 * This solution to the Covered Walkway problem uses my implementation of the dynamic slope convex hull trick.
 *
 * https://open.kattis.com/problems/coveredwalkway
 */

#include <bits/stdc++.h>

#include "ConvexHullTrick.h"
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int point_count, constant_cost;
    std::cin >> point_count >> constant_cost;
    ConvexHullTrick::DynamicOrder convex_hull_trick;
    long long lowest_cost = 0;
    for (int i = 0; i < point_count; i++) {
        long long point;
        std::cin >> point;
        Line new_line(-2 * point, point * point + lowest_cost + constant_cost);
        convex_hull_trick.add_line(new_line);
        lowest_cost = point * point + (long long)convex_hull_trick.min_value(point);
    }
    std::cout << lowest_cost << std::endl;
}