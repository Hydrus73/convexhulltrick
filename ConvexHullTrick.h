/**
 * This contains two implementations of the convex hull trick algorithm. One only works if the lines are sorted in decreasing order (faster), and the other works for any order of lines (more versatile).
 */

#include <bits/stdc++.h>
class Line {
   public:
    long double slope;
    long double y_intercept;
    Line(long double slope, long double y_intercept) : slope{slope}, y_intercept{y_intercept} {}
    long double value_at(long double x) const {
        return slope * x + y_intercept;
    }
    long double inverse(long double y) const {
        return (y - y_intercept) / slope;
    }
    long double intersection(Line other) const {
        return (other.y_intercept - y_intercept) / (slope - other.slope);
    }
    /**
     * @param higher The line with a higher slope than this one.
     * @param lower The line with a lower slope than this one.
     * @return A bool if this line is redundant (never the minimum).
     */
    bool is_redundant(Line higher, Line lower) const {
        return higher.intersection(lower) <= higher.intersection(*this);
    }
};
bool operator<(Line first, Line second) {
    return first.slope < second.slope;
}
class ConvexHullTrick {
   public:
    class DecreasingOrder {
       public:
        std::deque<Line> lines;
        // only works on decreasing line slope values
        void add_line(Line line) {
            while (lines.size() >= 2) {
                Line last = lines.back();
                lines.pop_back();
                Line next_to_last = lines.back();
                if (!last.is_redundant(next_to_last, line)) {
                    lines.push_back(last);
                    break;
                }
            }
            lines.push_back(line);
        }
        // only works on increasing x values
        long double min_value(long double x) {
            while (lines.size() >= 2) {
                Line first = lines.front();
                lines.pop_front();
                Line second = lines.front();
                if (second.value_at(x) > first.value_at(x)) {
                    lines.push_front(first);
                    break;
                }
            }
            return lines.front().value_at(x);
        }
    };
    class DynamicOrder {
       public:
        std::set<Line> lines;
        std::set<Line>::iterator get_lower(Line line) {
            if (lines.size() == 0) {
                return lines.end();
            }
            std::set<Line>::iterator lower_bound = lines.lower_bound(line);
            if (lower_bound == lines.begin()) {
                return lines.end();
            }
            return --lower_bound;
        }
        // works on slopes of unique values in any order
        void add_line(Line line) {
            std::set<Line>::iterator higher = lines.upper_bound(line);
            std::set<Line>::iterator lower = get_lower(line);
            if (higher == lines.end() || lower == lines.end() || !line.is_redundant(*higher, *lower)) {
                lines.insert(line);
                std::set<Line>::iterator prev_to_higher = lines.upper_bound(*higher);
                while (higher != lines.end() && prev_to_higher != lines.end() && higher->is_redundant(*prev_to_higher, line)) {
                    Line prev = *prev_to_higher;
                    lines.erase(higher);
                    higher = lines.find(prev);
                    prev_to_higher = lines.upper_bound(*higher);
                }
                lower = get_lower(line);
                std::set<Line>::iterator prev_to_lower = get_lower(*lower);
                while (lower != lines.end() && prev_to_lower != lines.end() && lower->is_redundant(line, *prev_to_lower)) {
                    Line prev = *prev_to_lower;
                    lines.erase(lower);
                    lower = lines.find(prev);
                    prev_to_lower = get_lower(*lower);
                }
            }
        }
        // only works on increasing x values
        long double min_value(long double x) {
            while (lines.size() >= 2) {
                std::set<Line>::iterator higher = --lines.end();
                std::set<Line>::iterator lower = --(--lines.end());
                if (higher->value_at(x) < lower->value_at(x)) {
                    break;
                }
                lines.erase(--lines.end());
            }
            return (--lines.end())->value_at(x);
        }
    };
};