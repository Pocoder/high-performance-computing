#include "omp.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

vector<int> generate_vector(int n){
	vector<int> vec(n);
	for (int i = 0; i<n;i++){
		vec[i] = rand() % 10000;
	}
	return vec;
}

double even_odd_sort(vector<int>& A){
	double t1 = omp_get_wtime();
	bool sorted = false;
	int init = 0;
	int n = A.size();
	while (!sorted || init !=0){
		sorted = true;
		for (int j = init; j < n - 1; j+=2){
			if (A[j] > A[j+1]){
				swap(A[j], A[j+1]);
				sorted = false;
			}
		}
		init = 1 - init;
	}
	double t2 = omp_get_wtime();
	return t2-t1;
}

double parallel_even_odd_sort(vector<int>& A){
	double t1 = omp_get_wtime();
	bool sorted = false;
	int init = 0;
	int n = A.size();
	while (!sorted || init !=0){
		sorted = true;
		#pragma omp parallel for shared(init, sorted, A, n)
		for (int j = init; j < n - 1; j+=2){
			if (A[j] > A[j+1]){
				swap(A[j], A[j+1]);
				sorted = false;
			}
		}
		init = 1 - init;
	}
	double t2 = omp_get_wtime();
	return t2-t1;
}

void print_vector(const vector<int>& vec){
	for (const auto& el : vec){
		cout << el << " ";
	}
	cout << endl;
}

void test(const vector<int>& sizes){
	omp_set_num_threads(4);
	for (const auto& size : sizes){
		double all_seq_time = 0;
		double all_parallel_time = 0;
		for (int i = 0; i<10;i++){
			auto v1 = generate_vector(size);
			auto v2 = v1;
			all_seq_time += even_odd_sort(v1);
			all_parallel_time += parallel_even_odd_sort(v2);
		}
		double mean_seq_time = all_seq_time / 10;
		double mean_parallel_time = all_parallel_time / 10;
		cout << "На массиве размера "<<size<< " в среднем за 10 раз время составило:"<<endl;
		cout << "Для последовательного алгоритма: " << mean_seq_time << endl;
		cout << "Для параллельного алгоритма на 4 потока "<<mean_parallel_time << endl;
		cout << "------------------------------------------------" << endl;
	}
}

int main(){
	vector<int> sizes = {500, 1000, 5000, 10000, 15000};
	test(sizes);
	return 0;
}
