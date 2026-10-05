#pragma once

#include <random>
#include <iostream>
#include <vector>
#include <algorithm>

inline int randomRange(std::mt19937& prng, int min, int max) {
    int m, mm;
    m = std::min(min, max);
    mm = std::max(min, max);
    std::uniform_int_distribution<int> dist(m, mm);
    return dist(prng);
}

inline float lerp(float start, float end, float t) {
    return start + t * (end - start);
}

struct Edge {
    int s;
    int d;
    float weight;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class DisjointSet {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }

    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);

        if (root_i != root_j) {
            if (rank[root_i] < rank[root_j]) {
                parent[root_i] = root_j;
            } else if (rank[root_i] > rank[root_j]) {
                parent[root_j] = root_i;
            } else {
                parent[root_j] = root_i;
                rank[root_i]++;
            }
        }
    }
};

inline float calcWeight(const Room& r1, const Room& r2) {
    float c1x = r1.x + r1.w / 2.0f;
    float c1y = r1.y + r1.h / 2.0f;
    float c2x = r2.x + r2.w / 2.0f;
    float c2y = r2.y + r2.h / 2.0f;
    return std::sqrt((c1x - c2x) * (c1x - c2x) + (c1y - c2y) * (c1y - c2y));
}

inline std::vector<Edge> edgeCalculator(const std::vector<Room>& rooms){
    std::vector<Edge> ret;
    if (rooms.size() < 2) return ret;

    for(int i = 0; i < (int)rooms.size() - 1; i++){
        for(int j = i+1; j < (int)rooms.size(); j++){
            ret.push_back(Edge{i, j, calcWeight(rooms.at(i), rooms.at(j))});
        }
    }

    return ret;
}

inline std::vector<Edge> kruskalMST(int vertices, std::vector<Edge>& edges) {
    std::vector<Edge> mst; 
    std::sort(edges.begin(), edges.end());

    DisjointSet ds(vertices);

    for (const auto& edge : edges) {
        int set_u = ds.find(edge.s);
        int set_v = ds.find(edge.d);

        if (set_u != set_v) {
            mst.push_back(edge);
            ds.unite(set_u, set_v);
        }

        if (mst.size() == static_cast<size_t>(vertices - 1)) {
            break;
        }
    }

    return mst;
}