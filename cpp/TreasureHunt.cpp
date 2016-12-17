#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <iostream>

//C:\Users\loay elzobeidy\Downloads\OpenGLMeshLoader_22362\OpenGLMeshLoader\models
int WIDTH = 1280;
int HEIGHT = 720;
float rotAng;
int movex = 0;
int movez = -20;
int mousex = 0;
int mousey = 0;
int timer = 0;
boolean first_meat = false;
boolean first_key = false;
boolean found = false;
boolean eat = false;
boolean start = true;
void LoadAssets(void);
void drawFirstRoom(void);
void drawLastRoom(void);
void key(unsigned char key, int x, int y);
void RenderGround();
void RenderCeiling();
GLuint tex;
char title[] = "3D Model Loader Sample";

int cameraZoom = 0;

// Model Variables
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
Model_3DS model_door;


// Textures
GLTexture tex_ground;
GLTexture tex_ceiling;

void time(int val) {
	timer++;
	glutPostRedisplay;
	glutTimerFunc(1000, time, 0);
}

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
	gluLookAt(movex, -10, movez, mousex, mousey, movez+250, 0.0, 1.0, 0.0);
	//std::cout << "movx"<<movex<<"movz"<<movez;
}

void drawRoom() {


	//Front Wall
	glPushMatrix();
	glColor3d(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3d(-250, -250, 250);
	glVertex3d(-250, 250, 250);
	glVertex3d(250, 250, 250);
	glVertex3d(250, -250, 250);
	glEnd();
	glPopMatrix();
	
	//Ground
	glPushMatrix();
	RenderGround();
	glPopMatrix();
	
	//Right Wall
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3d(1, 0, 1);
	glVertex3d(-250, 250, 250);
	glVertex3d(-250, -250, 250);
	glVertex3d(-250, -250, -250);
	glVertex3d(-250, 250, -250);
	glEnd();
	glPopMatrix();

	//Left wall
	glBegin(GL_QUADS);
	glColor3d(1, 1, 1);
	glVertex3d(250, 250, 250);
	glVertex3d(250, -250, 250);
	glVertex3d(250, -250, -250);
	glVertex3d(250, 250, -250);
	glEnd();

	

	////Ground wall
	//glPushMatrix();
	//glBegin(GL_QUADS);
	//glColor3d(0, 1, 1);
	//glVertex3d(250, -250, -250);
	//glVertex3d(250, -250, 250);
	//glVertex3d(-250, -250, 250);
	//glVertex3d(-250, -250, -250);
	//glEnd();
	//glPopMatrix();

	//Up
	glPushMatrix();
	/*glBegin(GL_QUADS);
	glColor3d(1, 1, 1);
	glVertex3d(250, 250, -250);
	glVertex3d(250, 250, 250);
	glVertex3d(-250, 250, 250);
	glVertex3d(-250, 250, -250);
	glEnd();*/
	RenderCeiling();
	glPopMatrix();

	glPopMatrix();
}

void restart() {
	movex = 0;
	movez = -20;
	mousex = 0;
	mousey = 0;
	timer = 0;
	first_key = false;
	first_meat = false;
	found = false;
	eat = false;
	glutPostRedisplay();
}

void Anim() {

}
	
void Display() {
	if (start) {
		std::cout << "Your goal is to pick up the treasure hiden somewhere in the house in less than 2 minutes!" << "\n";
		std::cout << "You can pick up objects by pressing 'f'" << "\n";
		start = false;
	}
	if (timer >= 120 && !found) {
		std::cout << "2 minutes have passed, try again!"<< "\n";
		restart();
		timer = 0;
	}
	setupCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//RenderGround();
	glPushMatrix();
	glPushMatrix();
	drawFirstRoom();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0,50);
	drawLastRoom();
	glPopMatrix();

	
	glFlush();
}

void drawFirstRoom() {
	glPushMatrix();

	glPushMatrix();
	glScaled(0.1, 0.1, 0.1);
	drawRoom();
	glPopMatrix();
	
	//Door
	glPushMatrix();
	glScaled(0.02, 0.02, 0.02);
	glTranslated(110, -1300, 1200);
	glRotated(90, 0, 1, 0);
	model_door.Draw();
	
	glPopMatrix();
	
	//BED 
	glPushMatrix();
	glScaled(0.03, 0.03, 0.03);
	glTranslated(-600, -830, 0);
	glRotated(180, 0, 1, 0);
	model_bed.Draw();
	glPopMatrix();
	//Carpet
	glPushMatrix();
	glScaled(0.1, 0.1, 0.1);
	glTranslated(0, -250, 0);
	glRotated(180, 0, 1, 0);
	model_carpet.Draw();
	glPopMatrix();
	
	
	//key 
	if (!first_key) {
		glColor3d(1, 0, 0);
		glPushMatrix();
		glScaled(0.1, 0.1, 0.1);
		glTranslated(0, -249, 0);
		model_key.Draw();
		glPopMatrix();
	}

	
	//Table 
	glColor3d(1, 1, 1);
	glPushMatrix();
	glScaled(0.1, 0.1, 0.1);
	glTranslated(200,-250,0);
	model_table.Draw();
	glPopMatrix();
	//meat
	if (!first_meat) {
		glPushMatrix();
		glColor3d(1, 0, 0);
		glScaled(0.01, 0.01, 0.01);
		glTranslated(1900, -2200, -200);
		model_meat.Draw();
		glPopMatrix();
	}
	//comodino
	glPushMatrix();
	glColor3d(1, 1, 1);
	glScaled(0.03, 0.03, 0.03);
	glTranslated(-300, -800, 600);
	glRotated(90, 0, 1, 0);
	model_comodino.Draw();
	glPopMatrix();
	glPopMatrix();
}

void drawLastRoom() {
	glPushMatrix();
	glPushMatrix();
	glTranslated(0, 0, 0);
	glPushMatrix();
	glScaled(0.1, 0.1, 0.1);
	drawRoom();
	glPopMatrix();

	//Wardrobe
	glPushMatrix();
	//glColor3d(1, 0, 0);
	glScaled(0.1, 0.1, 0.1);
	glTranslated(200, -250, 0);
	glRotated(270, 0, 1, 0);
	model_wardrobe.Draw();
	glPopMatrix();

	////key 
	//glPushMatrix();
	//glScaled(0.1, 0.1, 0.1);
	//glTranslated(200, -199, 0);
	//model_key.Draw();
	//glPopMatrix();

	//dog
	if (!eat) {
		glPushMatrix();
		glScaled(0.1, 0.1, 0.1);
		glTranslated(-100, -230, 0);
		glRotated(180, 0, 1, 0);
		model_dog.Draw();
		glPopMatrix();
	}
	//treasure 
	if (!found) {
	glPushMatrix();
	glScaled(0.0005, 0.0005, 0.0005);
	glTranslated(-10, -43000, 30000);
	glRotated(0, 0, 1, 0);
	model_treasure.Draw();
	glPopMatrix();
}

	//statue
	glPushMatrix();
	glScaled(0.06, 0.06, 0.06);
	glTranslated(-300,-400, 0);
	model_statue.Draw();
	glPopMatrix();

	glPopMatrix();
}

void Passivemouse(int x, int y) {
	mousex = 500 - x;
	mousey = 350 - y;
	glutPostRedisplay();
}

void key(unsigned char key, int x, int y) {
	switch (key) {
	case 'f':
		if ((movex<3&&movex>-3) && (movez<-5 && movez>-8)) {
			std::cout << "Now you have a key!"<< "\n";
			first_key = true;
		}
		else if ((movex<23 && movex>15) && (movez<-5 && movez>-8)) {
			std::cout << "Now you've some meat!"<< "\n";
			first_meat = true;
		}
		else if ((movex <-8 && movex>-15) && (movez < 44 && movez>40)) {
			std::cout << "Cograts, the dog isn't hungry anymoe! Go grab your treasure!" << "\n";
			eat = true;
		}
		else if ((movex <8 && movex>-9) && (movez < 63 && movez>50)) {
			std::cout << "YOU WOON!!" << "\n";
			found = true;
		}
		break;
	case 'd':
		std::cout << "movx" << movex << "movz" << movez;
	//case'27':
	//	std::cout << "Byee!" << "\n";
	//	//glutDestroyWindow(Win.id);
	//	exit(0);
	//	break;
	case 'r':
		restart();
		break;
	default: break;
	}
}
	
void changeMotion(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:if (!(movex >= 245 && movex <= 250)&& movex<24)movex += 1; break;
	case GLUT_KEY_RIGHT:if (!(movex <= -245 && movex >= -250)&&movex>-24) movex -= 1; break;
	case GLUT_KEY_UP:
		if (!(movez >= 245 && movez <= 250) && movez < 74) {
			if (movez <= 23) {
				//first room
				//std::cout << "first room";
				movez += 1;
			}
			else if (movez > 23 && !first_key) {
				// no key
				std::cout << "Try searching for a key!" << "\n";
			}
			else if (movez > 23 && movez < 25&& first_key && (movex<0||movex>4)) {
				//key but not door
				std::cout << "Try entering through the door" << "\n";
			}
			else if (movez > 42 && first_key && !first_meat && !eat) {
				//key and door but no meat
				std::cout << "You found a hungry dog that ate you, you die!!" << "\n";
				restart();
			}
			else if (movez > 42 && first_key && first_meat && !eat) {
				//key and door and meat but no eat
				std::cout << "Opps! You foud food but you didn't feed the dog!" << "\n";
				restart();
			}
			else {
				//Second room
				//std::cout << "You're in";
				movez += 1;
			}
			break;
		}
			
			
	case GLUT_KEY_DOWN:if (!(movex <= -245 && movex >= -250)&& movez>-24)movez -= 1; break;

	}

	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(1000, 700);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Treasure Hunt");
	LoadAssets();
	glutDisplayFunc(Display);
	glutTimerFunc(0, time, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glutSpecialFunc(changeMotion);
	glutKeyboardFunc(key);
	glutPassiveMotionFunc(Passivemouse);
	glutIdleFunc(Anim);
	glShadeModel(GL_SMOOTH);
	

	glutMainLoop();
	return 0;
}

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
	glVertex3d(250, -250, -250);
	glTexCoord2f(5, 0);
	glVertex3d(250, -250, 250);
	glTexCoord2f(5, 5);
	glVertex3d(-250, -250, 250);
	glTexCoord2f(0, 5);
	glVertex3d(-250, -250, -250);
	
	
	
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void RenderCeiling()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ceiling.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3d(250, 250, -250);
	glTexCoord2f(5, 0);
	glVertex3d(250, 250, 250);
	glTexCoord2f(5, 5);
	glVertex3d(-250, 250, 250);
	glTexCoord2f(0, 5);
	glVertex3d(-250, 250, -250);



	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void LoadAssets()
{
	// Loading Model files
	model_comodino.Load("Models/bird/Nightstand.3DS");
	model_bed.Load("Models/beds/Bed N210416.3DS");
	model_key.Load("Models/keys/Key 2 N080710.3DS");
	model_wardrobe.Load("Models/wardrobe/Wardrobe N240916.3DS");
	model_table.Load("Models/tables/Table.3DS");
	model_carpet.Load("Models/carpets/Carpet N220916.3DS");
	model_meat.Load("Models/meat/Roll 7.3DS");
	model_dog.Load("Models/dogs/Dog N190213.3DS");
	model_treasure.Load("Models/treasure/t/treasure.3DS");
	model_statue.Load("Models/temsal/Figurine N260715.3DS");
	model_door.Load("Models/doors/door2/Door LONGHI N250216.3DS");
	// Loading texture files
	tex_ground.Load("textures/floor.bmp");
	tex_ceiling.Load("textures/Ceiling.bmp");
}