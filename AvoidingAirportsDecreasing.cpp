/**
 * This solution to the Avoiding Airports problem uses my implementation of the decreasing slope convex hull trick.
 *
 * https://open.kattis.com/problems/avoidingairports
 */

#include <bits/stdc++.h>

#include "ConvexHullTrick.h"
class Flight {
   public:
    int country1, country2;
    long long time1, time2;
    long long frustration = -1;
    Flight(int country1, int country2, long long time1, long long time2) : country1{country1}, country2{country2}, time1{time1}, time2{time2} {}
};
class Event {
   public:
    bool arriving;
    int flight;
    long long time;
    Event(bool arriving, int flight, long long time) : arriving{arriving}, flight{flight}, time{time} {}
    bool operator<(Event other) {
        return time < other.time;
    }
};
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int countries, flights;
    std::cin >> countries >> flights;
    std::vector<ConvexHullTrick::DecreasingOrder> chts(countries);
    std::vector<Event> events;
    std::vector<Flight> flight_list;
    for (int i = 0; i < flights; i++) {
        int country1, country2;
        long long time1, time2;
        std::cin >> country1 >> country2 >> time1 >> time2;
        --country1;
        --country2;
        flight_list.push_back(Flight(country1, country2, time1, time2));
        events.push_back(Event(false, i, time1));
        events.push_back(Event(true, i, time2));
    }
    chts[0].add_line(Line(0, 0));
    sort(events.begin(), events.end());
    for (Event event : events) {
        if (!event.arriving) {
            if (chts[flight_list[event.flight].country1].lines.empty()) {
                continue;
            }
            flight_list[event.flight].frustration = event.time * event.time + (long long)chts[flight_list[event.flight].country1].min_value(event.time);
        } else {
            if (flight_list[event.flight].frustration == -1) {
                continue;
            }
            chts[flight_list[event.flight].country2].add_line(Line(-2 * event.time, event.time * event.time + flight_list[event.flight].frustration));
        }
    }
    long long min_frustration = std::numeric_limits<long long>::max();
    for (Flight flight : flight_list) {
        if (flight.country2 == countries - 1 && flight.frustration != -1) {
            min_frustration = std::min(min_frustration, flight.frustration);
        }
    }
    std::cout << min_frustration << std::endl;
}