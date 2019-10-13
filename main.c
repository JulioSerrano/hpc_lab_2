#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
  float sampling = 0.001;
  float upperLimitReal = 1;
  float lowerLimitReal = -1;
  float upperLimitImaginary = 1;
  float lowerLimitImaginary = -1;

  float i, j;
  int n = (((upperLimitReal - lowerLimitReal) / sampling) + 1) *
          (((upperLimitImaginary - lowerLimitImaginary) / sampling) + 1);

  float *image = (float *)malloc(sizeof(float) * n);
  n = 0;
  for (i = lowerLimitReal; i <= upperLimitReal; i += sampling) {
    for (j = lowerLimitImaginary; j <= upperLimitImaginary; j += sampling) {
      Complex c = complexInit(i, j);
      image[n] = mandelbrot(c, depth);
      n += 1;
      printf("%d\n", n);
    }
  }

  FILE *fp = fopen("image.raw", "w+");
  for (int i = 0; i < n; i++) {
    fwrite(&image[i], sizeof(float), 1, fp);
  }
}
