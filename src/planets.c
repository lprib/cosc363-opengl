#include "planets.h"

#include <GL/freeglut.h>
#include <math.h>
#include <stdbool.h>

#include "util.h"

typedef struct {
  GLuint texture_id;
  char* texture_filename;
  double orbit_speed;
  double rotation_speed;
  double orbit_dist;
  double size;
} Planet_t;

#define SIM_SPEED 3
#define NUM_PLANETS 3

static Planet_t planets[] = {{0, "./res/sun.bmp", 0, 0.01, 0, 0.5},
                             {0, "./res/earth.bmp", 0.2, -0.3, 1, 0.2},
                             {0, "./res/mars.bmp", 0.1, 0.4, 1.5, 0.1}};

static double counter = 0.0;

static double x[] = {-0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5};
static double y[] = {-0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5};
static double z[] = {-0.5, -0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5};
// tex coords for front, right, back
static double s1[] = {0.25, 0.5, 0.5, 0.25, 1, 0.75, 0.75, 1};
static double t1[] = {0.5, 0.5, 0.75, 0.75, 0.5, 0.5, 0.75, 0.75};
static double s2[] = {0, 0, 0, 0, 0.25, 0.25, 0.25, 0.25};
static double t2[] = {0.5, 0.25, 1, 0.75, 0.5, 0.25, 1, 0.75};

static void quad(int i0, int i1, int i2, int i3, bool first) {
  normal(x[i0], y[i0], z[i0], x[i1], y[i1], z[i1], x[i2], y[i2], z[i2]);
  double* s = first ? s1 : s2;
  double* t = first ? t1 : t2;
  glTexCoord2d(s[i0], t[i0]);
  glVertex3d(x[i0], y[i0], z[i0]);

  glTexCoord2d(s[i1], t[i1]);
  glVertex3d(x[i1], y[i1], z[i1]);

  glTexCoord2d(s[i2], t[i2]);
  glVertex3d(x[i2], y[i2], z[i2]);

  glTexCoord2d(s[i3], t[i3]);
  glVertex3d(x[i3], y[i3], z[i3]);
}

static void draw_planet(Planet_t* p) {
  glPushMatrix();
  glTranslated(p->orbit_dist * sin(p->orbit_speed * counter * SIM_SPEED), 0.0,
               p->orbit_dist * cos(p->orbit_speed * counter * SIM_SPEED));
  glRotated(counter * p->rotation_speed * 360 * SIM_SPEED, 0, 1, 0);
  glScaled(p->size, p->size, p->size);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, p->texture_id);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBegin(GL_QUADS);
  quad(0, 1, 2, 3, true);
  quad(1, 5, 6, 2, true);
  quad(5, 4, 7, 6, true);
  quad(7, 3, 2, 6, false);
  quad(4, 0, 3, 7, false);
  quad(5, 1, 0, 4, false);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void planets_init() {
  for (int i = 0; i < NUM_PLANETS; i++) {
    glGenTextures(1, &planets[i].texture_id);
    glBindTexture(GL_TEXTURE_2D, planets[i].texture_id);
    load_bmp(planets[i].texture_filename);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
}

void planets_update(double delta) { counter += delta; }

void planets_draw() {
  for (int i = 0; i < NUM_PLANETS; i++) {
    draw_planet(&planets[i]);
  }
}