#include <GL/freeglut.h>
#include <math.h>
#include "camera.h"


float camera_height = 10;
float camera_angle = 0;

int oldElapsedTime = 0;

void drawFloor() {
  glColor3f(0, 0.5, 0);

  for (int i = -50; i <= 50; i++) {
    glBegin(GL_LINES);
    glVertex3f(-50, -0.75, i);
    glVertex3f(50, -0.75, i);
    glVertex3f(i, -0.75, -50);
    glVertex3f(i, -0.75, 50);
    glEnd();
  }
}

void display() {
  float lightPostion[4] = {0, 10, 10, 1};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  camera_draw();
  // gluLookAt(12 * sin(camera_angle), camera_height, 12 * cos(camera_angle), 0, 0,
  //           0, 0, 1, 0);

  glLightfv(GL_LIGHT0, GL_POSITION, lightPostion);

  glDisable(GL_LIGHTING);
  drawFloor();

  glEnable(GL_LIGHTING);
  glColor3f(0, 1, 1);
  glutSolidTeapot(1);

  glFlush();
}

void initialize() {
  glClearColor(1, 1, 1, 1);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);

  camera_init();
}

void special(int key, int x, int y) {
  camera_special_keys(key, x, y);
  glutPostRedisplay();
}

void special_up(int key, int x, int y) {
  camera_special_keys_up(key, x, y);
}

void update_timer(int value) {
  int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
  int delta = elapsedTime - oldElapsedTime;
  oldElapsedTime = elapsedTime;

  glutTimerFunc(16, update_timer, 0);

  double deltaSeconds = delta / 1000.0;

  camera_update(delta);
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Teapot");
  initialize();
  glutDisplayFunc(display);
  glutSpecialFunc(special);
  glutSpecialUpFunc(special_up);
  update_timer(0);
  glutMainLoop();
  return 0;
}
