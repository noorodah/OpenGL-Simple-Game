
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
#include <string>

#include <math.h>      // For math routines (such as sqrt & trig).
#include <stdio.h>
#include <stdlib.h>      
#include<GL/glew.h>// For the "exit" function
#include <GL/glut.h>
// OpenGL Graphics Utility Library
#include  <IL/il.h>
#include  <IL/ilu.h>
#include  <IL/ilut.h>

//#include "SimpleLight.h"


GLint loadTexture(char const* filename);
void drawEnemy();
float enemyPosX = 0.0;
float enemyPosY = 0.0;
GLuint textID1;
void drawPoint();
void drawFloor();
char s[30];
int qSize = 2;
GLfloat xpos = 0.0;
GLfloat ypos = 0.0;
int width = 1200;
int height = 800;
double eyePosX = 0.0;
double eyePosY = 0.0;
double eyePosZ = -5.0;
void TrackPos();

void drawWalls();
int charPosX = 1;
GLfloat pointV[] = { 1.0f,0.0f 
,-7.0f,0.0f
,-10.0f,-3.0f,
 -20.0f,-2.0f,
-30.0f,0.0f,
-40.0f,2.0f,
-50.0f,1.2f,
-60.0f,-1.0f,
-70.0f,0.0f,
-80.0f,-3.0f};
GLfloat pointVE[] = { -3.0f,0.0f
,-10.0f,0.0f
,-20.0f,0.0f,
 -30.0f,0.0f,
-40.0f,0.0f,
-50.0f,0.0f,
-60.0f,0.0f,
-70.0f,0.0f,
-80.0f,0.0f,
-90.0f,0.0f };

// Values that control the material properties.
float Noemit[4] = { 0.0, 0.0, 0.0, 1.0 };
float SphShininess = 50;   // Specular exponent for the spheres.
float SphAmbDiff[6][4] = {      // The ambient/diffuse colors of the six spheres 
   { 0.5, 0.0, 0.0, 1.0 },         // Red
   { 0.5, 0.5, 0.0, 1.0 },         // Yellow
   { 0.0, 0.5, 0.0, 1.0 },         // Green
   { 0.0, 0.5, 0.5, 1.0 },         // Cyan
   { 0.0, 0.0, 0.5, 1.0 },         // Blue
   { 0.5, 0.0, 0.5, 1.0 }         // Magenta
};
float SphSpecular[4] = { 1, 1, 1, 1.0 };

// Lighting values
float ambientLight[4] = { 0.2, 0.2, 0.2, 1.0 };
float Lt0amb[4] = { 0.3, 0.3, 0.3, 1.0 };
float Lt0diff[4] = { 1.0, 1.0, 1.0, 1.0 };
float Lt0spec[4] = { 1.0, 1.0, 1.0, 1.0 };

float zeroPos[4] = { 0, 0, 0, 1 };         // Origin (homogeneous representation)
float dirI[4] = { 1, 0, 0, 0 };            // Direction of unit vector I (point at infinity)

int LightIsPositional = 0;

int RunMode = 1;      // Used as a boolean (1 or 0) for "on" and "off"

// The next global variable controls the animation's state and speed.
float CurrentAngle = 0.0f;         // Angle in degrees
float AnimateStep = 0.5f;         // Rotation step per update
void drawScene(void);
void drawCharacter();
void drawFloor();
void timer(int);
float scorePos=-4;
int score = 0;


bool started = false;
// glutKeyboardFunc is called below to set this function to handle
//      all "normal" key presses.
void myKeyboardFunc(unsigned char key, int x, int y)
{
	if (key =='a')
	{
		eyePosX += 0.15;
		xpos += 0.15;
		scorePos += 0.15;
		
	}
	else if (key == 'd')
	{
		
		xpos -= 0.15;
		eyePosX -= 0.15;
		scorePos -= 0.15;

	}
	else if (key == 'w') {
		if (ypos < 5)
			ypos += 0.15;
		
	}

	else if (key == 's') {
		if (ypos > 0)
			ypos -= 0.15;
		
	}
	else if (key == 27)
	{
		exit(0);
	}
}

// glutSpecialFunc is called below to set this function to handle
//      all "special" key presses.  See glut.h for the names of
//      special keys.
void mySpecialKeyFunc(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		if (AnimateStep < 5.0) {         // Don't let speed get very big.
			AnimateStep *= sqrt(2.0);      // Increase the angle increment
		}
		break;
	case GLUT_KEY_DOWN:
		if (AnimateStep > 1.0e-3) {      // Avoid underflow problems.
			AnimateStep /= sqrt(2.0);   // Decrease the angle increment
		}
		break;
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

		eyePosX, eyePosY, eyePosZ, //eye position in x,y,z
		eyePosX, eyePosY, eyePosZ + 5.0, //eye center position in x,y,z
		0.0, 1.0, 0.0  //eye up vector position in x,y,z
	);


	drawFloor();
	drawWalls();
	drawPoint();

	drawCharacter();
	drawEnemy();
	
	float topCorn = ypos - 3;
	float botCorn = ypos - 4;
	float topXcorn = xpos + charPosX-1;
	float botXcorn = xpos + charPosX;
	int y;
	if (topCorn < 0.3 && topCorn >= 0.0)
		y = 0;
	else
		y = 1;
	if ((y) == 0 && xpos + charPosX == 1)
		pointV[0] = -110.0;



	for (int i = 0; i < 19; i +=2) {

		if (pointV[i] >= topXcorn && pointV[i] <= botXcorn)
			if ((pointV[i + 1] >= botCorn && pointV[i + 1] <= topCorn) || (pointV[i + 1] <= botCorn && pointV[i + 1] >= topCorn)){
				pointV[i] = -10000;
				
				score += 1;
				TrackPos();
			}

	}

	//LOSING
	for (int i = 0; i < 19; i += 2) {

		if (pointVE[i] >= topXcorn && pointVE[i] <= botXcorn)
			if ((pointVE[i + 1] >= botCorn && pointVE[i + 1] <= topCorn) || (pointVE[i + 1] <= botCorn && pointVE[i + 1] >= topCorn)) {
				pointVE[i] = -10000;
				score += 1;
				fprintf(stdout,"Lost");
				exit(0);
			}

	}
	
	

	// Flush the pipeline, swap the buffers
	glFlush();
	glutSwapBuffers();

	glutPostRedisplay();   // Trigger an automatic redraw for animation
}



void drawCharacter() {
	glPushMatrix();
	glTranslatef(xpos,ypos,0.0f);
	glBegin(GL_QUADS);
	glColor3f(0.5f, 7.0f, 0.0f);
	glVertex3i(charPosX-1, -4, 0);
	
	glVertex3i(charPosX - 1, -3, 0);


	glVertex3i(charPosX, -3, 0);
	
	glVertex3i(charPosX , -4, 0);
	glEnd();
	glPopMatrix();
	
}

void drawPoint() {
	glColor3f(1.0f, 1.0f, 0.0f);
	glPointSize(20);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	for (int i =0;i<19;i+=2)
	glVertex2f(pointV[i], pointV[i+1]);
	glEnd();
	glDisable(GL_POINT_SMOOTH);
}
void drawEnemy() {

	glPushMatrix();
	glTranslatef(enemyPosX, enemyPosY, 0.0f);
		

	glColor3f(0+.4f, 0.3f, 0.0f);
	glPointSize(100);
	glBegin(GL_POINTS);
	for (int i = 0; i < 19; i += 2)
		glVertex2f(pointVE[i], pointVE[i + 1]);
	glEnd();
	glPopMatrix();
	
}
void drawWalls() {



	int start1 = -2;
	int start2 = -3;
	int finish1 = 3;
	

	for (int i = 0; i < 20; i++) {
	glBegin(GL_QUADS);
	glColor3f(0.5f, 7.0f, 9.0f);
	glVertex3i(start1, finish1, 0);
	glVertex3i(start2, finish1, 0);
	glVertex3i(start2, -4, 0);
	glVertex3i(start1, -4, 0);

	glEnd();
	start1 -= 7;
	start2 -= 7;
	
}
}

void drawFloor() {
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3i(-200, -6, 0);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3i(-200, -4, 0);

	
	glVertex3i(100, -6, 0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3i(100, -4, 0);
	glEnd();
}
// Initialize OpenGL's rendering modes
void initRendering()
{
	glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on

	glEnable(GL_LIGHTING);      // Enable lighting calculations
	glEnable(GL_LIGHT0);      // Turn on light #0.

	// Set global ambient light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	// Light 0 light values.  Its position is set in drawScene().
	glLightfv(GL_LIGHT0, GL_AMBIENT, Lt0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lt0spec);
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

	gluPerspective(60.0,w/h,0.0,-50);

}

void TrackPos() {
	fprintf(stdout, "Score is  : " );
	fprintf(stdout, "%i\n", score);
	/*float p = xpos +charPosX;
	float b = -3 + ypos;
	float topCorn = ypos - 3;
	float botCorn = ypos - 4;
	float topXcorn = xpos + charPosX - 1;
	float botXcorn = xpos + charPosX;
	fprintf(stdout, "  TopXaxis = ");
	fprintf(stdout, "%f", topXcorn);
	fprintf(stdout, "  BotXaxis = ");
	fprintf(stdout, "%f\n", botXcorn);

	fprintf(stdout, "  TopYaxis = ");
	fprintf(stdout, "%f", topCorn);
	fprintf(stdout, "  botYaxis =  ");
	fprintf(stdout, "%f\n",botXcorn);*/
	
}
// Main routine
// Set up OpenGL, define the callbacks and start the main loop
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	// We're going to animate it, so double buffer 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //GLUT_SINGLE

	// Window position (from top corner), and size (width% and hieght)
	glutInitWindowPosition(600, 60);
	glutInitWindowSize(width, height);
	int windowID = glutCreateWindow("Ezz");

	// Call this whenever window needs redrawing
	glutDisplayFunc(drawScene);

	// Set up callback functions for key presses
	glutKeyboardFunc(myKeyboardFunc);         // Handles "normal" ascii symbols
	glutSpecialFunc(mySpecialKeyFunc);      // Handles "special" keyboard keys

	// Set up the callback function for resizing windows
	glutReshapeFunc(resizeWindow);

	// Call this for background processing
	// glutIdleFunc( myIdleFunction );

	//  ----- Initialize DevIL -----
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ILenum devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		printf("Devil Error: %s \n", iluErrorString(devilError));
	}
	else
	{
		printf("Devil intialized successfully. Using Devil version:  %d \n", iluGetInteger(ILU_VERSION_NUM));
	}
	glutTimerFunc(0, timer, 0);




	fprintf(stdout, "Arrow keys control speed.  Press \"r\" to run,  \"s\" to single step.\n");



	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop();

	return(0);   // This line is never reached.
}
bool check = false;
void timer(int) {

	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
	
	for (int i = 0; i < 19; i += 2){
	
	     if(!check){
			 if (pointVE[i+1] < 5)
				 pointVE[i + 1]  += 0.15;
		
		   else {
			   check = true;
			   
		   }

		 }
		 else {
			 if (pointVE[i + 1] > -4)
				 pointVE[i + 1] -= 0.15;
			 else
				 check = false;
		 }
	}
	

}

GLint loadTexture(char const* filename)
{
	ILuint imageID;				// Create an image ID as a ULuint

	GLuint texID;			// Create a texture ID as a GLuint

	ILboolean success = false;			// Create a flag to keep track of success/failure

	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID

	ilBindImage(imageID); 			// Bind the image

	std::string fn = "media/";
	fn.append(filename);

	success = ilLoadImage((ILstring)fn.c_str()); 	// Load the image file

													// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			printf("Image conversion failed - IL reports error: %d - %s\n", error, iluErrorString(error));
			getchar();
			exit(-1);
		}

		//Start writing to the framebuffer/texture in gpu
		// Generate a new texture
		glGenTextures(1, &texID);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, texID);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
			0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself

		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters to filter the texture in the near and far and show in a repeat style 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		printf("Image load failed - IL reports error: %d - %s\n", error, iluErrorString(error));
		getchar();
		exit(-1);
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	glBindTexture(GL_TEXTURE_2D, 0);
	printf("Object's texture (%s) is loaded successfully as ID (%i)...\n", filename, texID);

	return texID;
}

void myKeyboardFunc(unsigned char key, int x, int y);
void mySpecialKeyFunc(int key, int x, int y);
void initRendering();
void resizeWindow(int w, int h);

