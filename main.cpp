/*
*	Author		Date	Version 	History
*
*
*
*
*/


#include "freeglut/include/GL/glut.h"
#include "freeglut/include/GL/freeglut.h"
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include "serial.h"
#include "tick.h"


// callback function
void renderScene(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glClearColor(0.8f, 0.8f, 1.0f, 0); // BG color
	glTranslatef(0, 0, -2);
	glColor3f(0, 0, 1);  // Cube color
	glutWireCube(1.0f); // Big cube
	glutSwapBuffers();
}

void changeSize(int w, int h){
	float ratio = w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(90, ratio, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(640, 640);
	glutCreateWindow("OpenGL lab");
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutMainLoop();
}

