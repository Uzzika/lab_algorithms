#include <iostream>
#include <fstream>
#include "vector.h"  // Подключаем ваш TVector

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::swap;

// Структура для хранения ребра
struct Edge {
    int u;
    int v;
    int weight;

    bool operator!=(const Edge& other) const {
        return u != other.u || v != other.v || weight != other.weight;
    }
};

// Структура для Union-Find без сжатия путей
struct UnionFindSimple {
    TVector<int> parent_;
    TVector<int> rank_;

    UnionFindSimple(int n) : parent_(n), rank_(n, 0) {
        for (int i = 0; i < n; ++i) {
            parent_[i] = i;
        }
    }
};

// Функция Find для UnionFindSimple
int FindSimple(const UnionFindSimple& uf, int x) {
    while (x != uf.parent_[x]) {
        x = uf.parent_[x];
    }
    return x;
}

// Функция Unite для UnionFindSimple
void UniteSimple(UnionFindSimple& uf, int x, int y) {
    const int root_x = FindSimple(uf, x);
    const int root_y = FindSimple(uf, y);
    if (root_x != root_y) {
        if (uf.rank_[root_x] > uf.rank_[root_y]) {
            uf.parent_[root_y] = root_x;
        } else if (uf.rank_[root_x] < uf.rank_[root_y]) {
            uf.parent_[root_x] = root_y;
        } else {
            uf.parent_[root_y] = root_x;
            uf.rank_[root_x]++;
        }
    }
}

// Структура для Union-Find с сжатием путей
struct UnionFindCompressed {
    TVector<int> parent_;
    TVector<int> rank_;

    UnionFindCompressed(int n) : parent_(n), rank_(n, 0) {
        for (int i = 0; i < n; ++i) {
            parent_[i] = i;
        }
    }
};

// Функция Find для UnionFindCompressed с сжатием путей
int FindCompressed(UnionFindCompressed& uf, int x) {
    if (x != uf.parent_[x]) {
        uf.parent_[x] = FindCompressed(uf, uf.parent_[x]);  // Сжатие путей
    }
    return uf.parent_[x];
}

// Функция Unite для UnionFindCompressed
void UniteCompressed(UnionFindCompressed& uf, int x, int y) {
    const int root_x = FindCompressed(uf, x);
    const int root_y = FindCompressed(uf, y);
    if (root_x != root_y) {
        if (uf.rank_[root_x] > uf.rank_[root_y]) {
            uf.parent_[root_y] = root_x;
        } else if (uf.rank_[root_x] < uf.rank_[root_y]) {
            uf.parent_[root_x] = root_y;
        } else {
            uf.parent_[root_y] = root_x;
            uf.rank_[root_x]++;
        }
    }
}

// Функция для сортировки рёбер по весу (пузырьковая сортировка)
void BubbleSort(TVector<Edge>& edges) {
    const size_t n = edges.size();
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (edges[j].weight > edges[j + 1].weight) {
                swap(edges[j], edges[j + 1]);
            }
        }
    }
}

// Алгоритм Краскала
TVector<Edge> Kruskal(int n, TVector<Edge>& edges, bool use_compression) {
    BubbleSort(edges);
    TVector<Edge> mst;
    int mst_weight = 0;

    if (use_compression) {
        UnionFindCompressed uf(n);
        for (const auto& edge : edges) {
            if (FindCompressed(uf, edge.u) != FindCompressed(uf, edge.v)) {
                UniteCompressed(uf, edge.u, edge.v);
                mst.push_back(edge);
                mst_weight += edge.weight;
            }
        }
    } else {
        UnionFindSimple uf(n);
        for (const auto& edge : edges) {
            if (FindSimple(uf, edge.u) != FindSimple(uf, edge.v)) {
                UniteSimple(uf, edge.u, edge.v);
                mst.push_back(edge);
                mst_weight += edge.weight;
            }
        }
    }

    cout << "MST weight: " << mst_weight << endl;
    return mst;
}

// Чтение графа из файла
TVector<Edge> ReadGraph(const std::string& filename, int& n) {
    ifstream input(filename);
    int m;
    input >> n >> m;
    TVector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        input >> edges[i].u >> edges[i].v >> edges[i].weight;
    }
    input.close();
    return edges;
}

// Запись результата в файл
void WriteResult(const std::string& filename, const TVector<Edge>& mst) {
    ofstream output(filename);
    for (const auto& edge : mst) {
        output << edge.u << " " << edge.v << " " << edge.weight << endl;
    }
    output.close();
}

// Основная функция
int main() {
    const std::string input_file = "graph.txt";
    const std::string output_file1 = "mst_no_compression.txt";
    const std::string output_file2 = "mst_with_compression.txt";

    int n;
    TVector<Edge> edges = ReadGraph(input_file, n);

    const TVector<Edge> mst1 = Kruskal(n, edges, false);
    WriteResult(output_file1, mst1);

    const TVector<Edge> mst2 = Kruskal(n, edges, true);
    WriteResult(output_file2, mst2);

    return 0;
}