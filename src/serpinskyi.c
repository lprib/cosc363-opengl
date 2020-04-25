#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "serpinski.h"

#define LERP_SPEED 4
#define Z_FIGHT_OFFSET 0.001
#define SUBDIVIDE_LIMIT 7

#define ZERO_POINT \
  { 0.0, 0.0, 0.0 }

static Tetra_t INITIAL = {
    .from_points = {ZERO_POINT, ZERO_POINT, ZERO_POINT, ZERO_POINT},
    .to_points = {{0.0, 1.0, 0.0},
                  {1.0, 0.0, 1.0},
                  {1.0, 0.0, -1.0},
                  {-1.0, 0.0, 0.0}},
    .normalize_bottom = 0.0,
    .normalize_top = 1.0};

static int num_tetras = 0;
static Tetra_t* tetras = NULL;
static int num_subdivides = 0;

static double current_lerp = 0.0;

static void glVertexPoint(Point_t point) {
  glVertex3d(point.x, point.y, point.z);
}

static double lerp_double(double d0, double d1, double t) {
  return d0 + t * (d1 - d0);
}

static Point_t lerp_point(Point_t p0, Point_t p1, double t) {
  Point_t ret = {lerp_double(p0.x, p1.x, t), lerp_double(p0.y, p1.y, t),
                 lerp_double(p0.z, p1.z, t)};

  return ret;
}

static void normal(Point_t p1, Point_t p2, Point_t p3) {
  float nx, ny, nz;
  nx = p1.y * (p2.z - p3.z) + p2.y * (p3.z - p1.z) + p3.y * (p1.z - p2.z);
  ny = p1.z * (p2.x - p3.x) + p2.z * (p3.x - p1.x) + p3.z * (p1.x - p2.x);
  nz = p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y);
  glNormal3f(nx, ny, nz);
}

static void draw_tetra(Tetra_t* tet, double lerp_amount) {
  glColor3f(tet->normalize_top, 1.0, 1.0 - tet->normalize_bottom);
  Point_t p0 = lerp_point(tet->from_points[0], tet->to_points[0], lerp_amount);
  Point_t p1 = lerp_point(tet->from_points[1], tet->to_points[1], lerp_amount);
  Point_t p2 = lerp_point(tet->from_points[2], tet->to_points[2], lerp_amount);
  Point_t p3 = lerp_point(tet->from_points[3], tet->to_points[3], lerp_amount);

  glPushMatrix();
  glBegin(GL_TRIANGLES);
  normal(p0, p1, p2);
  glVertexPoint(p0);
  glVertexPoint(p1);
  glVertexPoint(p2);

  normal(p0, p2, p3);
  glVertexPoint(p0);
  glVertexPoint(p2);
  glVertexPoint(p3);

  normal(p0, p3, p1);
  glVertexPoint(p0);
  glVertexPoint(p3);
  glVertexPoint(p1);

  normal(p1, p3, p2);
  glVertexPoint(p1);
  glVertexPoint(p3);
  glVertexPoint(p2);

  glEnd();
  glPopMatrix();
}

static void print_point(Point_t p) { printf("(%lf, %lf, %lf)", p.x, p.y, p.z); }

static void print_tetra(Tetra_t* tet) {
  printf("from: ");
  for (int i = 0; i < 4; i++) {
    print_point(tet->from_points[i]);
    printf(",  ");
  }
  printf("\nto: ");
  for (int i = 0; i < 4; i++) {
    print_point(tet->to_points[i]);
    printf(",  ");
  }
  printf("\nnb: %lf\n", tet->normalize_bottom);
  printf("\nnt: %lf\n", tet->normalize_top);
}

typedef struct {
  Tetra_t tets[4];
} SubdividedTetra_t;

static Tetra_t single_subdivide(Tetra_t* parent, int stationary_vertex_idx) {
  Tetra_t child = {0};
  child.normalize_top = 0.0;
  child.normalize_bottom = 0.0;
  for (int i = 0; i < 4; i++) {
    child.from_points[i] = parent->to_points[i];
    if (i == stationary_vertex_idx) {
      child.to_points[i] = parent->to_points[i];
    } else {
      child.to_points[i] = lerp_point(
          parent->to_points[i], parent->to_points[stationary_vertex_idx], 0.5);
    }
  }

  child.from_points[0].y += Z_FIGHT_OFFSET;

  return child;
}

static SubdividedTetra_t subdivide_tetra(Tetra_t* parent) {
  SubdividedTetra_t subdiv = {0};
  for (int i = 0; i < 4; i++) {
    subdiv.tets[i] = single_subdivide(parent, i);
    double top = parent->normalize_top;
    double bottom = parent->normalize_bottom;
    double middle = lerp_double(top, bottom, 0.5);
    if (i == 0) {
      // topmost child
      subdiv.tets[i].normalize_top = top;
      subdiv.tets[i].normalize_bottom = middle;
    } else {
      subdiv.tets[i].normalize_top = middle;
      subdiv.tets[i].normalize_bottom = bottom;
    }
  }
  return subdiv;
}

static void subdivide_all() {
  Tetra_t* new_buffer = malloc(sizeof(Tetra_t) * num_tetras * 4);
  for (int i = 0; i < num_tetras; i++) {
    SubdividedTetra_t subbed = subdivide_tetra(&tetras[i]);
    for (int offset = 0; offset < 4; offset++) {
      new_buffer[i * 4 + offset] = subbed.tets[offset];
    }
  }
  num_tetras *= 4;
  free(tetras);
  tetras = new_buffer;
}

void serpinski_update(double delta) {
  current_lerp += LERP_SPEED * delta;
  glutPostRedisplay();
}

void serpinski_draw() {
  for (int i = 0; i < num_tetras; i++) {
    draw_tetra(
        &tetras[i],
        sin(current_lerp + 2 * M_PI * tetras[i].normalize_top) / 2.0 + 1.0);
  }
}

void serpinski_keyboard_func(unsigned char key, int x, int y) {
  if (key == 's') {
    num_subdivides++;
    if (num_subdivides >= SUBDIVIDE_LIMIT) {
      free(tetras);
      num_tetras = 1;
      tetras = (Tetra_t*)malloc(sizeof(Tetra_t));
      tetras[0] = INITIAL;
      num_subdivides = 0;
    } else {
      subdivide_all();
      glutPostRedisplay();
    }
  }
}

void serpinski_init() {
  num_tetras = 1;
  tetras = (Tetra_t*)malloc(sizeof(Tetra_t));
  tetras[0] = INITIAL;
}