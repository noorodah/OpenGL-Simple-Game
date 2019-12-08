
/*
* SimpleLight.c
*
* Example program illustrating a simple use
* of lighting.  Creates six spheres of different
* fully saturated colors.  Adds a white light that
* slow rotates around the spheres.
* USAGE:
*    Press "r" key to toggle (off and on) running the animation
*    Press "s" key to single-step animation
*    The up arrow key and down array key control the
*         time step used in the animation rate.  This will
*         speed up or slow down the rate at which the light
*         revolves around the spheres.
*     Press ESCAPE to exit.
*
*/

#include <math.h>      // For math routines (such as sqrt & trig).
#include <stdio.h>
#include <stdlib.h>      // For the "exit" function
#include <GL/glut.h>   // OpenGL Graphics Utility Library
//#include "SimpleLight.h"


int qSize = 2;

int width = 500;
int height = 500;



float animRotation = 0.0f;
void drawScene(void);


// glutKeyboardFunc is called below to set this function to handle
//      all "normal" key presses.
void myKeyboardFunc(unsigned char key, int x, int y)
{
	if (key == 43)//'+')
	{
		qSize += 2;
	}
	else if (key == '-')
	{
		qSize -= 2;
	}
	else if (key == 27)
	{
		exit(0);
	}
}

/*
* drawScene() handles the animation and the redrawing of the
*      graphics window contents.
*/
void drawScene(void)
{
	// Clear the rendering window
	//glClearColor(red, green, blue, alpha) value is btwn 0.0f-1.0f
	glClearColor(0.7f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glutSolidSphere(1.0, 10, 10);

	//GLUquadric* q = gluNewQuadric();
	//gluSphere(q, 1.0, 4, 4);

	glMatrixMode(GL_PROJECTION); //Change to projection coordinates

	glLoadIdentity(); //Set modelview matrix to identity

	double aspectRatio = width / height;

	gluPerspective(90.0, 1.0, 0.01, 100.0);
	//gluOrtho2D();

	glMatrixMode(GL_MODELVIEW); //Change to world coordinates

	glLoadIdentity(); //Set modelview matrix to identity

	//Eye
	gluLookAt(
		0.0, 0.0, 5.0, //eye position in x,y,z
		0.0, 0.0, 0.0, //eye center position in x,y,z
		0.0, 1.0, 0.0  //eye up vector position in x,y,z
	);

	//glScalef(2.0f, 2.0f, 2.0f);

	glRotatef(animRotation, 0.0f, 1.0f, 0.0f);
	//glTranslatef(-3.0f, 0.0f, 0.0f);
	animRotation += 0.1f;

	if (animRotation == 360.0f)
	{
		animRotation = 0.0f;
	}

	glFrontFace(GL_CW);

	glBegin(GL_QUADS);

	//Cube's Front face
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3i(-qSize, qSize, qSize);
	glVertex3i(qSize, qSize, qSize);
	glVertex3i(qSize, -qSize, qSize);
	glVertex3i(-qSize, -qSize, qSize);

	//Cube's Back face
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3i(-qSize, qSize, -qSize);
	glVertex3i(qSize, qSize, -qSize);
	glVertex3i(qSize, -qSize, -qSize);
	glVertex3i(-qSize, -qSize, -qSize);

	//Cube's Right face
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3i(qSize, qSize, qSize);
	glVertex3i(qSize, qSize, -qSize);
	glVertex3i(qSize, -qSize, -qSize);
	glVertex3i(qSize, -qSize, qSize);

	//Cube's Left face
	glColor3f(0.0f, 0.35f, 0.5f);
	glVertex3i(-qSize, qSize, qSize);
	glVertex3i(-qSize, qSize, -qSize);
	glVertex3i(-qSize, -qSize, -qSize);
	glVertex3i(-qSize, -qSize, qSize);

	//Cube's Top face
	glColor3f(0.5f, 0.35f, 0.5f);
	glVertex3i(-qSize, qSize, qSize);
	glVertex3i(-qSize, qSize, -qSize);
	glVertex3i(qSize, qSize, -qSize);
	glVertex3i(qSize, qSize, qSize);

	//Cube's Bottom face
	glColor3f(0.5f, 0.87f, 0.22f);
	glVertex3i(-qSize, -qSize, qSize);
	glVertex3i(-qSize, -qSize, -qSize);
	glVertex3i(qSize, -qSize, -qSize);
	glVertex3i(qSize, -qSize, qSize);




	glEnd();

	// Flush the pipeline, swap the buffers
	glFlush();
	glutSwapBuffers();

	glutPostRedisplay();   // Trigger an automatic redraw for animation
}

// Called when the window is resized
//      w, h - width and height of the window in pixels.
void resizeWindow(int w, int h)
{
	float viewWidth = 7.0;         // Actually this is half of the width
	float viewHeight = 7.0;         // Again, this is half of the height
	glViewport(0, 0, w, h);
	h = (h == 0) ? 1 : h;
	w = (w == 0) ? 1 : w;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h < w) {
		viewWidth *= (float)w / (float)h;
	}
	else {
		viewHeight *= (float)h / (float)w;
	}
	glOrtho(-viewWidth, viewWidth, -viewHeight, viewHeight, -1.0, 1.0);

}


// Main routine
// Set up OpenGL, define the callbacks and start the main loop
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	// We're going to animate it, so double buffer 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //GLUT_SINGLE

	// Window position (from top corner), and size (width% and hieght)
	glutInitWindowPosition(600, 60);
	glutInitWindowSize(width, height);
	int windowID = glutCreateWindow("Ezz");

	// Call this whenever window needs redrawing
	glutDisplayFunc(drawScene);

	// Set up callback functions for key presses
	glutKeyboardFunc(myKeyboardFunc);         // Handles "normal" ascii symbols

	// Set up the callback function for resizing windows
	glutReshapeFunc(resizeWindow);

	// Call this for background processing
	// glutIdleFunc( myIdleFunction );



	fprintf(stdout, "Arrow keys control speed.  Press \"r\" to run,  \"s\" to single step.\n");

	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop();

	return(0);   // This line is never reached.
}

void myKeyboardFunc(unsigned char key, int x, int y);
void mySpecialKeyFunc(int key, int x, int y);
void initRendering();
void resizeWindow(int w, int h);