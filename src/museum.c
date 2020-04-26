#include "museum.h"

#include <GL/freeglut.h>
#include <IL/il.h>

#include "util.h"

GLuint brick_id;

static double roof_x[] = {0, 8, 8, 0, 4, 4};
static double roof_y[] = {6, 6, 6, 6, 8, 8};
static double roof_z[] = {0, 0, 8, 8, 0, 8};

static void rect_quad(double* xs, double* ys, double* zs, int i0, int i1,
                      int i2, int i3) {
  normal(xs[i0], ys[i0], zs[i0], xs[i1], ys[i1], zs[i1], xs[i2], ys[i2],
         zs[i2]);
  glTexCoord2d(0, 0);
  glVertex3d(xs[i0], ys[i0], zs[i0]);
  glTexCoord2d(1, 0);
  glVertex3d(xs[i1], ys[i1], zs[i1]);
  glTexCoord2d(1, 1);
  glVertex3d(xs[i2], ys[i2], zs[i2]);
  glTexCoord2d(0, 1);
  glVertex3d(xs[i3], ys[i3], zs[i3]);
}

static void draw_rect(double x, double y, double z, double sx, double sy,
                      double sz) {
  double xs[] = {x, x + sx, x + sx, x, x, x + sx, x + sx, x};
  double ys[] = {y, y, y + sy, y + sy, y, y, y + sy, y + sy};
  double zs[] = {z, z, z, z, z + sz, z + sz, z + sz, z + sz};

  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, brick_id);
  glBegin(GL_QUADS);

  rect_quad(xs, ys, zs, 0, 1, 2, 3);
  rect_quad(xs, ys, zs, 1, 5, 6, 2);
  rect_quad(xs, ys, zs, 5, 4, 7, 6);
  rect_quad(xs, ys, zs, 4, 0, 3, 7);
  rect_quad(xs, ys, zs, 3, 2, 6, 7);
  rect_quad(xs, ys, zs, 4, 5, 1, 0);

  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

static void roof_normal(int idx0, int idx1, int idx2) {
  normal(roof_x[idx0], roof_y[idx0], roof_z[idx0], roof_x[idx1], roof_y[idx1],
         roof_z[idx1], roof_x[idx2], roof_y[idx2], roof_z[idx2]);
}

static void roof_quad(int idx0, int idx1, int idx2, int idx3) {
  roof_normal(idx0, idx1, idx2);
  glTexCoord2d(0, 0);
  glVertex3d(roof_x[idx0], roof_y[idx0], roof_z[idx0]);
  glTexCoord2d(1, 0);
  glVertex3d(roof_x[idx1], roof_y[idx1], roof_z[idx1]);
  glTexCoord2d(1, 1);
  glVertex3d(roof_x[idx2], roof_y[idx2], roof_z[idx2]);
  glTexCoord2d(0, 1);
  glVertex3d(roof_x[idx3], roof_y[idx3], roof_z[idx3]);
}

static void draw_roof() {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, brick_id);
  glColor3d(1, 1, 1);
  glBegin(GL_QUADS);
  roof_quad(0, 4, 5, 3);
  roof_quad(1, 2, 5, 4);
  roof_quad(1, 0, 3, 2);
  roof_quad(0, 0, 1, 4);
  roof_quad(5, 5, 2, 3);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void museum_draw() {
  glPushMatrix();
  glColor3f(0.6, 0.6, 0.6);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, brick_id);
  draw_rect(0, 0, 0, 8.0, 0.1, 8.0);
  draw_rect(0, 0, 2.0, 0.1, 6.0, 6.0);
  draw_rect(7.9, 0, 2.0, 0.1, 6.0, 6.0);
  draw_rect(0.1, 0, 7.9, 7.8, 6.0, 0.1);
  draw_rect(0, 0, 1.9, 3.0, 6.0, 0.1);
  draw_rect(5.0, 0, 1.9, 3.0, 6.0, 0.1);
  glDisable(GL_TEXTURE_2D);
  draw_roof();
  glPopMatrix();
}

void museum_init() {
  glGenTextures(1, &brick_id);
  glBindTexture(GL_TEXTURE_2D, brick_id);
  load_bmp("./res/bricks.bmp");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}