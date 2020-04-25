#include "fire.h"

#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

#define SPEED 0.5
#define MAX_SIZE 0.1
#define VERTICAL_SCALE 2

#define NUM_PARTICLES 500

double count = 0.0;

typedef struct {
  double x, z, n, limit, speed;
} Particle_t;

Particle_t particles[NUM_PARTICLES];

static void update_particle(Particle_t* p, double delta) {
  p->n += p->speed * delta;
  if (p->n >= p->limit) {
    p->n = 0;
  }
}

static void set_color(double life_stage) {
  if(life_stage < 0.25) {
    double p = life_stage * 4.0;
    glColor3d(1.0, 1.0, 1.0-p);
  } else if (life_stage < 0.5) {
    double p = (life_stage - 0.25) * 4.0;
    glColor3d(1.0, 1.0-p, 0.0);
  } else {
    double p = (life_stage - 0.5) * 2.0;
    glColor3d(1.0-p, 0.0, 0.0);
  }
}

static void draw_particle(Particle_t* p) {
  glPushMatrix();
  set_color(p->n / p->limit);
  double scale_factor = MAX_SIZE * (1 - p->n);
  glTranslated(p->x, VERTICAL_SCALE * p->n, p->z);
  glutSolidSphere(scale_factor, 10, 10);
  glPopMatrix();
}

static double random_range(double start, double end) {
  return (double)rand() / (double)(RAND_MAX / (end - start)) + start;
}

void fire_update(double delta) {
  count += delta;
  for (int i = 0; i < NUM_PARTICLES; i++) {
    update_particle(&particles[i], delta);
  }
}

void fire_init() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    particles[i] = (Particle_t){random_range(0.0, 1.0),
                                random_range(0.0, 1.0),
                                0.0,
                                random_range(0.5, 1.0),
                                random_range(0.2, 0.8)};
  }
}

void fire_draw() {
  glDisable(GL_LIGHTING);
  for (int i = 0; i < NUM_PARTICLES; i++) {
    draw_particle(&particles[i]);
  }
  glEnable(GL_LIGHTING);
}