#ifndef INCLUDE_KRUSKAL_H_
#define INCLUDE_KRUSKAL_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include "vector.h"

// Структура для хранения ребра
struct Edge {
    int u, v, weight;

    Edge(int u = 0, int v = 0, int weight = 0) : u(u), v(v), weight(weight) {}

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Дискретное множество без сжатия путей
class DisjointSetNoCompression {
public:
    TVector<int> parent;
    TVector<int> rank;

    DisjointSetNoCompression(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int find(int u) {
        while (u != parent[u]) {
            u = parent[u];
        }
        return u;
    }

    void union_sets(int u, int v) {
        int root_u = find(u);
        int root_v = find(v);

        if (root_u != root_v) {
            if (rank[root_u] < rank[root_v]) {
                parent[root_u] = root_v;
            } else if (rank[root_u] > rank[root_v]) {
                parent[root_v] = root_u;
            } else {
                parent[root_v] = root_u;
                rank[root_u]++;
            }
        }
    }
};

// Дискретное множество со сжатием путей
class DisjointSetWithCompression {
public:
    TVector<int> parent;
    TVector<int> rank;

    DisjointSetWithCompression(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int find(int u) {
        if (u != parent[u]) {
            parent[u] = find(parent[u]);
        }
        return parent[u];
    }

    void union_sets(int u, int v) {
        int root_u = find(u);
        int root_v = find(v);

        if (root_u != root_v) {
            if (rank[root_u] < rank[root_v]) {
                parent[root_u] = root_v;
            } else if (rank[root_u] > rank[root_v]) {
                parent[root_v] = root_u;
            } else {
                parent[root_v] = root_u;
                rank[root_u]++;
            }
        }
    }
};

// Алгоритм Краскала с выбором типа DisjointSet
TVector<Edge> kruskal(int n, TVector<Edge>& edges, bool use_compression) {
    std::sort(edges.begin(), edges.end());  // Сортируем рёбра по весу
    TVector<Edge> mst;

    if (use_compression) {
        DisjointSetWithCompression ds(n);
        for (const Edge& edge : edges) {
            if (edge.u == edge.v && edge.weight == 0) {
                std::cout << "Игнорируем дефолтное ребро: " 
                          << edge.u << " - " << edge.v << " : " << edge.weight << std::endl;
                continue;
            }

            if (ds.find(edge.u) != ds.find(edge.v)) {
                ds.union_sets(edge.u, edge.v);
                mst.push_back(edge);
            }
        }
    } else {
        DisjointSetNoCompression ds(n);
        for (const Edge& edge : edges) {
            if (edge.u == edge.v && edge.weight == 0) {
                std::cout << "Игнорируем дефолтное ребро: " 
                          << edge.u << " - " << edge.v << " : " << edge.weight << std::endl;
                continue;
            }

            if (ds.find(edge.u) != ds.find(edge.v)) {
                ds.union_sets(edge.u, edge.v);
                mst.push_back(edge);
            }
        }
    }

    return mst;
}

#endif // INCLUDE_KRUSKAL_H_
