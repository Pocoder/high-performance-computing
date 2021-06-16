#include "omp.h"
#include <iostream>
#include <vector>

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
	int n = matrix.size(), m = vec.size();
	vector<int> result(n, 0);
	for (int i = 0; i < n; i++){
		for (int j = 0; j<m; j++){
			result[i] += matrix[i][j]*vec[j];
		}
	}
	return result;
}

vector<vector<int>> seq_multiply_matrices(const vector<vector<int>>& matrix1, const vector<vector<int>>& matrix2){
	int n = matrix1.size(), m = matrix2.size(), k = matrix2[0].size();
	vector<vector<int>> result(n, vector<int>(k,0));
	for (int i = 0; i < n; i++){
		for (int p = 0; p<k; p++){
			for (int j=0; j<m; j++){
				result[i][p] += matrix1[i][j]*matrix2[j][p];
			}
		}
	}
	return result;
}

int main(){
	int n = 1000, m = 1000, k = 1000;
	vector<vector<int>> matrix1 = generate_matrix(n, m);
	vector<vector<int>> matrix2 = generate_matrix(m, m);
	vector<int> vec = generate_vector(m);
}
