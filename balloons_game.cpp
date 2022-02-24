/*********

ADDITIONAL FEATURES: 1. Displays starting page with instructions
					 2. Pressing c/C, the color of the balloons are changed to a random color
					 3. Pressing b/B, the balloons get bigger (until its radius reach to a certain point)
					 4. Pressing s/S, the balloons get smaller (until its radius reach to a certain point)
					 5. Pressing F1, the clouds move to the right direction
					 6. Pressing F5, deletes the house(s)
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD   300 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define MAX 10

//defining the states of the program which has two different states
#define START 0
#define DESIGN 1
int state = START; //initialising the state

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

//creating a structure to deal with the shapes
typedef struct{
	float x, y;
}point_t;

point_t mouse; //creating a structure variable to deal with showing the mouse position inside on move function
point_t shapes[MAX]; //creating a structure variable to deal with creating the objects

int count = 0; //counting the shapes created
bool active = false; //for starting/stopping th animation

float r = 0.78, g = 0.08, b = 0.8; //initial values for colour of the ballons
float radius = 20, radius2 = 4; //initial values for the size of the ballons

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void designStart(){

	glColor3ub(195, 126, 186);
	glRectf(-winWidth / 2, winHeight / 2, winWidth / 2, -winHeight / 2);
	
	glColor3ub(102, 0, 51);
	glRectf(-400, 200, 400, -200);

	glColor3ub(195, 126, 186);
	vprint2(-350, 125, 0.28, "Screen Designed by Sumeyye Kurtulus");
	vprint2(-280, 50, 0.2, "Press ");
	vprint2(-200, 50, 0.2, "-> Space to see the view");
	vprint2(-200, 0, 0.2, "-> c/C to change the color of the ballons");
	vprint2(-200, -50, 0.2, "-> s/S to make the ballons smaller");
	vprint2(-200, -100, 0.2, "-> b/B to make the ballons bigger");
	vprint2(-200, -150, 0.2, "-> F5 to delete the house");

}

void drawBalloons(point_t shape){

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(shape.x - 30, shape.y + 100); //1
	glVertex2f(shape.x - 60, shape.y + 180);
	glVertex2f(shape.x - 30, shape.y + 100); //2
	glVertex2f(shape.x - 90, shape.y + 180);
	glVertex2f(shape.x - 30, shape.y + 100); //3
	glVertex2f(shape.x - 30, shape.y + 180);
	glVertex2f(shape.x - 30, shape.y + 100); //4
	glVertex2f(shape.x, shape.y + 180);
	glVertex2f(shape.x - 30, shape.y + 100); //5
	glVertex2f(shape.x + 30, shape.y + 180);
	glEnd();

	glColor4f(r, g, b, 0.8);
	circle(shape.x - 60, shape.y + 180, radius); //1
	circle(shape.x - 90, shape.y + 180, radius); //2
	circle(shape.x - 30, shape.y + 180, radius); //3
	circle(shape.x, shape.y + 180, radius); //4
	circle(shape.x + 30, shape.y + 180, radius); //5

	glColor4f(1, 1, 1, 0.8);
	circle(shape.x - 51, shape.y + 190, radius2); //1
	circle(shape.x - 81, shape.y + 190, radius2); //2
	circle(shape.x - 21, shape.y + 190, radius2); //3
	circle(shape.x + 9, shape.y + 190, radius2); //4
	circle(shape.x + 39, shape.y + 190, radius2); //5
}

void drawHouse(point_t shape){

	glColor3ub(222, 184, 135);
	glBegin(GL_POLYGON);
	glVertex2f(shape.x, shape.y + 80);
	glVertex2f(shape.x - 50, shape.y + 30);
	glVertex2f(shape.x - 30, shape.y + 30);
	glVertex2f(shape.x - 30, shape.y - 40);
	glVertex2f(shape.x + 30, shape.y - 40);
	glVertex2f(shape.x + 30, shape.y + 30);
	glVertex2f(shape.x + 50, shape.y + 30);
	glEnd();

	glColor3f(0, 0, 0);
	glRectf(shape.x - 10, shape.y + 90, shape.x - 50, shape.y + 100);

	glColor3ub(222, 184, 135);
	glRectf(shape.x - 80, shape.y - 40, shape.x + 80, shape.y - 50);
	glRectf(shape.x - 70, shape.y, shape.x + 70, shape.y - 40);

	glColor3ub(178, 34, 34);
	glBegin(GL_QUADS);
	glVertex2f(shape.x - 20, shape.y + 60);
	glVertex2f(shape.x - 40, shape.y + 40);
	glVertex2f(shape.x - 40, shape.y + 90);
	glVertex2f(shape.x - 20, shape.y + 90);
	glEnd();

	glColor3ub(222, 184, 135);
	glBegin(GL_POLYGON);
	glVertex2f(shape.x - 110, shape.y);
	glVertex2f(shape.x + 110, shape.y);
	glVertex2f(shape.x + 60, shape.y + 60);
	glVertex2f(shape.x - 60, shape.y + 60);
	glEnd();

	drawBalloons(shape);


	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(shape.x, shape.y + 80);
	glVertex2f(shape.x - 50, shape.y + 30);
	glVertex2f(shape.x - 30, shape.y + 30);
	glVertex2f(shape.x - 30, shape.y - 40);
	glVertex2f(shape.x + 30, shape.y - 40);
	glVertex2f(shape.x + 30, shape.y + 30);
	glVertex2f(shape.x + 50, shape.y + 30);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(shape.x - 80, shape.y - 40);
	glVertex2f(shape.x - 80, shape.y - 50);
	glVertex2f(shape.x + 80, shape.y - 50);
	glVertex2f(shape.x + 80, shape.y - 40);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(shape.x - 60, shape.y + 60);
	glVertex2f(shape.x - 110, shape.y);
	
	glVertex2f(shape.x + 60, shape.y + 60);
	glVertex2f(shape.x + 110, shape.y);

	glVertex2f(shape.x - 60, shape.y + 60);
	glVertex2f(shape.x - 18, shape.y + 60);

	glVertex2f(shape.x + 60, shape.y + 60);
	glVertex2f(shape.x + 18, shape.y + 60);

	glVertex2f(shape.x - 110, shape.y);
	glVertex2f(shape.x - 30, shape.y);

	glVertex2f(shape.x + 110, shape.y);
	glVertex2f(shape.x + 30, shape.y);

	glVertex2f(shape.x - 70, shape.y);
	glVertex2f(shape.x - 70, shape.y - 40);

	glVertex2f(shape.x + 70, shape.y);
	glVertex2f(shape.x + 70, shape.y - 40);

	glVertex2f(shape.x, shape.y - 40);
	glVertex2f(shape.x, shape.y - 10);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(shape.x - 15, shape.y - 40);
	glVertex2f(shape.x + 15, shape.y - 40);
	glVertex2f(shape.x + 15, shape.y - 10);
	glVertex2f(shape.x - 15, shape.y - 10);
	glEnd();

	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex2f(shape.x - 10, shape.y - 30);
	glVertex2f(shape.x + 9, shape.y - 30);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(shape.x - 25, shape.y + 30);
	glVertex2f(shape.x - 25, shape.y );
	glVertex2f(shape.x - 5 , shape.y );
	glVertex2f(shape.x - 5, shape.y + 30);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(shape.x - 15, shape.y + 30);
	glVertex2f(shape.x - 15, shape.y);
	
	glVertex2f(shape.x + 15, shape.y + 30);
	glVertex2f(shape.x + 15, shape.y);

	glVertex2f(shape.x - 25, shape.y + 15);
	glVertex2f(shape.x - 5, shape.y + 15);

	glVertex2f(shape.x + 25, shape.y + 15);
	glVertex2f(shape.x + 5, shape.y + 15);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(shape.x + 25, shape.y + 30);
	glVertex2f(shape.x + 25, shape.y);
	glVertex2f(shape.x + 5, shape.y);
	glVertex2f(shape.x + 5, shape.y + 30);
	glEnd();


}

//declearing and initialising the values for the clouds
point_t clouds{ -350, 300 };
float R1 = 30, R2 = 20;

void displayClouds(){

	glColor3f(1, 1, 1);

	circle(clouds.x, clouds.y, R1);
	circle(clouds.x - 30, clouds.y, R2);
	circle(clouds.x + 30, clouds.y, R2);

	circle(clouds.x - 120, clouds.y - 20, R1);
	circle(clouds.x - 150, clouds.y - 20, R2);
	circle(clouds.x - 90, clouds.y - 20, R2);

	circle(clouds.x + 120, clouds.y - 20, R1);
	circle(clouds.x + 150, clouds.y - 20, R2);
	circle(clouds.x + 90, clouds.y - 20, R2);

	circle(clouds.x + 240, clouds.y, R1);
	circle(clouds.x + 270, clouds.y, R2);
	circle(clouds.x + 210, clouds.y, R2);

	circle(clouds.x + 360, clouds.y - 20, R1);
	circle(clouds.x + 390, clouds.y - 20, R2);
	circle(clouds.x + 330, clouds.y - 20, R2);

	circle(clouds.x + 480, clouds.y, R1);
	circle(clouds.x + 510, clouds.y, R2);
	circle(clouds.x + 450, clouds.y, R2);

	circle(clouds.x + 600, clouds.y - 20, R1);
	circle(clouds.x + 630, clouds.y - 20, R2);
	circle(clouds.x + 570, clouds.y - 20, R2);

	circle(clouds.x + 720, clouds.y, R1);
	circle(clouds.x + 750, clouds.y, R2);
	circle(clouds.x + 690, clouds.y, R2);

	circle(clouds.x + 840, clouds.y - 20, R1);
	circle(clouds.x + 870, clouds.y - 20, R2);
	circle(clouds.x + 810, clouds.y - 20, R2);

}

//declearing and initialising the values for the grass
point_t grass = { -707, -400 };

void displayGrass(){

	glColor3ub(63, 102, 63);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i <= 1360 ; i += 30){
	
		glVertex2f(grass.x + i, grass.y);
		glVertex2f(grass.x + i + 30, grass.y + 60);
		glVertex2f(grass.x + i + 60, grass.y);

	}
	glEnd();



}

void drawAllShapes(){
	for (int i = 0; i< count; i++) {
	    drawHouse(shapes[i]);
		glColor3f(1, 1, 1);
		vprint(shapes[i].x - 37, shapes[i].y + 80, GLUT_BITMAP_8_BY_13, "%d", i + 1);
	}
}


//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3ub(153, 204, 255);
	glRectf(-winWidth / 2, winHeight / 2, winWidth / 2, -winHeight / 2);


	switch (state){
	
	case START: designStart(); break;
	case DESIGN: 
		displayClouds();
		displayGrass();
		drawAllShapes();

		glColor3ub(149, 107, 158);
		glRectf(550, 380, 670, 310);

		glColor3ub(116, 68, 125);
		glRectf(560, 370, 660, 320);

		glColor3ub(125, 188, 232);
		vprint(580, 355, GLUT_BITMAP_8_BY_13, "Sumeyye");
		vprint(575, 340, GLUT_BITMAP_8_BY_13, "Kurtulus");
		vprint(575, 325, GLUT_BITMAP_8_BY_13, "21902787");

		glColor3f(0, 0, 0);
		vprint(-700, 375, GLUT_BITMAP_8_BY_13, "(%.0f, %.0f)", mouse.x, mouse.y);
	}

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	if (key == 67 || key == 99) // c/C
	{
		r = (rand() % 256) / 255.0;
		g = (rand() % 256) / 255.0;
		b = (rand() % 256) / 255.0;
	}

	if (key == 66 || key == 98) // b/B
	{
		if (radius <= 30)
			radius += 2;
		if (radius2 <= 9)
			radius2 += 1;
		
	}

	if (key == 83 || key == 115) // s/S
	{
		if (radius > 20)
			radius -= 2;
		if (radius2 > 4)
			radius2 -= 1;

	}

	
	if (key == ' ' && state == START)
		state = DESIGN;


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	if (key == GLUT_KEY_F1)
		active = !active;

	if (key == GLUT_KEY_F5 && count >= 1)
		count--;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//



void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.


	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && count < MAX && state == DESIGN){
		
		shapes[count] = { x - winWidth / 2, winHeight / 2 - y };

		if (count == 0){
			count = 1;
		}
		else if (count >= 1){
			if (shapes[count - 1].x == shapes[count].x
				&& shapes[count - 1].y == shapes[count].y)
				return; //preventing user to create another house at the same location
			else
				count++;
		}
		

	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}


//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {


	// Write your codes here.
	mouse.x = x - winWidth / 2;
	mouse.y = winHeight / 2 - y;


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	if (active){
	
		//moving house(s)
		for (int i = 0; i < count; i++){
			shapes[i].y += 10;

			if (shapes[i].y > winHeight / 2.0)
				shapes[i].y = -winHeight / 2.0;

		}

		//moving clouds
		clouds.x += 10;
		if (clouds.x > winWidth / 2.0)
			clouds.x = -winWidth / 2.0;


	}

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Homework1: Animation by Sumeyye Kurtulus");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();
	//displayCircle();
	glutMainLoop();
}