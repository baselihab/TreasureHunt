#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>

int WIDTH = 1280;
int HEIGHT = 720;
void LoadAssets(void);
void drawFirstRoom(void);
void drawLastRoom(void);
GLuint tex;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(20, 5, 20);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_comodino;
Model_3DS model_bed;
Model_3DS model_key;
Model_3DS model_wardrobe;
Model_3DS model_table;
Model_3DS model_carpet;
Model_3DS model_meat;
Model_3DS model_dog;
Model_3DS model_treasure;
Model_3DS model_statue;


// Textures
GLTexture tex_ground;
GLTexture sky;
//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}
float rotAng;
int movex = 0;
int movez = -20;




int mousex = 0;
int mousey = 0;


void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-0.5, 0.5, -0.5, 0.5, -1, 1);
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(movex, 0, movez, mousex, mousey, movez+250, 0.0, 1.0, 0.0);
}

void drawRoom() {
	glPushMatrix();
	glColor3d(0, 0, 0);

	glBegin(GL_POLYGON);
	glVertex3d(-250, -250, 250);
	glVertex3d(-250, 250, 250);
	glVertex3d(250, 250, 250);
	glVertex3d(250, 0, 250);
	glVertex3d(100, 0, 250);
	glVertex3d(100, -250, 250);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3d(0, 0, 0);
	glVertex3d(-250, 250, 250);

	glVertex3d(-250, -250, 250);

	glVertex3d(-250, -250, -250);
	glVertex3d(-250, 250, -250);
	glEnd();
	glPopMatrix();

	glBegin(GL_QUADS);
	glColor3d(1, 1, 0);
	glVertex3d(250, 250, 250);
	glVertex3d(250, -250, 250);
	glVertex3d(250, -250, -250);
	glVertex3d(250, 250, -250);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(1, 0, 1);
	glVertex3d(250, -250, -250);
	glVertex3d(250, -250, 250);
	glVertex3d(-250, -250, 250);
	glVertex3d(-250, -250, -250);

	glEnd();

	glPushMatrix();
	glColor3d(0, 0.5, 1);
	glBegin(GL_QUADS);
	glColor3d(1, 1, 0);
	glVertex3d(250, 250, -250);
	glVertex3d(250, 250, 250);
	glVertex3d(-250, 250, 250);
	glVertex3d(-250, 250, -250);
	glEnd();
	glPopMatrix();

	glPopMatrix();
}




bool detect() {
	switch (movez) {
	case 495:
	case -495:return true;
	}
	return false;


}
void Anim() {

}
void Display() {
		setupCamera();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		glPushMatrix();
		drawFirstRoom();
		glPopMatrix();
		
		
		glPushMatrix();
		
		drawLastRoom();
		glPopMatrix();
		
		glPopMatrix();
	glFlush();
}
void drawFirstRoom() {
	glPushMatrix();

	glPushMatrix();
	glScaled(0.1, 0.1, 0.1);
	drawRoom();
	glPopMatrix();
	
	
	//BED 
	glPushMatrix();
	glScaled(0.03, 0.03, 0.03);
	glTranslated(-600, -700, 0);
	glRotated(180, 0, 1, 0);
	model_bed.Draw();
	glPopMatrix();
	//Carpet
	glPushMatrix();
	glScaled(0.1, 0.1, 0.1);
	glTranslated(0, -200, 0);
	glRotated(180, 0, 1, 0);
	model_carpet.Draw();
	glPopMatrix();

	//key 
	glPushMatrix();
	glScaled(0.1, 0.1, 0.1);
	glTranslated(0, -199, 0);
	model_key.Draw();
	glPopMatrix();


	//Table 
	glPushMatrix();
	
	glScaled(0.1, 0.1, 0.1);
	glTranslated(200,-230,0);
	model_table.Draw();
	glPopMatrix();
	//meat
	glPushMatrix();
	glColor3d(1, 0, 0);
	glScaled(0.01, 0.01, 0.01);
	glTranslated(1900,-2000, -200);
	model_meat.Draw();
	glPopMatrix();
	//comodino
	glPushMatrix();
	glScaled(0.03, 0.03, 0.03);
	glTranslated(-300, -800, 600);
	glRotated(90, 0, 1, 0);
	model_comodino.Draw();
	glPopMatrix();
	glPopMatrix();
}
void drawLastRoom() {
	glPushMatrix();
	glTranslated(0,0,500);
	glPushMatrix();
	glScaled(0.1, 0.1, 0.1);
	drawRoom();
	glPopMatrix();
	
	//Carpet
	glPushMatrix();
	glColor3d(1, 0, 0);
	glScaled(0.1, 0.1, 0.1);
	glTranslated(200, -200, 0);
	glRotated(180, 0, 1, 0);
	model_carpet.Draw();
	glPopMatrix();
	glPopMatrix();

	//key 
	glPushMatrix();
	glScaled(0.1, 0.1, 0.1);
	glTranslated(200, -199, 0);
	model_key.Draw();
	glPopMatrix();

	//dog
	glPushMatrix();
	glScaled(0.1, 0.1, 0.1);
	glTranslated(-200, -230, 0);
	glRotated(180, 0, 1, 0);
	model_dog.Draw();
	glPopMatrix();

	//treasure 
	glPushMatrix();
	glScaled(1,1,1);
	glTranslated(-10,-24,20);
	glRotated(100, 0, 1, 0);
	model_treasure.Draw();
	glPopMatrix();

	//statue
	glPushMatrix();
	glScaled(0.06, 0.06, 0.06);
	glTranslated(-300,-400, 0);
	model_statue.Draw();
	glPopMatrix();


}
void Passivemouse(int x, int y) {
	mousex = 500 - x;
	mousey = 350 - y;
	glutPostRedisplay();
}
void changeMotion(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:if (!(movex >= 245 && movex <= 250))movex += 1; break;
	case GLUT_KEY_RIGHT:if (!(movex <= -245 && movex >= -250)) movex -= 1; break;
	case GLUT_KEY_UP:if (!(movez >= 245 && movez <= 250))movez += 1; break;
	case GLUT_KEY_DOWN:if (!(movex <= -245 && movex >= -250))movez -= 1; break;

	}

	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(1000, 700);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Lab 5");
	LoadAssets();
	glutDisplayFunc(Display);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glutSpecialFunc(changeMotion);
	//glutKeyboardFunc(Keypress);
	glutPassiveMotionFunc(Passivemouse);
	glutIdleFunc(Anim);
	glShadeModel(GL_SMOOTH);

	glutMainLoop();
	return 0;
}


//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
//=======================================================================
//LoadAssets
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3ds");
	model_tree.Load("Models/tree/Tree1.3ds");
	model_comodino.Load("Models/bird/Nightstand.3DS");
	model_bed.Load("Models/beds/Bed N210416.3DS");
	model_key.Load("Models/keys/Key 2 N080710.3DS");
	model_wardrobe.Load("Models/wardrobe/Wardrobe N240916.3DS");
	model_table.Load("Models/tables/Table.3DS");
	model_carpet.Load("Models/carpets/Carpet N220916.3DS");
	model_meat.Load("Models/meat/Roll 7.3DS");
	model_dog.Load("Models/dogs/Dog N190213.3DS");
	model_treasure.Load("Models/treasure/Chest with money N180411.3DS");
	model_statue.Load("Models/temsal/Figurine N260715.3DS");
	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	loadBMP(&tex, "Textures/sky4-jpg.bmp", true);
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = {0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	// Draw Ground
	RenderGround();

	// Draw Tree Model
	glPushMatrix();
	glTranslatef(10, 0, 0);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	// Draw house Model
	glPushMatrix();
	glScaled(0.01,0.01,0.01);
	glRotatef(90.f, 1, 0, 0);
	model_bed.Draw();
	glPopMatrix();
	

//sky box
	glPushMatrix();

	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	glTranslated(50,0,0);
	glRotated(90,1,0,1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj,true);
	gluQuadricNormals(qobj,GL_SMOOTH);
	gluSphere(qobj,100,100,100);
	gluDeleteQuadric(qobj);
	
	glPopMatrix();
	
	
	
	glutSwapBuffers();
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'r':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================


//=======================================================================
// Main Function
//=======================================================================
