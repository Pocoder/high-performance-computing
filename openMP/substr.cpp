#include "omp.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int count_substr(const string& text, const string& templ){
	double t1 = omp_get_wtime();
	int count = 0;
	for (int i = 0; i< text.length()-templ.length()+1; i++){
		bool contain = true;
		for (int j = 0; j<templ.length(); j++){
			if (text[i+j] != templ[j]){
				contain = false;
				break;
			}
		}
		if (contain)
			count++;
	}
	double t2 = omp_get_wtime();
	cout << "Количество: "<<count<< ", время: " << t2-t1 << endl;
	return count;
}

int parallel_count_substr(const string& text, const string& templ){
	double t1 = omp_get_wtime();
	int count = 0;
	int threads = 8;
	omp_set_num_threads(threads);
	#pragma omp parallel for
	for (int i = 0; i< text.length()-templ.length()+1; i++){
		bool contain = true;
		for (int j = 0; j<templ.length(); j++){
			if (text[i+j] != templ[j]){
				contain = false;
				break;
			}
		}
		if (contain)
			count++;
	}
	double t2 = omp_get_wtime();
	cout << "Параллельная версия, потоков: " << threads<<endl;
	cout << "Количество: "<<count<< ", время: " << t2-t1 << endl;
	return count;
}

int main(){
	ifstream in("./ral.txt");
	string text = "";
	string str = "";
	string sub = "руслан";
	if (in.is_open()){
		while (getline(in, str)){
			text += str;
		}
	}
	int count1 = count_substr(text, sub);
	int count2 = parallel_count_substr(text, sub);
	cout << text.length() << endl;
	return 0;
}
