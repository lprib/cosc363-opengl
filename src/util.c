#include "util.h"

#include <GL/freeglut.h>
#include <IL/il.h>
#include <stdio.h>

// Standard normal calc from 3 points
void normal(double x1, double y1, double z1, double x2, double y2, double z2,
            double x3, double y3, double z3) {
  double nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
  double ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
  double nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
  glNormal3d(nx, ny, nz);
}

// Adapted from Dr. Makundan's bmp loading routing
// re-written in c rather than cpp
void load_bmp(char* filename) {
  char* imageData;
  char header1[18];
  char header2[24];
  short int planes, bpp;
  int width, height;
  int nbytes, size, index, temp;

  FILE* file;
  file = fopen(filename, "rb");
  if (file == NULL) {
    printf("error opening file %s\n", filename);
    exit(1);
  }

  fread(header1, sizeof(header1), 1, file);
  fread(&width, sizeof(int), 1, file);
  fread(&height, sizeof(int), 1, file);
  fread(&planes, sizeof(short int), 1, file);
  fread(&bpp, sizeof(short int), 1, file);
  fread(header2, sizeof(header2), 1, file);

  // printf("w %d h %d\n", width, height);

  nbytes = bpp / 8;           // No. of bytes per pixels
  size = width * height * nbytes;  // Total number of bytes to be read
  imageData = malloc(size * sizeof(char));
  fread(imageData, size, 1, file);
  // Swap r and b values
  for (int i = 0; i < width * height; i++) {
    index = i * nbytes;
    temp = imageData[index];
    imageData[index] = imageData[index + 2];
    imageData[index + 2] = temp;
  }
  glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
               imageData);
  free(imageData);
}