#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib> 
#include <ctime> 
#include <chrono>
#include <cmath> 
#include <sstream>
#include <omp.h> // Include OpenMP 

std::vector<std::vector<int>> create_random_of_matrix(const int size) {
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size));
#pragma omp parallel for
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            matrix[i][j] = rand() % 100;
    return matrix;
}

std::vector<std::vector<int>> multiply_matrix(const std::vector<std::vector<int>>& matrix1, const std::vector<std::vector<int>>& matrix2, const int size) {
    std::vector<std::vector<int>> result(size, std::vector<int>(size, 0));
#pragma omp parallel for collapse(2)
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            for (int k = 0; k < size; ++k)
                result[i][j] += matrix1[i][k] * matrix2[k][j];
    return result;
}

void write_matrix_to_file(const std::vector<std::vector<int>>& matrix, const std::string& filename) {
    std::ofstream output_file(filename);
    if (!output_file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl; return;
    }
    for (const auto& row : matrix) {
        for (int value : row)
            output_file << value << " ";
        output_file << std::endl;
    }
    output_file.close();
}

double calculate_mean(const std::vector<double>& values) {
    double sum = 0.0;
    for (double value : values) {
        sum += value;
    }
    return sum / values.size();
}

double calculate_standard_deviation(const std::vector<double>& values, double mean) {
    double variance = 0.0;
    for (double value : values) {
        variance += pow(value - mean, 2);
    }
    variance /= values.size();
    return sqrt(variance);
}

int main() {
    srand(time(nullptr));
    std::vector<int> sizes = { 50, 100, 200, 250, 500, 750, 1000 };

    std::ofstream file("avg_times.txt");
    for (int size : sizes)
    {
        std::vector<double> timings;
        for (int i = 0; i < 7; ++i)
        {
            std::vector<std::vector<int>> matrix1 = create_random_of_matrix(size), matrix2 = create_random_of_matrix(size);
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::vector<int>> multiply_result = multiply_matrix(matrix1, matrix2, size);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            timings.push_back(duration.count());
            if (i == 0) {
                std::stringstream filename_stream_m1, filename_stream_m2, filename_stream_result;
                filename_stream_m1 << "matrix1_" << size << ".txt";
                std::string filename_matrix1 = filename_stream_m1.str();
                filename_stream_m2 << "matrix2_" << size << ".txt";
                std::string filename_matrix2 = filename_stream_m2.str();
                filename_stream_result << "multiply_result_" << size << ".txt";
                std::string filename_result = filename_stream_result.str();
                write_matrix_to_file(multiply_result, filename_result);
                write_matrix_to_file(matrix1, filename_matrix1);
                write_matrix_to_file(matrix2, filename_matrix2);
            }
        }
        double mean = calculate_mean(timings);
        double std_deviation = calculate_standard_deviation(timings, mean);
        double margin_of_error = 1.96 * (std_deviation / sqrt(timings.size())); 
        file << size << " - Mean: " << mean << " ms, Std Deviation: " << std_deviation << " ms, 95% Confidence Interval: (" << (mean - margin_of_error) << ", " << (mean + margin_of_error) << ") ms" << std::endl;
    }
    file.close();
}
