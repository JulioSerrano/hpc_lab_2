#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Complex {
  double real;
  double imaginary;
} Complex;

double square(double n) { return n * n; }

float **createImage(int rows, int columns) {
  float **image = (float **)malloc(sizeof(float *) * rows);
  for (int i = 0; i < rows; i++) {
    image[i] = (float *)malloc(sizeof(float) * columns);
  }
  return image;
}

double complexAbsolute(Complex c) {
  return sqrt((square(c.real) + square(c.imaginary)));
}

Complex complexSum(Complex c1, Complex c2) {
  Complex c;
  c.imaginary = c1.imaginary + c2.imaginary;
  c.real = c1.real + c2.real;
  return c;
}

Complex complexsquare(Complex complex) {
  Complex c;
  c.real = square(complex.real) + square(complex.imaginary) * -1;
  c.imaginary = 2 * complex.real * complex.imaginary;

  return c;
}

double mandelbrot(Complex c, int depth) {
  int iterador;
  double aux_real, aux_imag;
  Complex z_n;

  z_n.real = 0;
  z_n.imaginary = 0;
  iterador = 1;

  z_n.real = z_n.real + c.real;
  z_n.imaginary = z_n.imaginary + c.imaginary;
  while(complexAbsolute(z_n) < 2 && iterador < depth){
    aux_real = (z_n.real * z_n.real)  - (z_n.imaginary * z_n.imaginary);
    aux_imag = 2 * z_n.real * z_n.imaginary;
    z_n.real = aux_real + c.real;
    z_n.imaginary = aux_imag + c.imaginary;
    iterador +=1;
  }
  return log(iterador) + 1;
}

double getDistance(double upperLimit, double lowerLimit, double sampling) {
  return round((upperLimit - lowerLimit) / sampling) + 1;
}

void createMandelbrotImage(double lowerLimitImaginary, double lowerLimitReal,
                           double rows, double columns, double sampling,
                           int depth, float **image) {
  double x, y;
  for (int i = 0; i < rows; i++) {
    y = lowerLimitImaginary + sampling * i;
    for (int j = 0; j < columns; j++) {
      Complex c;
      c.real = lowerLimitReal + sampling * j;
      c.imaginary = y;
      image[i][j] = mandelbrot(c, depth);
    }
  }
}


int main() {
  // int depth = 500;
  // double sampling = 0.00000000001;
  // double upperLimitReal = -0.748766707771757;
  // double lowerLimitReal = -0.748766713922161;
  // double upperLimitImaginary = 0.123640851045266;
  // double lowerLimitImaginary = 0.123640844894862;
  int depth = 500;
  double sampling = 0.001;
  double upperLimitReal = 1;
  double lowerLimitReal = -1;
  double upperLimitImaginary = 1;
  double lowerLimitImaginary = -1;

  double i, j, rows, columns;
  rows = getDistance(upperLimitReal, lowerLimitReal, sampling);
  columns = getDistance(upperLimitImaginary, lowerLimitImaginary, sampling);

  float **image = createImage(rows, columns);
  createMandelbrotImage(lowerLimitImaginary, lowerLimitReal, rows, columns,
                        sampling, depth, image);


  /*FILE *fp = fopen("image.raw", "w+");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      fwrite(&image[i][j], sizeof(float), 1, fp);
    }
  }*/
}
