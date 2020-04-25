#include "camera.h"

#include <GL/freeglut.h>
#include <math.h>
#include <stdbool.h>

// In units per second (used with frame delta time)
#define MOVE_SPEED 5
#define ANGLE_SPEED 3

//camera angle
static double angle = 0.0;
//camera x position
static double cx = 0.0;
//camera y position
static double cz = 0.0;

//look vector
static double look_x = 0.0;
static double look_z = -1.0;

// Which keys are currently held down (left, right, up, down) arrows respectively
static bool keysDown[] = {false, false, false, false};

void camera_init() {
  glutIgnoreKeyRepeat(true);
  glFrustum(-0.5, 0.5, -0.5, 0.5, 1.0, 1000.0);
}

// Updates the state of pressed keys in keysDownArray, based on the key
// provided. if press is true, the specified key will be set. if press is false
// it will be reset.
static void update_key_array(int key, bool press) {
  switch (key) {
    case GLUT_KEY_LEFT:
      keysDown[0] = press;
      break;
    case GLUT_KEY_RIGHT:
      keysDown[1] = press;
      break;
    case GLUT_KEY_UP:
      keysDown[2] = press;
      break;
    case GLUT_KEY_DOWN:
      keysDown[3] = press;
      break;
  }
}

void camera_special_keys(int key, int x, int y) { update_key_array(key, true); }

void camera_special_keys_up(int key, int x, int y) {
  update_key_array(key, false);
}

void camera_update(double delta) {
  // left
  if (keysDown[0]) {
    angle -= ANGLE_SPEED * delta;
    look_x = sin(angle);
    look_z = -cos(angle);
    glutPostRedisplay();
  }

  // right
  if (keysDown[1]) {
    angle += ANGLE_SPEED * delta;
    look_x = sin(angle);
    look_z = -cos(angle);
    glutPostRedisplay();
  }

  // up
  if (keysDown[2]) {
    cx += look_x * MOVE_SPEED * delta;
    cz += look_z * MOVE_SPEED * delta;
    glutPostRedisplay();
  }

  // down
  if (keysDown[3]) {
    cx -= look_x * MOVE_SPEED * delta;
    cz -= look_z * MOVE_SPEED * delta;
    glutPostRedisplay();
  }
}

void camera_draw() {
  gluLookAt(cx, 1.0f, cz, cx + look_x, 1.0f, cz + look_z, 0.0f, 1.0f, 0.0f);
}