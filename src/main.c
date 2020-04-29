#include <GL/freeglut.h>
#include <math.h>

#include "camera.h"
#include "fire.h"
#include "julia.h"
#include "museum.h"
#include "planets.h"
#include "serpinski.h"
#include "skybox.h"

int oldElapsedTime = 0;

void display() {
  float lightPostion[4] = {0, 10, 15, 1};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  camera_draw();

  glLightfv(GL_LIGHT0, GL_POSITION, lightPostion);

  glPushMatrix();
  // glTranslated(5.0, 0.0, 0.0);
  glTranslated(5.0, 0.5, 15.0);
  glRotated(-90, 1.0, 0.0, 0.0);
  glScaled(5.0, 2.0, 5.0);
  julia_draw();
  glPopMatrix();

  glPushMatrix();
  glTranslated(13.0, 0.1, 10.0);
  glScaled(2.0, 2.0, 2.0);
  serpinski_draw();
  glPopMatrix();

  glPushMatrix();
  glTranslated(7.5, 0.0, 8.5);
  fire_draw();
  glPopMatrix();

  glPushMatrix();
  glTranslated(0, -0.2, 0);
  glScaled(2.0, 2.0, 2.0);
  museum_draw();
  glPopMatrix();

  glPushMatrix();
  glTranslated(4.0, 0.5, 10.0);
  glScaled(2.0, 2.0, 2.0);
  planets_draw();
  glPopMatrix();

  skybox_draw();

  glFlush();
}

void initialize() {
  glClearColor(0.5, 0.5, 0.5, 1);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);

  glMatrixMode(GL_PROJECTION);

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  glLoadIdentity();

  camera_init();
  serpinski_init();
  fire_init();
  museum_init();
  planets_init();
  skybox_init();
}

void keyboard_func(unsigned char key, int x, int y) {
  serpinski_keyboard_func(key, x, y);
}

void special(int key, int x, int y) { camera_special_keys(key, x, y); }

void special_up(int key, int x, int y) { camera_special_keys_up(key, x, y); }

void update_timer(int value) {
  int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
  int delta = elapsedTime - oldElapsedTime;
  oldElapsedTime = elapsedTime;

  glutTimerFunc(16, update_timer, 0);

  double deltaSeconds = delta / 1000.0;

  camera_update(deltaSeconds);
  serpinski_update(deltaSeconds);
  julia_update(deltaSeconds);
  fire_update(deltaSeconds);
  planets_update(deltaSeconds);
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("The Crib");
  initialize();
  glutDisplayFunc(display);
  glutSpecialFunc(special);
  glutSpecialUpFunc(special_up);
  glutKeyboardFunc(keyboard_func);
  glutReshapeFunc(camera_reshape_func);
  update_timer(0);
  glutMainLoop();
  return 0;
}
