#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

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
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      image[i][j] = 0;
    }
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
  Complex Zn = c;
  float n = 1;
  while (complexAbsolute(Zn) < 2 && n < depth) {
    Zn = complexSum(complexsquare(Zn), c);
    n += 1;
  }
  return log(n) + 1;
}

double getDistance(double upperLimit, double lowerLimit, double sampling) {
  return round((upperLimit - lowerLimit) / sampling) + 1;
}

double distance(double x, double y) {
	return sqrt( square(x) + square(y) );
}

void createMandelbrotImage(double lowerLimitImaginary, double lowerLimitReal,
                           int rows, int columns, double sampling,
                           int depth, float **image) {
  
  double Cx, Cy, Zn_r, Zn_i, Zn_aux;
	int n, i, j;
  # pragma omp parallel shared(image, sampling, depth, rows, columns) private(Cx, Cy, Zn_r, Zn_i, Zn_aux, n, i, j) num_threads(10)
  {
    # pragma omp for
    for (i = 0; i < rows; i++) {
      Cy = lowerLimitImaginary + sampling * i;
      for (j = 0; j < columns; j++) {
        Cx = lowerLimitReal + sampling * j;
				n = 1;
				Zn_r = Cx;
				Zn_i = Cy;
				while( distance(Zn_r, Zn_i) < 2 && n < depth) {
					Zn_aux = square(Zn_r) + Cx - square(Zn_i);
					Zn_i = 2*Zn_i*Zn_r + Cy;
					Zn_r = Zn_aux;		
					n++;
				}
				image[j][i] = log(n) + 1;
      }
    }
  }
}



int main() {
  int depth = 500;
  double sampling = 0.00000000001;
  double upperLimitReal = -0.748766707771757;
  double lowerLimitReal = -0.748766713922161;
  double upperLimitImaginary = 0.123640851045266;
  double lowerLimitImaginary = 0.123640844894862;
  // int depth = 500;
  // double sampling = 0.001;
  // double upperLimitReal = 1;
  // double lowerLimitReal = -1;
  // double upperLimitImaginary = 1;
  // double lowerLimitImaginary = -1;

  double i, j, rows, columns;
  rows = getDistance(upperLimitReal, lowerLimitReal, sampling);
  columns = getDistance(upperLimitImaginary, lowerLimitImaginary, sampling);

  float **image = createImage(rows, columns);
  
  createMandelbrotImage(lowerLimitImaginary, lowerLimitReal, rows, columns,
                        sampling, depth, image);

  FILE *fp = fopen("image.raw", "w+");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      fwrite(&image[i][j], sizeof(float), 1, fp);
    }
  }
  fclose(fp);
}
