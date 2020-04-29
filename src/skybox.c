#include "skybox.h"

#include <GL/freeglut.h>

#include "util.h"

static char* filenames[] = {"./res/skybox/left.bmp",  "./res/skybox/back.bmp",
                            "./res/skybox/right.bmp", "./res/skybox/up.bmp",
                            "./res/skybox/front.bmp", "./res/skybox/down.bmp"};
static GLuint tex_ids[6] = {0};

static double x[] = {-0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5};
static double y[] = {-0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5};
static double z[] = {-0.5, -0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5};

void skybox_init() {
  for (int i = 0; i < 6; i++) {
    glGenTextures(1, &tex_ids[i]);
    glBindTexture(GL_TEXTURE_2D, tex_ids[i]);
    load_bmp(filenames[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  }
}

static void face(int i0, int i1, int i2, int i3, int tex_idx) {
  glBindTexture(GL_TEXTURE_2D, tex_ids[tex_idx]);
  glBegin(GL_QUADS);

  glTexCoord2i(0, 1);
  glVertex3d(x[i3], y[i3], z[i3]);

  glTexCoord2i(1, 1);
  glVertex3d(x[i2], y[i2], z[i2]);

  glTexCoord2i(1, 0);
  glVertex3d(x[i1], y[i1], z[i1]);

  glTexCoord2i(0, 0);
  glVertex3d(x[i0], y[i0], z[i0]);
  glEnd();
}

void skybox_draw() {
  glPushMatrix();
  glScaled(500, 500, 500);
  glEnable(GL_TEXTURE_2D);
  face(2, 6, 7, 3, 3);
  face(1, 5, 6, 2, 2);
  face(0, 4, 5, 1, 5);
  face(0, 1, 2, 3, 1);
  face(4, 0, 3, 7, 0);
  face(5, 4, 7, 6, 4);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}