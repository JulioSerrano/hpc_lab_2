#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h> 
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
                           int depth, float **image, int threads) {
  
  double x, y;
	int n, i, j;
  Complex c,Zn;
  # pragma omp parallel shared(image, sampling, depth, rows, columns) private(c, x, y, Zn, n, i, j) num_threads(threads)
  {
    # pragma omp for
    for (i = 0; i < columns; i++) {
      y = lowerLimitImaginary + sampling * i;
      for (j = 0; j < rows; j++) {
        x = lowerLimitReal + sampling * j;
				n = 1;
				c.real = x;
				c.imaginary = y;
        Zn = c;
				while( complexAbsolute(Zn) < 2 && n < depth) {
					Zn = complexSum(complexsquare(Zn), c);
					n++;
				}
				image[i][j] = log(n) + 1;
      }
    }
  }
}

int main(int argc, char **argv) {
  int depth, c, threads;
  double sampling;
  double upperLimitReal;
  double lowerLimitReal;
  double upperLimitImaginary;
  double lowerLimitImaginary;
  char* filename = (char *)malloc(sizeof(char) * 20);

  while ((c = getopt(argc, argv, "i:a:b:c:d:s:f:t:")) != -1)
    switch (c) {
    case 'i':
      depth = atoi(optarg);
      break;
    case 'a':
      lowerLimitReal = atof(optarg);
      break;
    case 'b':
      lowerLimitImaginary = atof(optarg);
      break;
    case 'c':
      upperLimitReal = atof(optarg);
      break;
    case 'd':
      upperLimitImaginary = atof(optarg);
      break;
    case 's':
      sampling = atof(optarg);
      break;
    case 'f':
      filename = optarg;
      break;
    case 't':
      threads = atoi(optarg);
      break;
    case '?':
      if (optopt == 'c')
        fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Opcion desconocida `-%c'.\n", optopt);
      else
        fprintf(stderr, "Opcion de caracter desconocido `\\x%x'.\n", optopt);
      return 1;
    default:
      abort();
    }

  double i, j, rows, columns;
  rows = getDistance(upperLimitReal, lowerLimitReal, sampling);
  columns = getDistance(upperLimitImaginary, lowerLimitImaginary, sampling);

  float **image = createImage(rows, columns);

  createMandelbrotImage(lowerLimitImaginary, lowerLimitReal, rows, columns,
                        sampling, depth, image, threads);

  FILE *fp = fopen(filename, "w+");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      fwrite(&image[i][j], sizeof(float), 1, fp);
    }
  }

  // for (int i = 0; i < rows; i++) {
  //     free(image[i]);
  // }
  // free(image);
  // fclose(fp);
}

// time ./test -i 500 -a -1 -b -1 -c 1 -d 1 -s 0.001 -f salida.raw -t 24
// time ./test -i 500 -a -0.748766713922161 -b 0.123640844894862 -c -0.748766707771757 -d 0.123640851045266 -s 1e-11 -f salida.raw -t 12
