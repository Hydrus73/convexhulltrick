/**
 * This solution to the Yet Another Tree Problem problem uses my implementation of the decreasing slope convex hull trick.
 *
 * https://open.kattis.com/problems/yatp
 */

#include <bits/stdc++.h>

#include "ConvexHullTrick.h"
#define MAX_LOG 18
int get_sizes(int node, int parent, std::vector<bool>& found_centroid, std::vector<int>& sizes, std::vector<std::vector<std::pair<int, long long>>>& adj) {
    int size = 1;
    for (std::pair<int, long long> p : adj[node]) {
        if (p.first == parent || found_centroid[p.first]) {
            continue;
        }
        size += get_sizes(p.first, node, found_centroid, sizes, adj);
    }
    sizes[node] = size;
    return size;
}
int get_centroid(int node, int parent, int total_nodes, std::vector<bool>& found_centroid, std::vector<int>& sizes, std::vector<std::vector<std::pair<int, long long>>>& adj) {
    int largest_child = -1;
    int size = 0;
    for (std::pair<int, long long> p : adj[node]) {
        if (p.first == parent || found_centroid[p.first]) {
            continue;
        }
        if (size < sizes[p.first]) {
            largest_child = p.first;
            size = sizes[p.first];
        }
    }
    if (largest_child == -1 || size <= total_nodes / 2) {
        return node;
    }
    return get_centroid(largest_child, node, total_nodes, found_centroid, sizes, adj);
}
int get_centroid_decomposition(int node, int previous, std::vector<int>& sizes, std::vector<bool>& found_centroid, std::vector<std::vector<std::pair<int, long long>>>& adj, std::vector<std::vector<int>>& centroid_decomposition, std::vector<int>& parent) {
    get_sizes(node, -1, found_centroid, sizes, adj);
    int centroid = get_centroid(node, -1, sizes[node], found_centroid, sizes, adj);
    found_centroid[centroid] = true;
    parent[centroid] = previous;
    for (std::pair<int, long long> p : adj[centroid]) {
        if (found_centroid[p.first]) {
            continue;
        }
        centroid_decomposition[centroid].push_back(get_centroid_decomposition(p.first, centroid, sizes, found_centroid, adj, centroid_decomposition, parent));
    }
    return centroid;
}
void get_depth_and_distance(int node, int parent, int edge_weight, std::vector<std::vector<std::pair<int, long long>>>& adj, std::vector<int>& depth, std::vector<long long>& distance, std::vector<std::vector<int>>& parent_table) {
    if (parent == -1) {
        depth[node] = 0;
        distance[node] = 0;
        parent_table[node][0] = -1;
    } else {
        depth[node] = depth[parent] + 1;
        distance[node] = distance[parent] + edge_weight;
        parent_table[node][0] = parent;
    }
    for (std::pair<int, long long> p : adj[node]) {
        if (p.first == parent) {
            continue;
        }
        get_depth_and_distance(p.first, node, p.second, adj, depth, distance, parent_table);
    }
}
void fill_parent_table(std::vector<std::vector<int>>& parent_table) {
    for (int i = 1; i < MAX_LOG; i++) {
        for (unsigned node = 0; node < parent_table.size(); node++) {
            if (parent_table[node][i - 1] == -1) {
                parent_table[node][i] = -1;
            } else {
                parent_table[node][i] = parent_table[parent_table[node][i - 1]][i - 1];
            }
        }
    }
}
long long get_distance(int node1, int node2, std::vector<int>& depth, std::vector<long long>& distance, std::vector<std::vector<int>>& parent_table) {
    long long dist = distance[node1] + distance[node2];
    if (depth[node2] < depth[node1]) {
        std::swap(node1, node2);
    }
    int depth_diff = depth[node2] - depth[node1];
    for (int i = 0; i < MAX_LOG; i++) {
        if ((1 << i) & depth_diff) {
            node2 = parent_table[node2][i];
        }
    }
    int lca = node1;
    if (node1 != node2) {
        for (int i = MAX_LOG - 1; i >= 0; i--) {
            if (parent_table[node1][i] != parent_table[node2][i]) {
                node1 = parent_table[node1][i];
                node2 = parent_table[node2][i];
            }
        }
        lca = parent_table[node1][0];
    }
    return dist - 2 * distance[lca];
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int nodes;
    std::cin >> nodes;
    std::vector<long long> penalties(nodes);
    std::vector<long long> dp(nodes);
    for (int i = 0; i < nodes; i++) {
        std::cin >> penalties[i];
        dp[i] = penalties[i] * penalties[i];
    }
    std::vector<std::vector<std::pair<int, long long>>> adj(nodes);
    for (int i = 0; i < nodes - 1; i++) {
        int u, v;
        long long weight;
        std::cin >> u >> v >> weight;
        --u;
        --v;
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight});
    }
    std::vector<int> depth(nodes);
    std::vector<long long> distance(nodes);
    std::vector<std::vector<int>> parent_table(nodes, std::vector<int>(MAX_LOG));
    get_depth_and_distance(0, -1, -1, adj, depth, distance, parent_table);
    fill_parent_table(parent_table);
    std::vector<std::vector<int>> centroid_decomposition(nodes);
    std::vector<bool> found_centroid(nodes, false);
    std::vector<int> sizes(nodes);
    std::vector<int> parent(nodes, -1);
    get_centroid_decomposition(0, -1, sizes, found_centroid, adj, centroid_decomposition, parent);
    std::vector<std::vector<std::pair<int, long long>>> children(nodes);
    for (int i = 0; i < nodes; i++) {
        for (int current = parent[i]; current != -1; current = parent[current]) {
            children[current].push_back({i, get_distance(current, i, depth, distance, parent_table)});
        }
    }
    for (int i = 0; i < nodes; i++) {
        ConvexHullTrick::DecreasingOrder cht;
        std::sort(children[i].begin(), children[i].end(), [&penalties](std::pair<int, long long> j, std::pair<int, long long> k) {
            if (penalties[j.first] == penalties[k.first]) {
                return j.second < k.second;
            }
            return penalties[j.first] > penalties[k.first];
        });
        long long penalty = -1;
        for (std::pair<int, long long> child : children[i]) {
            long long dist = child.second;
            dp[i] = std::min(dp[i], dist + penalties[child.first] * penalties[i]);
            dp[child.first] = std::min(dp[child.first], dist + penalties[child.first] * penalties[i]);
            if (penalty != penalties[child.first]) {
                cht.add_line(Line(penalties[child.first], dist));
            }
            penalty = penalties[child.first];
        }
        std::sort(children[i].begin(), children[i].end(), [&penalties](std::pair<int, long long> j, std::pair<int, long long> k) {
            if (penalties[j.first] == penalties[k.first]) {
                return j.second < k.second;
            }
            return penalties[j.first] < penalties[k.first];
        });
        for (std::pair<int, long long> child : children[i]) {
            long long dist = child.second;
            dp[child.first] = std::min(dp[child.first], dist + (long long)cht.min_value(penalties[child.first]));
        }
    }
    long long answer = 0;
    for (int i = 0; i < nodes; i++) {
        answer += dp[i];
    }
    std::cout << answer << std::endl;
}