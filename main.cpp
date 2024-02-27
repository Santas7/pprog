#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib> 
#include <ctime> 

std::vector<std::vector<int>> create_random_of_matrix(const int size) {
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size));
    for (int i = 0; i < size; ++i) 
        for (int j = 0; j < size; ++j) 
            matrix[i][j] = rand() % 100;
    return matrix;
}

std::vector<std::vector<int>> multiply_matrix(const std::vector<std::vector<int>>& matrix1, const std::vector<std::vector<int>>& matrix2, const int size) {
    std::vector<std::vector<int>> result(size, std::vector<int>(size, 0));
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

int main() {
    srand(time(nullptr));
    std::vector<int> sizes = { 50, 100, 200, 250, 500, 750, 1000 };
    std::ofstream file("avg_times.txt");
    for (int size : sizes) 
    {
        auto total_d = std::chrono::milliseconds(0);
        for (int i = 0; i < 7; ++i) 
        {
            std::vector<std::vector<int>> matrix1 = create_random_of_matrix(size), matrix2 = create_random_of_matrix(size);
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::vector<int>> multiply_result = multiply_matrix(matrix1, matrix2, size);
            auto end = std::chrono::high_resolution_clock::now(), duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            total_d += duration;
            if (i == 0) 
            {
                std::stringstream filename_stream_m1, filename_stream_m2, filename_stream_result;
                filename_stream_m1 << "matrix_1_" << size << ".txt";
                std::string filename_matrix1 = filename_stream_m1.str();
                filename_stream_m2 << "matrix_2_" << size << ".txt";
                std::string filename_matrix2 = filename_stream_m2.str();
                filename_stream_result << "multiply_result_" << size << ".txt";
                std::string filename_result = filename_stream_result.str();
                write_matrix_to_file(multiply_result, filename_result);
                write_matrix_to_file(matrix1, filename_matrix1);
                write_matrix_to_file(matrix2, filename_matrix2);
            }
        }
        file << size << " - " << (total_d / 7).count() << std::endl;
    }
    file.close();
}
