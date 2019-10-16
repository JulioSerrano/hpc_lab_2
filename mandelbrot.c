#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Complex {
  double real;
  double imaginary;
} Complex;

// funcion que otorga memoria a una matriz en donde se guardará la imagen resultante
// entrada: las filas y columnas uqe representan las dimensiones de la matriz
// salida: Una matriz de flotantes
float **createImage(int rows, int columns) {
  float **image = (float **)malloc(sizeof(float *) * rows);
  for (int i = 0; i < rows; i++) {
    image[i] = (float *)malloc(sizeof(float) * columns);
  }
  return image;
}
// funcion que obtiene el absoluto de un numero complejo
// entrada: Una estructura de un numero complejo 
// salida: El decimal resultante de la operacion
double complexAbsolute(Complex c) {
  return sqrt((c.real* c.real) + (c.imaginary * c.imaginary));
}

// funcion utilizada para saber en que iteracion n el punto i,j se escapa de la matriz
// entrada: Una estructuta de un numero complejo i,j y el depth que representa hasta que numero se hara la iteración
// salida: un numero que será usado para formar los colores de la imagen
double mandelbrot(Complex c, int depth) {
  double realAux, imaginaryAux;
  int n = 1;
  Complex Zn = c;
  while (complexAbsolute(Zn) < 2 && n < depth) {
    realAux = (Zn.real * Zn.real) - (Zn.imaginary * Zn.imaginary);
    imaginaryAux = 2 * Zn.real * Zn.imaginary;
    Zn.real = realAux + c.real;
    Zn.imaginary = imaginaryAux + c.imaginary;
    n += 1;
  }
  return log(n) + 1;
}

// funcion utilizada para saber la distancia entre los puntos ingresados
// entrada: los limites inferiores u superiores de la matriz y el sampleo
// salida: el numero que indica la distancia entre los puntos considerando el punto 0,0
double getDistance(double upperLimit, double lowerLimit, double sampling) {
  return round((upperLimit - lowerLimit) / sampling) + 1;
}

// funcion que se encarga de recorrer segun el sampleo y las dimensiones ingresadas para luego evaluar el punto i,j
// entrada: se ingresan los limites de la matriz, el sampleo, el limite de iteraciones y las dimensiones de la matriz
// salida: se escribe por referencia en la imagen resultante
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

int main(int argc, char **argv) {
  int depth, c;
  double sampling;
  double upperLimitReal;
  double lowerLimitReal;
  double upperLimitImaginary;
  double lowerLimitImaginary;
  char *filename = (char *)malloc(sizeof(char) * 20);

  while ((c = getopt(argc, argv, "i:a:b:c:d:s:f:")) != -1)
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
                        sampling, depth, image);

  FILE *fp = fopen(filename, "w+");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      fwrite(&image[i][j], sizeof(float), 1, fp);
    }
  }
  for (int i = 0; i < rows; i++) {
      free(image[i]);
  }
  free(image);
  fclose(fp);
}
