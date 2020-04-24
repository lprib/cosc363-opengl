#include <math.h>
// #include <GL/gl.h>
#include <GL/freeglut.h>
#include "test.h"

float camera_height = 10;
float camera_angle = 0;

void display() {
    float lightPostion[4] = {0, 10, 10, 1};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(12 * sin(camera_angle), camera_height, 12 * cos(camera_angle), 0, 0, 0, 0, 1, 0);

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
    secret();
}

void special(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		camera_height++;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		camera_height--;
	}
	glutPostRedisplay();
}

void rotate_timer(int value)
{
	camera_angle+= .05;
	glutPostRedisplay();
	glutTimerFunc(50, rotate_timer, 0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Teapot");
	initialize();
	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutTimerFunc(50, rotate_timer, 0);
	glutMainLoop();
	return 0;
}
