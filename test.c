#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

typedef struct Complex {
  float real;
  float imaginary;
} Complex;

Complex complexInit(float real, float imaginary) {
  Complex c;
  c.imaginary = imaginary;
  c.real = real;
  return c;
}

float complexAbsolute(Complex c) {
  return pow(c.real, 2) + pow(c.imaginary, 2);
}

Complex complexSum(Complex c1, Complex c2) {
  Complex c;
  c.imaginary = c1.imaginary + c2.imaginary;
  c.real = c1.real + c2.real;
  return c;
}

Complex complexPow(Complex complex) {
  Complex c;
  c.real = pow(complex.real, 2) + pow(complex.imaginary, 2) * -1;
  c.imaginary = 2 * complex.real * complex.imaginary;

  return c;
}

float mandelbrot(Complex c, int depth) {
  Complex Zn = c;
  float n = 1;
  while (complexAbsolute(Zn) < 2 && n < depth) {
    Zn = complexSum(complexPow(Zn), c);
    n += 1;
  }
  return log(n) + 1;
}

int main() {
  int depth = 500;
  float sampling = 0.5;
  float upperLimitReal = 1;
  float lowerLimitReal = -1;
  float upperLimitImaginary = 1;
  float lowerLimitImaginary = -1;

  float i, j;

  float rows = ((upperLimitReal - lowerLimitReal) / sampling) + 1;
  float columns = ((upperLimitImaginary - lowerLimitImaginary) / sampling) + 1;
  float n = rows*columns;

  float *image = (float *)malloc(sizeof(float) * n);
  // n = 0;


  float index, numth, mytid;
  float N;
  #pragma omp parallel private(mytid, numth, N, index) num_threads(2)
  {

  mytid = omp_get_thread_num();
  numth = omp_get_num_threads();

  printf("%d ",(int)mytid);
  printf("%d\n",(int)numth);
  N = ((upperLimitReal - lowerLimitReal)/numth);
  index = lowerLimitReal+(mytid * N);
  printf("N: %f\n", N);
  printf("index: %f\n",index);


  for (i = index; i <= N+index; i += sampling) {
    for (j = lowerLimitImaginary; j <= upperLimitImaginary; j += sampling) {
      Complex c = complexInit(i, j);
      mandelbrot(c, depth);
      printf("%f %f\n", i,j);
      // n += 1;
      // printf("%d\n", n);
    }
  }

  }




  // int imagen[100000];

  // int i, n, numth, mytid;

  // #pragma omp parallel private(mytid, numth, n, i) num_threads(20)
  // {
  //   int j;
  //   mytid = omp_get_thread_num();
  //   numth = omp_get_num_threads();

  //   printf("%d ",mytid);
  //   printf("%d ",numth);
  //   n = 100000/numth;
  //   i = mytid * n;

  //   #pragma omp for
  //   for(j=i; j<n+i; j++)
  //     printf("%d-",j);
  //     imagen[j] = 2*imagen[j];
  // }

  // for(int i=0; i<100000; i++)
  //   imagen[i] = 2*imagen[i];

  // for(int i=0; i<100000; i++){
  //   printf("%d ",imagen[i]);
  // }
}