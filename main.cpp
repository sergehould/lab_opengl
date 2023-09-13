/*////////////////////////////////////////////////

	SH	27 June 2023	Code to prove a concept.	
						Explorer16-32 communnicates serially
						with the openGL. The position of the propeller
						is set by the rx value. The rx sets the tx (loop back signal)
						to re-transmit to Ex16-32. Works perfecly - no latency.
	SH	28 June 2023	Add the ball and ball position with gravity effect.
						Haptic code tested ok
	SH	28 08 2023		Use receive_int16() function to receive 16 bit integer 
						rather than the previous byte (8 bits).

	See: Vanier\courses_C_drive\MPLAB_projects_original\projects\haptic_balance

	TODO:
		Fix the com port issue of not being able to use a string
		The port should be set by the user when starting up 

	Note: to set platform, properties->General-> platform Toolset -> select Visual Studio 2019 (v142)

////////////////////////////////////////////////*/


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
	glutReshapeFunc(changeSize);
	glutMainLoop();
}

