#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;
using namespace chrono;

double estimate_pi_monte_carlo(long long N, int block_size = 10000000) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_real_distribution<double> dis(-1.0, 1.0);
    
    long long total_inside = 0;
    long long processed = 0;
    
    while (processed < N) {
        long long current_block = min(block_size, N - processed);
        long long block_inside = 0;
        
        for (long long i = 0; i < current_block; i++) {
            double x = dis(gen);
            double y = dis(gen);
            if (x*x + y*y <= 1.0) {
                block_inside++;
            }
        }
        
        total_inside += block_inside;
        processed += current_block;
        
        // Прогрес для великих N
        if (N >= 1e9 && processed % (N/10) == 0) {
            cout << "  Прогрес: " << (processed * 100 / N) << "%" << endl;
        }
    }
    
    return 4.0 * total_inside / N;
}

int main() {
    vector<long long> N_values = {
        1'000'000,
        10'000'000,
        100'000'000,
        1'000'000'000,
        10'000'000'000,
        100'000'000'000
    };
    
    const double true_pi = M_PI;
    
    // Відкриваємо файл для результатів
    ofstream results_file("results/pi_monte_carlo_results.csv");
    results_file << "N,pi_estimated,accuracy,execution_time_sec,time_per_point_sec\n";
    
    for (long long N : N_values) {
        cout << "\nОбчислення для N = " << N << "..." << endl;
        
        auto start = high_resolution_clock::now();
        
        double pi_est = estimate_pi_monte_carlo(N);
        
        auto end = high_resolution_clock::now();
        double execution_time = duration<double>(end - start).count();
        
        double accuracy = fabs(pi_est - true_pi);
        double time_per_point = execution_time / N;
        
        cout << fixed << setprecision(10);
        cout << "  Оцінка π = " << pi_est << endl;
        cout << "  Точність = " << accuracy << endl;
        cout << "  Час = " << execution_time << " сек" << endl;
        cout << "  Час/точку = " << scientific << time_per_point << " сек" << endl;
        
        results_file << N << ","
                     << fixed << setprecision(10) << pi_est << ","
                     << scientific << accuracy << ","
                     << fixed << execution_time << ","
                     << scientific << time_per_point << "\n";
    }
    
    results_file.close();
    cout << "\nРезультати збережено у results/pi_monte_carlo_results.csv" << endl;
    
    return 0;
}