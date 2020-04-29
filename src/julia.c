#include "julia.h"

#include <GL/freeglut.h>
#include <math.h>

#include "util.h"

// max iterations of sqaring/addition before iteration is halted
#define MAX_ITERATIONS 10

// resolution of julia set
#define BUF_WIDTH 500
#define BUF_HEIGHT 500

// Complex addition component
static double cx, cy;

// Escape radius
static double r = 2;

// The cumulative frame delta time since the surface was last recomputed
static double lastUpdate = 0.0;
// Cumulative frame delta counter
static double counter = 0.0;

// Stores the julia set picture data. values in [0.0, 1.0]
static double buffer[BUF_WIDTH * BUF_HEIGHT] = {0};

// linearly map input from one range to another, used to normalise values to the
// escape radius
static double map(double input, double input_start, double input_end,
                  double output_start, double output_end) {
  return output_start +
         ((output_end - output_start) / (input_end - input_start)) *
             (input - input_start);
}

// recompute the picture buffer based on cx and cy
static void populate_buffer() {
  for (int x = 0; x < BUF_WIDTH; x++) {
    for (int y = 0; y < BUF_HEIGHT; y++) {
      double zx = map(x, 0, BUF_WIDTH, -r, r);
      double zy = map(y, 0, BUF_HEIGHT, -r, r);

      // Use exponential of the negative magnitude of complex (zx + zx i)
      // for smooth height gradient even though the escape iterations are
      // integer based
      double smooth_color = exp(-sqrt(zx * zx + zy * zy));

      int iter = 0;
      while (zx * zx + zy * zy < r * r && iter < MAX_ITERATIONS) {
        double x_temp = zx * zx - zy * zy;
        zy = 2 * zx * zy + cy;
        zx = x_temp + cx;
        smooth_color += exp(-sqrt(zx * zx + zy * zy));
        iter++;
      }

      // update buffer with normalised value
      buffer[y * BUF_WIDTH + x] =
          map(smooth_color, 0, MAX_ITERATIONS, 1.0, 0.0);
    }
  }
}

// Checks whether enough time has passed for a 20fps update
// If so, recomputes surface
void julia_update(double delta) {
  counter += delta;
  // only recal at 20fps
  if (counter - lastUpdate > 1.0 / 30.0) {
    cx = 1.5 * sin(counter) + 0.3 * sin(6.123123 * counter) +
         0.1 * sin(17 * counter);
    cy = 0.5 * cos(counter) + 0.3 * cos(5.156 * counter) +
         0.1 * cos(13 * counter);
    populate_buffer();
    glutPostRedisplay();

    lastUpdate = counter;
  }
}

// Gets the world-space height that corresponds to a buffer index
static double height_at(int buf_x, int buf_y) {
  return -buffer[buf_y * BUF_WIDTH + buf_x];
}

// Draws and colors a vertex of the surface based on buffer indices
static void bufferVertex(int buf_x, int buf_y) {
  double n = buffer[buf_y * BUF_WIDTH + buf_x];
  glColor3f(n, 0.0, 1 - n);
  glVertex3d(buf_x, height_at(buf_x, buf_y), buf_y);
}

void julia_draw() {
  glEnable(GL_LIGHTING);
  glPushMatrix();
  // Make surface a unit square on ground
  glTranslated(0.0, 1.0, 0.0);
  glScaled(1.0 / (double)BUF_WIDTH, 1.0, 1.0 / (double)BUF_HEIGHT);
  for (int y = 1; y < BUF_HEIGHT - 1; y++) {
    glBegin(GL_TRIANGLE_STRIP);
    bufferVertex(0, y);
    double prev_topleft_height = height_at(0, y);
    for (int x = 1; x < BUF_WIDTH - 1; x++) {
      double topright_height = height_at(x + 1, y);
      double bottomleft_height = height_at(x, y + 1);
      normal(x, prev_topleft_height, y, x, bottomleft_height, y + 1, x + 1,
             topright_height, y);
      bufferVertex(x, y + 1);
      normal(x, prev_topleft_height, y, x, bottomleft_height, y + 1, x + 1,
             topright_height, y);
      bufferVertex(x + 1, y);

      prev_topleft_height = height_at(x, y);
    }
    bufferVertex(BUF_WIDTH - 1, y + 1);
    glEnd();
  }
  glPopMatrix();
}
