#include <iostream>
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>
#include "vector.h" // Если это ваша собственная обёртка для TVector, оставьте это

using std::cout;
using std::endl;
using std::ofstream;
using std::string; 

// Структура ребра
struct Edge {
    int u;
    int v;
    int weight;
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

    int Find(int x) {
        while (x != parent_[x]) {
            x = parent_[x];
        }
        return x;
    }

    void Unite(int x, int y) {
        const int root_x = Find(x);
        const int root_y = Find(y);
        if (root_x != root_y) {
            if (rank_[root_x] > rank_[root_y]) {
                parent_[root_y] = root_x;
            } else if (rank_[root_x] < rank_[root_y]) {
                parent_[root_x] = root_y;
            } else {
                parent_[root_y] = root_x;
                rank_[root_x]++;
            }
        }
    }
};

// Структура для Union-Find с сжатием путей
struct UnionFindCompressed {
    TVector<int> parent_;
    TVector<int> rank_;

    UnionFindCompressed(int n) : parent_(n), rank_(n, 0) {
        for (int i = 0; i < n; ++i) {
            parent_[i] = i;
        }
    }

    int Find(int x) {
        if (x != parent_[x]) {
            parent_[x] = Find(parent_[x]);  // Сжатие путей
        }
        return parent_[x];
    }

    void Unite(int x, int y) {
        const int root_x = Find(x);
        const int root_y = Find(y);
        if (root_x != root_y) {
            if (rank_[root_x] > rank_[root_y]) {
                parent_[root_y] = root_x;
            } else if (rank_[root_x] < rank_[root_y]) {
                parent_[root_x] = root_y;
            } else {
                parent_[root_y] = root_x;
                rank_[root_x]++;
            }
        }
    }
};

// Генерация случайного графа
TVector<Edge> GenerateRandomGraph(int n, int m, int weight_limit) {
    TVector<Edge> edges;
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist_node(0, n - 1);
    std::uniform_int_distribution<int> dist_weight(1, weight_limit);

    for (int i = 0; i < m; ++i) {
        int u = dist_node(rng);
        int v = dist_node(rng);
        while (u == v) {
            v = dist_node(rng);  // Исключаем петли
        }
        int weight = dist_weight(rng);
        edges.push_back({u, v, weight});
    }

    return edges;
}

// Замер времени выполнения алгоритма Краскала
template <typename UnionFindType>
double MeasureKruskal(int n, const TVector<Edge>& edges) {
    auto start = std::chrono::high_resolution_clock::now();
    UnionFindType uf(n);

    TVector<Edge> mst;
    int mst_weight = 0;

    // Простая сортировка рёбер (по весу)
    TVector<Edge> sorted_edges = edges;
    std::sort(sorted_edges.begin(), sorted_edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    for (const auto& edge : sorted_edges) {
        if (uf.Find(edge.u) != uf.Find(edge.v)) {
            uf.Unite(edge.u, edge.v);
            mst.push_back(edge);
            mst_weight += edge.weight;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count();
}

// Основная функция экспериментов
void RunExperiments(const string& output_file) {
    const int trials = 10;  // Количество повторений для каждого графа
    const int max_nodes = 1000;  // Максимальное число вершин
    const int max_edges = 5000;  // Максимальное число рёбер
    const int weight_limit = 100;  // Максимальный вес рёбер

    ofstream results(output_file);

    results << "Nodes,Edges,Time_No_Compression,Time_With_Compression\n";

    for (int nodes = 100; nodes <= max_nodes; nodes += 100) {
        for (int edges = nodes - 1; edges <= max_edges; edges += 1000) {
            double total_time_no_compression = 0;
            double total_time_with_compression = 0;

            for (int trial = 0; trial < trials; ++trial) {
                TVector<Edge> graph = GenerateRandomGraph(nodes, edges, weight_limit);

                total_time_no_compression += MeasureKruskal<UnionFindSimple>(nodes, graph);
                total_time_with_compression += MeasureKruskal<UnionFindCompressed>(nodes, graph);
            }

            results << nodes << "," << edges << "," 
                    << (total_time_no_compression / trials) << "," 
                    << (total_time_with_compression / trials) << endl;
        }
    }

    results.close();
}

int main() {
    const string output_file = "experiment_results.csv";
    RunExperiments(output_file);

    cout << "Эксперименты завершены. Результаты сохранены в " << output_file << endl;
    return 0;
}
