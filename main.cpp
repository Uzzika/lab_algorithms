#include <iostream>
#include <fstream>
#include "kruskal.h"
#include "vector.h"

int main() {
    std::ifstream input("C:\\Users\\79200\\source\\prog-algorythms24-25\\graph.txt");
    if (!input) {
        std::cerr << "Ошибка: файл graph.txt не найден!" << std::endl;
        return 1;
    }

    int n, m;
    input >> n >> m;

    if (n <= 0 || m <= 0) {
        std::cerr << "Ошибка: количество вершин или рёбер некорректно!" << std::endl;
        return 1;
    }

    TVector<Edge> edges;
    for (int i = 0; i < m; ++i) {
        int u, v, weight;
        input >> u >> v >> weight;

        if (input.fail()) {
            std::cerr << "Ошибка: некорректный формат данных в файле!" << std::endl;
            return 1;
        }

        if (!(u == 0 && v == 0 && weight == 0)) {
            edges.push_back(Edge(u, v, weight));
        } else {
            std::cout << "Игнорируем дефолтное ребро на этапе чтения." << std::endl;
        }
    }
    input.close();

    // Фильтрация перед запуском алгоритма
    TVector<Edge> filtered_edges;
    for (size_t i = 0; i < edges.size(); ++i) {
        if (!(edges[i].u == 0 && edges[i].v == 0 && edges[i].weight == 0)) {
            filtered_edges.push_back(edges[i]);
        }
    }
    edges = filtered_edges;

    // Запускаем алгоритм Краскала без сжатия путей
    std::cout << "MST без сжатия путей:" << std::endl;
    TVector<Edge> mst_no_compression = kruskal(n, edges, false);
    int total_weight_no_compression = 0;
    for (const Edge& edge : mst_no_compression) {
        std::cout << edge.u << " - " << edge.v << " : " << edge.weight << std::endl;
        total_weight_no_compression += edge.weight;
    }
    std::cout << "Общий вес: " << total_weight_no_compression << std::endl;

    // Запускаем алгоритм Краскала со сжатием путей
    std::cout << "\nMST со сжатием путей:" << std::endl;
    TVector<Edge> mst_with_compression = kruskal(n, edges, true);
    int total_weight_with_compression = 0;
    for (const Edge& edge : mst_with_compression) {
        std::cout << edge.u << " - " << edge.v << " : " << edge.weight << std::endl;
        total_weight_with_compression += edge.weight;
    }
    std::cout << "Общий вес: " << total_weight_with_compression << std::endl;

    return 0;
}
