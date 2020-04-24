#include <GL/freeglut.h>
#include <stdio.h>

void drawFloor() {
    glColor3f(0, 0.5, 0);

    for(int i = -50; i <= 50; i++) {
        glBegin(GL_LINES);
        glVertex3f(-50, -0.75, i);
		glVertex3f(50, -0.75, i);
		glVertex3f(i, -0.75, -50);
		glVertex3f(i, -0.75, 50);
		glEnd();
    }
}

void secret() {
    printf("hello");
}