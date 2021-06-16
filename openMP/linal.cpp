#include "omp.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<vector<int>> generate_matrix(int n, int m){
	vector<vector<int>> matrix(n, vector<int>(m, 0));
	for (int i = 0; i<n; i++){
		for (int j = 0; j<m;j++){
			matrix[i][j] = rand() % 1000;
		}
	}
	return matrix;
}

vector<int> generate_vector(int n){
	vector<int> vec(n);
	for (int i = 0; i<n; i++){
		vec[i] = rand() % 1000;
	}
	return vec;
}

vector<int> seq_multiply_matrix_vector(const vector<vector<int>>& matrix, const vector<int>& vec){
	double t1 = omp_get_wtime();
	int n = matrix.size(), m = vec.size();
	vector<int> result(n, 0);
	for (int i = 0; i < n; i++){
		for (int j = 0; j<m; j++){
			result[i] += matrix[i][j]*vec[j];
		}
	}
	double t2 = omp_get_wtime();
	cout << "Время последовательного матрица X вектор: " << t2-t1 << endl;
	return result;
}

vector<int> rows_multiply_matrix_vector(const vector<vector<int>>& matrix, const vector<int>& vec){
	int n = matrix.size(), m = vec.size();
	vector<int> result(n, 0);
	double t1 = omp_get_wtime();
	#pragma omp parallel for
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			result[i] += matrix[i][j]*vec[j];
		}
	}
	double t2 = omp_get_wtime();
	cout << "Время ленточного матрица X вектор: " << t2-t1 << endl;
	return result;
}

vector<int> cols_multiply_matrix_vector(const vector<vector<int>>& matrix, const vector<int>& vec){
	int n = matrix.size(), m = vec.size();
	vector<int> result(n, 0);
	double t1 = omp_get_wtime();
	#pragma omp parallel for
	for (int j = 0; j < m; j++){
		for (int i = 0; i < n; i++){
			#pragma omp atomic
			result[i] += matrix[i][j]*vec[j];
		}
	}
	double t2 = omp_get_wtime();
	cout << "Время поколоночного матрица X вектор: " << t2-t1 << endl;
	return result;
}

vector<int> blocks_multiply_matrix_vector(const vector<vector<int>>& matrix, const vector<int>& vec){
	int n = matrix.size(), m = vec.size();
	vector<int> result(n, 0);
	double t1 = omp_get_wtime();
	#pragma omp parallel for
	for (int j = 0; j < m; j++){
		#pragma omp parallel for
		for (int i = 0; i < n; i++){
			#pragma omp atomic
			result[i] += matrix[i][j]*vec[j];
		}
	}
	double t2 = omp_get_wtime();
	cout << "Время блочного матрица X вектор: " << t2-t1 << endl;
	return result;
}

vector<vector<int>> seq_multiply_matrices(const vector<vector<int>>& matrix1, const vector<vector<int>>& matrix2){
	int n = matrix1.size(), m = matrix2.size(), k = matrix2[0].size();
	vector<vector<int>> result(n, vector<int>(k,0));
	double t1 = omp_get_wtime();
	for (int i = 0; i < n; i++){
		for (int p = 0; p<k; p++){
			for (int j=0; j<m; j++){
				result[i][p] += matrix1[i][j]*matrix2[j][p];
			}
		}
	}
	double t2 = omp_get_wtime();
	cout << "Время последовательного матрица X матрица: " << t2-t1 << endl;
	return result;
}

vector<vector<int>> stripes_multiply_matrices(const vector<vector<int>>& matrix1, const vector<vector<int>>& matrix2){
	int n = matrix1.size(), m = matrix2.size(), k = matrix2[0].size();
	vector<vector<int>> result(n, vector<int>(k,0));
	double t1 = omp_get_wtime();
	#pragma omp parallel for
	for (int i = 0; i < n; i++){
		for (int p = 0; p<k; p++){
			for (int j=0; j<m; j++){
				result[i][p] += matrix1[i][j]*matrix2[j][p];
			}
		}
	}
	double t2 = omp_get_wtime();
	cout << "Время ленточного матрица X матрица: " << t2-t1 << endl;
	return result;
}

vector<vector<int>> blocks_multiply_matrices(const vector<vector<int>>& matrix1, const vector<vector<int>>& matrix2){
	int n = matrix1.size(), m = matrix2.size(), k = matrix2[0].size();
	vector<vector<int>> result(n, vector<int>(k,0));
	double t1 = omp_get_wtime();
	#pragma omp parallel for
	for (int i = 0; i < n; i++){
		#pragma omp parallel for
		for (int p = 0; p<k; p++){
			#pragma omp parallel for
			for (int j=0; j<m; j++){
				result[i][p] += matrix1[i][j]*matrix2[j][p];
			}
		}
	}
	double t2 = omp_get_wtime();
	cout << "Время блочного матрица X матрица: " << t2-t1 << endl;
	return result;
}

void test1(int n, int m, vector<int> testing_threads){
	cout << "Размер матрицы: " << n << " x " << m << endl;
	vector<vector<int>> matrix1 = generate_matrix(n, m);
	vector<int> vec = generate_vector(m);
	auto ans1 = seq_multiply_matrix_vector(matrix1, vec);
	cout << endl;
	for (const auto& threads : testing_threads){
		omp_set_num_threads(threads);
		cout << threads << " потоков" << endl;
		auto ans2 = rows_multiply_matrix_vector(matrix1, vec);
		auto ans3 = cols_multiply_matrix_vector(matrix1, vec);
		auto ans4 = blocks_multiply_matrix_vector(matrix1, vec);
		cout << endl;
	}
	cout << "-----------------------------------------------"<<endl;
}


void test2(int n, int m, int k, vector<int> testing_threads){
	cout << "Размер матриц: " << n << " x " << m << " и " << m << " x "<<k<< endl;
	vector<vector<int>> matrix1 = generate_matrix(n, m);
	vector<vector<int>> matrix2 = generate_matrix(m, k);
	auto ans1 = seq_multiply_matrices(matrix1, matrix2);
	cout << endl;
	for (const auto& threads : testing_threads){
		omp_set_num_threads(threads);
		cout << threads << " потоков" << endl;
		auto ans2 = stripes_multiply_matrices(matrix1, matrix2);
		auto ans3 = blocks_multiply_matrices(matrix1, matrix2);
		cout << "-----------------------------------------------"<<endl;
	}
}

int main(){
	vector<int> testing_threads = {2, 4, 8};
	vector<int> testing_n = {1000, 5000, 10000};
	cout << "Тестирование перемножения матрицы на вектор" << endl;
	cout << "-----------------------------------------------"<<endl;
	for (const auto& n : testing_n){
		test1(n, n, testing_threads);
	}
	cout << endl;
	testing_n = vector<int>({100, 250, 500});
	cout << "Тестирование перемножения матрицы на матрицу" << endl;
	cout << "-----------------------------------------------"<<endl;
	for (const auto& n : testing_n){
		test2(n, n, n, testing_threads);
	}
}
