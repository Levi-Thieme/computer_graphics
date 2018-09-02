/* Example program for Window initialization and drawing */

#include "stdafx.h"
#include <GL/freeglut.h>
#include <math.h>

void init_window(int argc, char** argv)
{
	glutInit(&argc, argv);				/* Initialize GLUT */
	glutInitDisplayMode(GLUT_RGB); 		/* Specify display mode */

	glutInitWindowSize(800, 800);		/* Set window size */
	glutInitWindowPosition(0, 0);		/* Set window position */
	glutCreateWindow("Window Creation");	/* Create Window */
}

void other_init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);		/* Set background color */
	glMatrixMode(GL_PROJECTION);		/* Modify Projection Matrix */
	glLoadIdentity();				/* Set to identity matrix */
	glOrtho(0.0, 800.0, 800.0, 0.0, -1.0, 1.0);	/* Orthographic viewing volume */

												//	glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	/* Clear color values */
	glColor3f(0.0, 0.0, 1.0);			/* Set foreground color */
	glPointSize(4.0);				/* Set point size */

	glColor3f(1.0, 0.0, 1.0);
	glLineWidth(10.0);			/* Set line width */

	glBegin(GL_LINE_STRIP);
	glVertex3f(100, 100, 0);
	glVertex3f(150, 150, 0);
	glVertex3f(200, 200, 0);
	glVertex3f(300, 400, 0);
	glVertex3f(450, 600, 0);
	glEnd();
	glFlush();						/* Clear event buffer */
}

void main(int argc, char** argv)
{
	init_window(argc, argv);			/* Initialize window */
	other_init();						/* Initialize other parameter */
	glutDisplayFunc(display); 			/* Redisplay callback event handling */
	glutMainLoop();					/* Main event loop */
}
