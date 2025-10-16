/**
 * This solution to the Longest Life problem uses my implementation of the dynamic slope convex hull trick.
 *
 * https://open.kattis.com/problems/longestlife
 */

#include <bits/stdc++.h>

#include "ConvexHullTrick.h"
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    long double total_time;
    std::cin >> total_time;
    int total_pills;
    std::cin >> total_pills;
    long double constant;
    std::cin >> constant;
    ConvexHullTrick::DynamicOrder convex_hull_trick;
    convex_hull_trick.add_line(Line(1, 0));
    long double max_age = total_time;
    for (int i = 0; i < total_pills; i++) {
        long double time, x, y;
        std::cin >> time >> x >> y;
        long double slope = y / x;
        long double current_age = convex_hull_trick.min_value(time);
        long double age_at_switch = current_age + constant;
        long double y_intercept = age_at_switch - time * slope;
        Line new_line(slope, y_intercept);
        convex_hull_trick.add_line(new_line);
        max_age = std::max(max_age, new_line.inverse(total_time));
    }
    std::cout << std::fixed << std::setprecision(16) << max_age << std::endl;
}