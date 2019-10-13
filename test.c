#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int main() {
  int ntasks = 3;
#pragma omp parallel num_threads(ntasks)
{ 
  printf("Hello World!, numero de hebra = %d\n", omp_get_thread_num()); 
}
}