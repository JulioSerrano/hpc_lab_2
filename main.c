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

Complex **createComplexMatrix(float upperLimitReal, float lowerLimitReal,
                              float upperLimitImaginary,
                              float lowerLimitImaginary, double sampling) {

  float rows = ((upperLimitReal - lowerLimitReal) / sampling) + 1;
  float columns = ((upperLimitImaginary - lowerLimitImaginary) / sampling) + 1;

  Complex **matrix = (Complex **)malloc(sizeof(Complex *) * rows);
  for (int i = 0; i < rows; i++) {
    matrix[i] = (Complex *)malloc(sizeof(Complex) * columns);
  }
  float i, j;
  int x = 0, y = 0;
  for (i = lowerLimitReal; i <= upperLimitReal; i += sampling) {
    y = 0;
    for (j = lowerLimitImaginary; j <= upperLimitImaginary; j += sampling) {
      matrix[x][y].real = i;
      matrix[x][y].imaginary = j;
      y++;
    }
    x++;
  }
  return matrix;
}

int main() {
  int depth = 500;
  double sampling = 0.001;
  float upperLimitReal = 1;
  float lowerLimitReal = -1;
  float upperLimitImaginary = 1;
  float lowerLimitImaginary = -1;

  float i, j;

  float rows = ((upperLimitReal - lowerLimitReal) / sampling) + 1;
  float columns = ((upperLimitImaginary - lowerLimitImaginary) / sampling) + 1;

  // float *image = (float *)malloc(sizeof(float) * n);
  // n = 0;
  // for (i = lowerLimitReal; i <= upperLimitReal; i += sampling) {
  //   for (j = lowerLimitImaginary; j <= upperLimitImaginary; j += sampling) {
  //     Complex c = complexInit(i, j);
  //     image[n] = mandelbrot(c, depth);
  //     n += 1;
  //     printf("%d\n", n);
  //   }
  // }

  Complex **matrix =
      createComplexMatrix(upperLimitReal, lowerLimitImaginary,
                          upperLimitImaginary, lowerLimitImaginary, sampling);

  float **image = createImage(rows, columns);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      printf("%d %d\n", i, j);
      image[i][j] = mandelbrot(matrix[i][j], depth);
    }
  }

  FILE *fp = fopen("image.raw", "w+");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      fwrite(&image[i][j], sizeof(float), 1, fp);
    }
  }
}
