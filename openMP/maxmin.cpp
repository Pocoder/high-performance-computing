#include "omp.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<int>> generate_matrix(int m, int n){
	vector<vector<int>> matrix(m, vector<int>(n, 0));
	for (int i = 0;i<m;i++){
		for (int j = 0; j<n; j++){
			matrix[i][j] = rand() % 1000;
		}
	}
	return matrix;
}

int seq_alg(const vector<vector<int>>& matrix){
	double t1 = omp_get_wtime();
	int n = matrix.size(), m = matrix[0].size();
	vector<int> mins(n);
	for (int i = 0; i < n; i++){
		mins[i] = matrix[i][0];
		for (int j = 1; j < m; j++){
			mins[i] = min(mins[i], matrix[i][j]);
		}
	}
	int maxmin = mins[0];
	for (int i = 1; i<n; i++){
		maxmin = max(maxmin, mins[i]);
	}
	double t2 = omp_get_wtime();
	cout << "Время работы последовательного алгоритма: " << t2-t1 << endl;
	return maxmin;
}

int par_alg(const vector<vector<int>>& matrix){
	double t1 = omp_get_wtime();
	int n = matrix.size(), m = matrix[0].size();
	vector<int> mins(n);
	omp_set_num_threads(16);
	#pragma omp parallel for
	for (int i = 0; i < n; i++){
		int min_i = matrix[i][0];
		#pragma omp parallel for reduction(min: min_i)
		for (int j = 1; j<m;j++){
			min_i = min(min_i, matrix[i][j]);
		}
		mins[i] = min_i;
	}

	int maxmin = mins[0];
	#pragma omp parallel for reduction(max: maxmin)
	for (int i = 1; i<n; i++){
		maxmin = max(maxmin, mins[i]);
	}
	double t2 = omp_get_wtime();
	cout << "Время работы параллельного алгоритма: " << t2-t1 << endl;
	return maxmin;
}


int main(){
	const int n = 50000, m=10000;
	vector<vector<int>> matrix = generate_matrix(n, m);
	int ans_seq = seq_alg(matrix);
	cout << "seq alg answer: " << ans_seq << endl;
	int ans_par = par_alg(matrix);
	cout << "par alg answer: " << ans_par << endl;
	return 0;
}
