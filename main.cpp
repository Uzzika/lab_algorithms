#include <iostream>
#include <fstream>
#include <chrono>
#include "kruskal.h"  // Включаем kruskal.h с функцией generateGraph
#include "vector.h"

// Функция для замера времени выполнения
void measureAndLog(std::ofstream& results, const std::string& label, int n, TVector<Edge>& edges) {
    std::cout << "Запуск теста: " << label << std::endl;

    auto startA = std::chrono::high_resolution_clock::now();
    kruskal(n, edges, false);  // Алгоритм А (без сжатия путей)
    auto endA = std::chrono::high_resolution_clock::now();

    auto startB = std::chrono::high_resolution_clock::now();
    kruskal(n, edges, true);  // Алгоритм B (со сжатием путей)
    auto endB = std::chrono::high_resolution_clock::now();

    results << label << ", A: "
            << std::chrono::duration<double>(endA - startA).count()
            << ", B: "
            << std::chrono::duration<double>(endB - startB).count()
            << "\n";

    // Добавляем flush для немедленной записи
    results.flush();
}

int main() {
    try {
        int q = 1, r = 100;  // Диапазон весов рёбер

        // === 3.1 ===
        std::ofstream results_3_1("C:\\Users\\79200\\source\\prog-algorythms24-25\\results_3_1.txt");
        for (int n = 10; n <= 200; n += 10) {
            int m1 = std::min((n * n) / 10, n * (n - 1) / 2);  // Ограничиваем m
            int m2 = std::min(n * n, n * (n - 1) / 2);
            TVector<Edge> edges1 = generateGraph(n, m1, q, r);
            TVector<Edge> edges2 = generateGraph(n, m2, q, r);
            measureAndLog(results_3_1, "3.1, n=" + std::to_string(n) + ", m=n^2/10", n, edges1);
            measureAndLog(results_3_1, "3.1, n=" + std::to_string(n) + ", m=n^2", n, edges2);
        }

        // === 3.2 ===
        std::ofstream results_3_2("C:\\Users\\79200\\source\\prog-algorythms24-25\\results_3_2.txt");
        for (int n = 10; n <= 200; n += 10) {
            int m1 = std::min(100 * n, n * (n - 1) / 2);
            int m2 = std::min(1000 * n, n * (n - 1) / 2);
            TVector<Edge> edges1 = generateGraph(n, m1, q, r);
            TVector<Edge> edges2 = generateGraph(n, m2, q, r);
            measureAndLog(results_3_2, "3.2, n=" + std::to_string(n) + ", m=100n", n, edges1);
            measureAndLog(results_3_2, "3.2, n=" + std::to_string(n) + ", m=1000n", n, edges2);
        }

        // === 3.3 === Выполняем фиксированное n, варьируем m
        std::ofstream results_3_3("C:\\Users\\79200\\source\\prog-algorythms24-25\\results_3_3.txt");
        if (!results_3_3) {
            std::cerr << "Ошибка открытия файла results_3_3.txt для записи!\n";
            return 1;
        }
        int n_fixed = 200;  // Фиксированное значение n
        for (int m = 0; m <= 5000; m += 500) {  // Варьируем m от 0 до 5000 с шагом 500
            TVector<Edge> edges = generateGraph(n_fixed, m, q, r);
            measureAndLog(results_3_3, "3.3, n=" + std::to_string(n_fixed) + ", m=" + std::to_string(m), n_fixed, edges);
        }
        results_3_3.close();
        std::cout << "=== Тест 3.3 завершён ===" << std::endl;

        // === 3.4 === Выполняем фиксированное n и m, варьируем r
        std::ofstream results_3_4("C:\\Users\\79200\\source\\prog-algorythms24-25\\results_3_4.txt");
        if (!results_3_4) {
            std::cerr << "Ошибка открытия файла results_3_4.txt для записи!\n";
            return 1;
        }
        for (int r_val = 10; r_val <= 200; r_val += 10) {  // Варьируем r от 10 до 200 с шагом 10
            int m = n_fixed * n_fixed / 10;  // m ≈ n^2 / 10
            TVector<Edge> edges = generateGraph(n_fixed, m, q, r_val);
            measureAndLog(results_3_4, "3.4, r=" + std::to_string(r_val), n_fixed, edges);
        }
        results_3_4.close();
        std::cout << "=== Тест 3.4 завершён ===" << std::endl;

        std::cout << "Все тесты завершены. Результаты записаны в отдельные файлы.\n";

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}