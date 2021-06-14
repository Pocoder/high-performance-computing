#include <iostream>
#include "omp.h"
#include <iomanip>

using namespace std;

double f(double x){
  return 4.0 / (1.0 + x*x);
}

int main(){
  double a = 0;
  double b = 1;
  int steps = 1000;
  double step = (b-a)/steps;
  double result = 0;
  for (int i = 0; i<steps;i++){
	double x = a + (i+0.5)*step;
	result += f(x) * step;
  }
  cout << "1 thread: " << setprecision(10) << result << endl;

  long double result2 = 0;
  #pragma omp parallel for shared(a,steps,step)
  for (int i = 0; i<steps;i++){
        long double x = a + (i+0.5)*step;
        result2 += f(x) * step;
  }
  cout << "multi-thread without reduction: " << setprecision(10) << result2 << endl;


  long double result3 = 0;
  #pragma omp parallel for shared(a,steps,step) reduction(+:result3)
  for (int i = 0; i<steps;i++){
        long double x = a + (i+0.5)*step;
        result3 += f(x) * step;
  }
  cout << "multi-thread with reduction: " << setprecision(10) << result3 << endl;

  return 0;
}
