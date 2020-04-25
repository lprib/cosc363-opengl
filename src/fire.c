#include "fire.h"

#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

#define SPEED 0.5
#define MAX_SIZE 0.1

#define NUM_PARTICLES 200

typedef struct {
  double x, z, n, limit;
} Particle_t;

Particle_t particles[NUM_PARTICLES];

static void update_particle(Particle_t* p, double delta) {
  p->n += SPEED * delta;
  if (p->n >= p->limit) {
    p->n = 0;
  }
}

static void draw_particle(Particle_t* p) {
  glPushMatrix();
  double scale_factor = MAX_SIZE * (1 - p->n);
  glTranslated(p->x, p->n, p->z);
  glutSolidSphere(scale_factor, 10, 10);
  glPopMatrix();
}

static double random_range(double start, double end) {
  return (double)rand() / (double)(RAND_MAX / (end - start)) + start;
}

void fire_update(double delta) {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    update_particle(&particles[i], delta);
  }
}

void fire_init() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    particles[i] = (Particle_t){random_range(0.0, 1.0), random_range(0.0, 1.0),
                                0.0, random_range(0.2, 1.0)};
  }
}

void fire_draw() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    draw_particle(&particles[i]);
  }
}