#include "julia.h"

#include <GL/freeglut.h>
#include <math.h>

#define MAX_ITERATIONS 5
#define BUF_WIDTH 500
#define BUF_HEIGHT 500

#define SURFACE_DEPTH 1.0
#define HEIGHT_SCALE 2

static double cx, cy;
static double r = 2;

static double counter = 0.0;

static double buffer[BUF_WIDTH * BUF_HEIGHT] = {0};

static double map(double input, double input_start, double input_end,
                  double output_start, double output_end) {
  return output_start +
         ((output_end - output_start) / (input_end - input_start)) *
             (input - input_start);
}

static void populate_buffer() {
  for (int x = 0; x < BUF_WIDTH; x++) {
    for (int y = 0; y < BUF_HEIGHT; y++) {
      double zx = map(x, 0, BUF_WIDTH, -r, r);
      double zy = map(y, 0, BUF_HEIGHT, -r, r);

      double smooth_color = exp(-sqrt(zx * zx + zy * zy));

      int i = 0;
      while (zx * zx + zy * zy < r * r && i < MAX_ITERATIONS) {
        double x_temp = zx * zx - zy * zy;
        zy = 2 * zx * zy + cy;
        zx = x_temp + cx;
        smooth_color += exp(-sqrt(zx * zx + zy * zy));
        i++;
      }

      buffer[y * BUF_WIDTH + x] =
          map(smooth_color, 0, MAX_ITERATIONS, SURFACE_DEPTH, 0);
    }
  }
}

void julia_init() {
  cx = 0.4;
  cy = -0.4;
  populate_buffer();
}
void julia_update(double delta) {
  counter += delta;
  cx = 1.5* sin(counter) + 0.3 * sin(6.123123*counter)+0.1*sin(17*counter);
  cy = 0.5 * cos(counter) + 0.3 * cos(5.156*counter)+0.1 *cos(13*counter);
  populate_buffer();
  glutPostRedisplay();
}

static double height_at(int buf_x, int buf_y) {
  double n = buffer[buf_y * BUF_WIDTH + buf_x];
  return -n * (double)HEIGHT_SCALE;
}

static void bufferVertex(int buf_x, int buf_y) {
  double n = buffer[buf_y * BUF_WIDTH + buf_x];
  glColor3f(n, 0.0, 1 - n);
  glVertex3d(buf_x, height_at(buf_x, buf_y), buf_y);
}

void normal(double x1, double y1, double z1, double x2, double y2, double z2,
            double x3, double y3, double z3) {
  double nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
  double ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
  double nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
  glNormal3d(nx, ny, nz);
}

void julia_draw() {
  glPushMatrix();
  glTranslated(0.0, 2.0, 0.0);
  glRotatef(10, 1.0, 0.0, 0.0);
  glScaled(.02, 1.0, .02);
  for (int y = 1; y < BUF_HEIGHT - 1; y++) {
    glBegin(GL_TRIANGLE_STRIP);
    bufferVertex(0, y);
    double prev_topleft_height = height_at(0, y);
    for (int x = 1; x < BUF_WIDTH - 1; x++) {
      double topright_height = height_at(x + 1, y);
      double bottomleft_height = height_at(x, y + 1);
      double bottomright_height = height_at(x+1, y+1);
      normal(x, prev_topleft_height, y, x, bottomleft_height, y + 1, x + 1,
             topright_height, y);
      bufferVertex(x, y + 1);
      normal(x, prev_topleft_height, y, x, bottomleft_height, y + 1, x + 1,
             topright_height, y);
      // normal(x+1, bottomright_height, y+1, x+1, topright_height, y, x, bottomleft_height, y+1);
      bufferVertex(x + 1, y);

      prev_topleft_height = height_at(x, y);
    }
    bufferVertex(BUF_WIDTH - 1, y + 1);
    glEnd();
  }
  glPopMatrix();
}
