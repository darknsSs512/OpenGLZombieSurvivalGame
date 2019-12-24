#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <cmath>
#include <math.h>
#include "OpenGLMeshLoader19.h"
GLTexture tex_ground;
GLTexture redGround;
GLTexture rockGround;

GLTexture zombieFace;
GLTexture zombieFace2;
GLTexture zombieArm;
GLTexture zombieLeg;
GLTexture zombieTorso;

GLTexture kingFace;
GLTexture kingFace2;
GLTexture kingArm;
GLTexture kingLeg;
GLTexture kingTorso;

GLTexture humanFace;
GLTexture humanArm;
GLTexture humanLeg;
GLTexture humanTorso;

GLTexture treeTrunk;
GLTexture redTree;

GLTexture treeLeafe;
GLTexture redLeafe;

GLTexture rock;
GLTexture hill;

GLTexture scroll;

GLTexture stick;
GLTexture swordHandle;
GLTexture swordBlade;
GLTexture warhammerHandle;
GLTexture warhammerHead;

//GLTexture warhammer;
bool onTree = false;
GLTexture stickIcon;
GLTexture swordIcon;
GLTexture warhammerIcon;

GLTexture fist;
short deathPanel= 0;
bool died = false;
bool stickOn=false;
bool swordOn = false;
bool warhammerOn = false;
bool winState = false;
double sunX = 48, sunY = 32, sunZ = 0, sunSpeed = 1;

enum weapon {myStick,mySword,myWarhammer,myFist};
double stickX, stickZ;
double swordX, swordZ;
double warhammerX, warhammerZ;

weapon equipedWeapon=myFist;
int WIDTH = 1280;
int HEIGHT = 720;

bool secondView = 0;

bool humanReaction = false;//triggered when zombie hit you
short humanReactionCounter = 0;
short humanReactionCounterLimit = 20; // how far the human is knocked back
bool humanDeathReaction = false;
short humanDeathReactionCounter = 0;


float scrollAng = 0,scrollrotSpeed=0.4; // ang of the arms and legs of human and zombie
float stickAng = 0; // ang of the arms and legs of human and zombie
double scrollX[3]{ 41,46,42};
double scrollZ[3]{ -153,-2,100};
bool scrollTaken[3];

float ang = 0, limbAng = 0, limbAngZ = 0; // ang of the arms and legs of human and zombie

float angInc = 2, angIncZ = 0.5; // how fast the human and zombie arms and legs move
bool keyarr[200]; // to allow multiple key listening (stores 1 if the Unicode of the key is pressed) 
double speed = 0.5;// spagity
double playerX = 42, playerY = 7, playerZ = 122;
int PointBehindHuman = 30 / speed; // decrease this when u increase speed

const int zombieN = 19; // number of zombies
short remaining = 7;
const int zombieHP = 2; // health of zombie
byte aggro = 55;
//							Array for each attribute of the Zombie (X,Z) displacement, knocked back , dying animation
double zombieX[zombieN]{20,83,-5,42,93,19,67};
double zombieZ[zombieN]{12,12,-164,-164,-164,-125,-125};
double zombieXlvl1[7]{ 20,83,-5,42,93,19,67 };
double zombieZlvl1[7]{ 12,12,-164,-164,-164,-125,-125 };

double zombieXlvl2[19]{ 42,-4,42,91,-4,42,91 ,-4,42,91 ,-4,42,91 ,-4,42,91 ,-4,42,91 };
double zombieZlvl2[19]{-155,-125 ,- 125,-125,-95,-95,-95,-65,-65,-65,-35,-35,-35,-5,-5,-5,25,25,25 };


bool zombieHitReact[zombieN];

short zombieHitReactCounter[zombieN];
bool zombieDeathReact[zombieN];
short zombieDeathReactCounter[zombieN];

bool zombieDead[zombieN];// 1 if zombie 'i' is dead
short zombieHealth[zombieN];//how many times a zombie was hit


double zHitterIndex;

GLuint tex;
GLuint texFace;
GLuint sunTex;


char title[] = "Full of Zombies";
bool moving = false;//human is moving

bool hittingReaction = false;//
short hittingReactionCounter = 0;
double hittingAng = 0;
short range = 0;
short damage = 0;
double maxHP = 0.32, currHP = 0.32;

short scrollN = 0;
// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.001;
GLdouble zFar = 1000;
double i = 0;
bool firstLevel = true;

bool disableHitting = 0;

double Dist(double x1, double z1, double x2, double z2) {
	return sqrt(((x1 - x2) * (x1 - x2))
		+ ((z1 - z2) * (z1 - z2)));
}


double dist(int i) {
	return sqrt(((playerX - zombieX[i]) * (playerX - zombieX[i]))
		+ ((playerZ - zombieZ[i]) * (playerZ - zombieZ[i])));
}

double weaponDist(double x, double z) {
	return sqrt(((playerX - x) * (playerX - x))
		+ ((playerZ - z) * (playerZ - z)));
}

double scrollDist(int i) {
	return sqrt(((playerX - scrollX[i]) * (playerX - scrollX[i]))
		+ ((playerZ - scrollZ[i]) * (playerZ - scrollZ[i])));
}

double angInDegrees(double ang) {// radian to deg
	return -ang * (180.0 / 3.141592653589793238463);
}
void drawBox(GLfloat size, GLenum type, short limb)

{

	static GLfloat n[6][3] =
	{
	  {-1.0, 0.0, 0.0},
	  {0.0, 1.0, 0.0},
	  {1.0, 0.0, 0.0},
	  {0.0, -1.0, 0.0},
	  {0.0, 0.0, 1.0},
	  {0.0, 0.0, -1.0}
	};
	static GLint faces[6][4] =
	{
	  {0, 1, 2, 3},
	  {3, 2, 6, 7},
	  {7, 6, 5, 4},
	  {4, 5, 1, 0},
	  {5, 6, 2, 1},
	  {7, 4, 0, 3}
	};
	GLfloat v[8][3];
	GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

	for (i = 5; i >= 0; i--) {
		glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
		switch (limb)
		{
		case 0:	glBindTexture(GL_TEXTURE_2D, zombieFace.texture[0]);
			break;
		case 1:	glBindTexture(GL_TEXTURE_2D, zombieArm.texture[0]);
			break;
		case 2:	glBindTexture(GL_TEXTURE_2D, zombieLeg.texture[0]);
			break;
		case 3:	glBindTexture(GL_TEXTURE_2D, zombieTorso.texture[0]);
			break;
		case 4:	glBindTexture(GL_TEXTURE_2D, kingFace.texture[0]);
			break;
		case 5:	glBindTexture(GL_TEXTURE_2D, kingArm.texture[0]);
			break;
		case 6:	glBindTexture(GL_TEXTURE_2D, kingLeg.texture[0]);
			break;
		case 7:	glBindTexture(GL_TEXTURE_2D, kingTorso.texture[0]);
			break;
		case 8:	glBindTexture(GL_TEXTURE_2D, zombieFace2.texture[0]);
			break;
		case 9:	glBindTexture(GL_TEXTURE_2D, kingFace2.texture[0]);
			break;

		case '0':	glBindTexture(GL_TEXTURE_2D, humanFace.texture[0]);
			break;
		case '1':	glBindTexture(GL_TEXTURE_2D, humanArm.texture[0]);
			break;
		case '2':	glBindTexture(GL_TEXTURE_2D, humanLeg.texture[0]);
			break;
		case '3':	glBindTexture(GL_TEXTURE_2D, humanTorso.texture[0]);
			break;
		case 't':	glBindTexture(GL_TEXTURE_2D,firstLevel? treeTrunk.texture[0]: redTree.texture[0]);
			break;
		case 'l':	glBindTexture(GL_TEXTURE_2D, firstLevel ? treeLeafe.texture[0]: redLeafe.texture[0]);
			break;
		case 's':	glBindTexture(GL_TEXTURE_2D, scroll.texture[0]);
			break;
		case 'c':	glBindTexture(GL_TEXTURE_2D, stick.texture[0]);
			break;
		case 'h':	glBindTexture(GL_TEXTURE_2D, swordHandle.texture[0]);
			break;
		case 'b':	glBindTexture(GL_TEXTURE_2D, swordBlade.texture[0]);
			break;
		case 'x':	glBindTexture(GL_TEXTURE_2D, warhammerHead.texture[0]);
			break;
		case 'y':	glBindTexture(GL_TEXTURE_2D, warhammerHandle.texture[0]);
			break;
		case 'z':	glBindTexture(GL_TEXTURE_2D, rock.texture[0]);
			break;
		case 'p':	glBindTexture(GL_TEXTURE_2D, hill.texture[0]);
			break;

		}
		glBegin(type);
		glNormal3fv(&n[i][0]);
		glTexCoord2f(0, 0);
		glVertex3fv(&v[faces[i][0]][0]);
		glTexCoord2f(1, 0);

		glVertex3fv(&v[faces[i][1]][0]);
		glTexCoord2f(1, 1);

		glVertex3fv(&v[faces[i][2]][0]);
		glTexCoord2f(0, 1);

		glVertex3fv(&v[faces[i][3]][0]);
		glEnd();
	}
}

void APIENTRY
glutSolidCube(GLdouble size, short limb)
{
	drawBox(size, GL_QUADS, limb);
}

void showStick(double x,double z) {

	glPushMatrix(); //stick
	glTranslatef(x, 3, z);
	glRotatef(stickAng, 0, 1, 0);
	glRotatef(30, 1, 0, 0);
	glScaled(0.1, 1, 0.1);
	glutSolidCube(3, 'c');
	glPopMatrix();

}

void showWarhammer(double x, double z) {
	glPushMatrix(); //warhammer
	glTranslatef(x, 0, z);
	glRotatef(stickAng, 0, 1, 0);
	glRotatef(30, 0, 1, 0);
	glTranslatef(-x, 0, -z);

	glPushMatrix();//head
	glTranslatef(x, 8, z);
	glScaled(1.5, 1, 1);
	glutSolidCube(2, 'x');
	glPopMatrix();

	glPushMatrix();//handle
	glTranslatef(x, 3.5, z);
	glScaled(0.05, 1, 0.05);
	glutSolidCube(7, 'h');
	glPopMatrix();

	glPopMatrix();


}

void drawWarhammer(double x, double z) {
	glPushMatrix(); //warhammer

	double y = 7;
	
	//dour stuff

	glTranslated(-3, 0, 0);

	glRotated(-180, 0, 1, 0);

	glRotated(90, 0, 0, 1);


	glPushMatrix();//head
	glTranslatef(0, 8, 0);
	glScaled(1.5, 1, 1);
	glutSolidCube(2, 'x');
	glPopMatrix();

	glPushMatrix();//handle
	glTranslatef(0, 3.5, 0);
	glScaled(0.05, 1, 0.05);
	glutSolidCube(7, 'h');
	glPopMatrix();
	glPopMatrix();


}
void drawStick(double x, double z) {

	double y = 7;
	glPushMatrix(); 


	//dour stuff

	glTranslated(-2,-2, 0);

	glRotated(-180, 0, 1, 0);

	glRotated(90, 0, 0, 1);

	glPushMatrix(); //stick
	glTranslatef(0, 3, 0);
	glScaled(0.1, 1, 0.1);
	glutSolidCube(3, 'c');
	glPopMatrix();
	glPopMatrix(); 

}


void showSword(double x,double z) {


	glPushMatrix(); //sword
	glTranslatef(x, 0, z);
	glRotatef(stickAng, 0, 1, 0);
	glRotatef(30, 0, 1, 0);
	glTranslatef(-x, 0, -z);

	glPushMatrix();//blade
	glTranslatef(x, 6, z);
	glScaled(0.2, 1, 0.01);
	glutSolidCube(6, 'b');
	glPopMatrix();

	glPushMatrix();//sword grib
	glTranslatef(x, 3, z);
	glScaled(1, 0.1, 0.1);
	glutSolidCube(3, 'h');
	glPopMatrix();

	glPushMatrix();//sword handle
	glTranslatef(x, 2.5, z);
	glScaled(0.2, 1, 0.2);
	glutSolidCube(1, 'h');
	glPopMatrix();

	glPopMatrix();

}



void drawSword(double x, double z) {


	double y=7;
	glPushMatrix(); //sword


	//dour stuff
	
	glTranslated(-6,0 , 0);
	
	glRotated(-180, 0, 1, 0);

	glRotated(90, 0, 0, 1);
	
	glPushMatrix();//blade
	glTranslatef(0, y+3, 0);
	glScaled(0.2, 1, 0.01);
	glutSolidCube(6, 'b');
	glPopMatrix();

	glPushMatrix();//sword grib
	glTranslatef(0, y, 0);
	glScaled(1, 0.1, 0.1);
	glutSolidCube(3, 'h');
	glPopMatrix();

	glPushMatrix();//sword handle
	glTranslatef(0, y-0.5, 0);
	glScaled(0.2, 1, 0.2);
	glutSolidCube(1, 'h');
	glPopMatrix();

	glPopMatrix();

}



void drawTree(short x, short z) {

	glPushMatrix();
	glTranslatef(x, 0, z);
	glScaled(3, 3, 3);
	glPushMatrix();//											TreeTrunk
	glTranslatef(0, 3, 0);
	glScalef(1, 3, 1);
	glutSolidCube(2, secondView?'p': 't');
	glPopMatrix();
	if (!secondView) {
		glPushMatrix();
		glTranslatef(0, 6, 0);
		glScalef(5, 1, 5);
		//glTranslatef(i, 0, 0);
		glutSolidCube(2, 'l');
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 8, 0);
		glScalef(3, 1, 3);
		//glTranslatef(i, 0, 0);
		glutSolidCube(2, 'l');
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 10, 0);
		glScalef(1, 1, 1);
		//glTranslatef(i, 0, 0);
		glutSolidCube(2, 'l');
		glPopMatrix();
	}
	glPopMatrix();


}



void drawRock(short x, short z) {

	glPushMatrix();
	glTranslatef(x, 0, z);
	glScaled(3, 3, 3);
	glPushMatrix();
	glTranslatef(0, 3, 0);
	glScalef(1, 3, 1);
	glutSolidCube(2, 'z');
	glPopMatrix();
	glPopMatrix();


}

void drawRect(double x, double y, double w, double h, double r, double g, double b) {
	glColor3f(r, g, b);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x, y + h);
	glVertex2f(x + w, y + h);
	glVertex2f(x + w, y);

	glEnd();
}
void drawRectTex(double x, double y, double w, double h,char texName) {
	switch (texName) {
	case's':		glBindTexture(GL_TEXTURE_2D, scroll.texture[0]); break;
	case'c':		 
	case'w':		
	case'f':		 
	case'd':
		equipedWeapon == myFist ? glBindTexture(GL_TEXTURE_2D, fist.texture[0]):
		equipedWeapon == myStick? glBindTexture(GL_TEXTURE_2D, stickIcon.texture[0]) :
		equipedWeapon == mySword ? glBindTexture(GL_TEXTURE_2D, swordIcon.texture[0]): 
		glBindTexture(GL_TEXTURE_2D, warhammerIcon.texture[0]);
		break;

	}
	
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);	
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).

	glVertex2f(x, y);
	glTexCoord2f(1, 0);
	glVertex2f(x, y + h);
	glTexCoord2f(1, 1);
	glVertex2f(x + w, y + h);
	glTexCoord2f(0, 1);
	glVertex2f(x + w, y);

	glEnd();
}


void print(double x, double y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}
void draw2D() {
	if (currHP > maxHP)if (currHP > 0)currHP -= 0.0005;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity(); //start drawing 2d from here (height is 1 , width is 1)
	drawRect(0.02, 0.89, 0.36, 0.09, 1, 1, 0);
	drawRect(0.04, 0.91, currHP, 0.05, 249.0 / 255.0, 202.0 / 255.0, 36.0 / 255.0);
	//249, 202, 36

	drawRect(0.04, 0.91, maxHP, 0.05, 1, 0, 0);
	glColor3d(1,1,0);
	drawRect(0.86, 0, 0.25, 0.18, 1, 1, 1);// WEAPON / SCROLLS PANEL
	drawRectTex(0.95, 0.1, 0.05, 0.08,'s');//SCROLL
	if (stickOn) {
		drawRectTex(0.95, 0, 0.05, 0.08, 'c');//stick icon
	}
	else if (swordOn)
	{
		drawRectTex(0.95, 0, 0.05, 0.08, 'd');//sword icon

	}
	else 
		if (warhammerOn) {
			drawRectTex(0.95, 0, 0.05, 0.08, 'w');//warhammer icon
		}
		else
			drawRectTex(0.95, 0, 0.05, 0.08, 'f'); // fist icon

	glColor3f(0, 0, 0);
	char* Scrolls[40];
	sprintf((char*)Scrolls, "Scrolls %d/3", scrollN);
	print(0.87, 0.13, (char*)Scrolls);
	sprintf((char*)Scrolls, "Weapon", scrollN);

	print(0.87, 0.03, (char*)Scrolls);


	if (secondView) {
		drawRect(0.69, 0.94, 0.31, 0.06, 1, 1, 1);
		glColor3f(0, 0, 0);
		print(0.7, 0.95, "Press 'l' to go back to the first view");
	}
	else {
	
		drawRect(0.69, 0.94, 0.31, 0.06, 1, 1, 1);
		glColor3f(0, 0, 0);
		print(0.7, 0.95, "Press 'k' to go to the second view");
		
	}
	if (winState) {
		
		

		if (firstLevel) {
			drawRect(0.4, 0.7, 0.24, 0.1, 1, 1, 1);
			glColor3f(0, 0, 0);
			print(0.41, 0.76, "You cleared this level !");
			print(0.41, 0.71, "Press '2' to Go to next level.");
		
		}
		else {
			drawRect(0.4, 0.7, 0.26, 0.1, 1, 1, 1);
			glColor3f(0, 0, 0);
			print(0.41, 0.76, "You defeated the Zombie king!");
			print(0.41, 0.71, "press '2' to restart this level");

		}
	}
	if (deathPanel > 200)
	{
		died = false;
		deathPanel = 0;
	}if (died) {
		drawRect(0.45, 0.7, 0.1, 0.1, 1, 1, 1);
		glColor3f(0, 0, 0);

		print(0.46, 0.75, "You died!");
		deathPanel++;

	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT2);


}


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


Vector Eye(playerX, playerY, playerZ);
Vector At(playerX + 2, playerY, playerZ);
Vector Up(0, 1, 0);

int cameraZoom = 0;
double x = 0;
bool RotatePlayer = false;
bool thirdPerson = false;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_tank;
// Textures

double speedZ() {//get direction vector in z-cord
	//printf("outZ  x:%f z:%f\n", playerX, playerZ);
	if (firstLevel) {
		if (playerX > -35 && playerX < 35) // first barrier
			if (playerZ > 60 && playerZ < 70)
			{
				if (playerZ < 60 + speed)
				{
					printf("left x:%f z:%f\n", playerX, playerZ);
					return min(0, speed * sin(ang));
				}
				if (playerZ > 70 - speed)
				{
					printf("right x:%f z:%f\n", playerX, playerZ);
					return max(0, speed * sin(ang));
				}
			}
		if (playerX > 0 && playerX < 40)  // 2nd barrier p1
			if (playerZ > -5 && playerZ < 5)
			{
				if (playerZ < -5 + speed)
				{
					printf("left x:%f z:%f\n", playerX, playerZ);
					return min(0, speed * sin(ang));
				}
				if (playerZ > 5 - speed)
				{
					printf("right x:%f z:%f\n", playerX, playerZ);
					return max(0, speed * sin(ang));
				}
			}
		if (playerX > 60 && playerX < 115) // 2nd barrier p2
			if (playerZ > -5 && playerZ < 5)
			{
				if (playerZ < -5 + speed)
				{
					printf("left x:%f z:%f\n", playerX, playerZ);
					return min(0, speed * sin(ang));
				}
				if (playerZ > 5 - speed)
				{
					printf("right x:%f z:%f\n", playerX, playerZ);
					return max(0, speed * sin(ang));
				}
			}

	}

	return playerZ < -170 ? max(0, speed * sin(ang)) : playerZ>155 ? min(0, speed * sin(ang)) : speed * sin(ang);

}


double speedZs() {//get direction vector in z-cord
	//printf("outZ  x:%f z:%f\n", playerX, playerZ);
	if (firstLevel) {
		if (playerX > -35 && playerX < 35) // first barrier
			if (playerZ > 60 && playerZ < 70)
			{
				if (playerZ < 60 + speed)
				{
					printf("left x:%f z:%f\n", playerX, playerZ);
					return max(0, speed * sin(ang));
				}
				if (playerZ > 70 - speed)
				{
					printf("right x:%f z:%f\n", playerX, playerZ);
					return min(0, speed * sin(ang));
				}
			}
		if (playerX > 0 && playerX < 40)  // 2nd barrier p1
			if (playerZ > -5 && playerZ < 5)
			{
				if (playerZ < -5 + speed)
				{
					printf("left x:%f z:%f\n", playerX, playerZ);
					return max(0, speed * sin(ang));
				}
				if (playerZ > 5 - speed)
				{
					printf("right x:%f z:%f\n", playerX, playerZ);
					return min(0, speed * sin(ang));
				}
			}
		if (playerX > 60 && playerX < 115) // 2nd barrier p2
			if (playerZ > -5 && playerZ < 5)
			{
				if (playerZ < -5 + speed)
				{
					printf("left x:%f z:%f\n", playerX, playerZ);
					return max(0, speed * sin(ang));
				}
				if (playerZ > 5 - speed)
				{
					printf("right x:%f z:%f\n", playerX, playerZ);
					return min(0, speed * sin(ang));
				}
			}

	}

	return playerZ < -170 ? min(0, speed * sin(ang)) : playerZ>155 ? max(0, speed * sin(ang)) : speed * sin(ang);

}


double speedX() {//get direction vector in x-cord
	printf("outZ  x:%f z:%f\n", playerX, playerZ);
	if (firstLevel) {
		if (playerX > -35 && playerX < 35)
			if (playerZ > 60 && playerZ < 70)
			{
				if (playerX < -35 + speed)
				{
					printf("bottom x:%f z:%f\n", playerX, playerZ);
					return min(0, speed * cos(ang));
				}
				if (playerX > 35 - speed)
				{
					printf("upper x:%f z:%f\n", playerX, playerZ);
					return max(0, speed * cos(ang));
				}

			}

		if (playerX > 0 && playerX < 40)
			if (playerZ > -5 && playerZ < 5)
			{
				if (playerX < 0 + speed)
				{
					printf("bottom x:%f z:%f\n", playerX, playerZ);
					return min(0, speed * cos(ang));
				}
				if (playerX > 40 - speed)
				{
					printf("upper x:%f z:%f\n", playerX, playerZ);
					return max(0, speed * cos(ang));
				}

			}

		if (playerX > 60 && playerX < 115)
			if (playerZ > -5 && playerZ < 5)
			{
				if (playerX < 60 + speed)
				{
					printf("bottom x:%f z:%f\n", playerX, playerZ);
					return min(0, speed * cos(ang));
				}
				if (playerX > 115 - speed)
				{
					printf("upper x:%f z:%f\n", playerX, playerZ);
					return max(0, speed * cos(ang));
				}

			}
	}
	return playerX < -35 ? max(0, speed * cos(ang)) : playerX>115 ? min(0, speed * cos(ang)) : speed * cos(ang);

}


double speedXs() {//get direction vector in x-cord
	printf("outZ  x:%f z:%f\n", playerX, playerZ);
	if (firstLevel) {
		if (playerX > -35 && playerX < 35)
			if (playerZ > 60 && playerZ < 70)
			{
				if (playerX < -35 + speed)
				{
					printf("bottom x:%f z:%f\n", playerX, playerZ);
					return max(0, speed * cos(ang));
				}
				if (playerX > 35 - speed)
				{
					printf("upper x:%f z:%f\n", playerX, playerZ);
					return min(0, speed * cos(ang));
				}

			}

		if (playerX > 0 && playerX < 40)
			if (playerZ > -5 && playerZ < 5)
			{
				if (playerX < 0 + speed)
				{
					printf("bottom x:%f z:%f\n", playerX, playerZ);
					return max(0, speed * cos(ang));
				}
				if (playerX > 40 - speed)
				{
					printf("upper x:%f z:%f\n", playerX, playerZ);
					return min(0, speed * cos(ang));
				}

			}

		if (playerX > 60 && playerX < 115)
			if (playerZ > -5 && playerZ < 5)
			{
				if (playerX < 60 + speed)
				{
					printf("bottom x:%f z:%f\n", playerX, playerZ);
					return max(0, speed * cos(ang));
				}
				if (playerX > 115 - speed)
				{
					printf("upper x:%f z:%f\n", playerX, playerZ);
					return min(0, speed * cos(ang));
				}

			}
	}
	return playerX < -35 ? min(0, speed * cos(ang)) : playerX>115 ? max(0, speed * cos(ang)) : speed * cos(ang);

}

double rSpeedX() {//get direction vector in x-cord

	return speed * cos(ang);

}
double rSpeedZ() {//get direction vector in x-cord

	return speed * sin(ang);

}


double Zang(int i) {
	return atan((double)((double)(zombieZ[i] - playerZ) / (double)(zombieX[i] - playerX)));
}


double ZspeedZ(int i,bool pushed) {
	float ans;
	if (dist(i) > aggro)return 0;
	if (zombieZ[i] > playerZ) ans = -abs(speed * sin(Zang(i)));
	else  ans = abs(speed * sin(Zang(i)));
	
	if (firstLevel) {
		if (pushed) {
			if (playerX > -35 && playerX < 35)
				if (playerZ > 60 && playerZ < 70)
				{
					if (playerZ < 60 + speed)
					{
						return min(0, ans / 2);
					}
					if (playerZ > 70 - speed)
					{
						return max(0, ans / 2);
					}
				}

			if (playerX > 0 && playerX < 40)  // 2nd barrier p1
				if (playerZ > -5 && playerZ < 5)
				{
					if (playerZ < -5 + speed)
					{
						return min(0, ans / 2);
					}
					if (playerZ > 5 - speed)
					{
						return max(0, ans / 2);
					}
				}
			if (playerX > 60 && playerX < 115) // 2nd barrier p2
				if (playerZ > -5 && playerZ < 5)
				{
					if (playerZ < -5 + speed)
					{
						return min(0, ans / 2);
					}
					if (playerZ > 5 - speed)
					{
						return max(0, ans / 2);
					}
				}
		}
		else {
			if (zombieX[i] > -35 && zombieX[i] < 35)
				if (zombieZ[i] > 60 && zombieZ[i] < 70)
				{
					if (zombieZ[i] < 60 + speed)
					{
						return min(0, ans / 2);
					}
					if (zombieZ[i] > 70 - speed)
					{
						return max(0, ans / 2);
					}
				}

			if (zombieX[i] > 0 && zombieX[i] < 40)  // 2nd barrier p1
				if (zombieZ[i] > -5 && zombieZ[i] < 5)
				{
					if (zombieZ[i] < -5 + speed)
					{
						return min(0, ans / 2);
					}
					if (zombieZ[i] > 5 - speed)
					{
						return max(0, ans / 2);
					}
				}
			if (zombieX[i] > 60 && zombieX[i] < 115) // 2nd barrier p2
				if (zombieZ[i] > -5 && zombieZ[i] < 5)
				{
					if (zombieZ[i] < -5 + speed)
					{
						return min(0, ans / 2);
					}
					if (zombieZ[i] > 5 - speed)
					{
						return max(0, ans / 2);
					}
				}
		}
	}
	if (pushed)
		return playerZ < -170 ? max(0, ans / 2) : playerZ>155 ? min(0, ans / 2) : ans / 2;
	else 
		return ans / 2;
	
	}

double ZspeedX(int i,bool pushed) {
	float ans;
	if (dist(i) > aggro)return 0;
	mciSendString("play ./audio/zombieSound.mp3", NULL, 0, NULL);

	if (zombieX[i] > playerX) ans = -abs(speed * cos(Zang(i)));
	else  ans = abs(speed * cos(Zang(i)));

	if (firstLevel) {
		if (pushed) {
			if (playerX > -35 && playerX < 35)
				if (playerZ > 60 && playerZ < 70)
				{
					if (playerX < -35 + speed)
					{
						printf("Z left x:%f z:%f\n", playerX, playerZ);
						return min(0, ans / 2);
					}
					if (playerX > 35 - speed)
					{
						printf("Z right x:%f z:%f\n", playerX, playerZ);
						return max(0, ans / 2);
					}
				}

			if (playerX > 0 && playerX < 40)
				if (playerZ > -5 && playerZ < 5)
				{
					if (playerX < 0 + speed)
					{
						printf("bottom x:%f z:%f\n", playerX, playerZ);
						return min(0, ans / 2);
					}
					if (playerX > 40 - speed)
					{
						printf("upper x:%f z:%f\n", playerX, playerZ);
						return max(0, ans / 2);
					}

				}

			if (playerX > 60 && playerX < 115)
				if (playerZ > -5 && playerZ < 5)
				{
					if (playerX < 60 + speed)
					{
						printf("bottom x:%f z:%f\n", playerX, playerZ);
						return min(0, ans / 2);
					}
					if (playerX > 115 - speed)
					{
						printf("upper x:%f z:%f\n", playerX, playerZ);
						return max(0, ans / 2);
					}

				}
		}
		else {
			if (zombieX[i] > -35 && zombieX[i] < 35)
				if (zombieZ[i] > 60 && zombieZ[i] < 70)
				{
					if (zombieX[i] < -35 + speed)
					{
						printf("Z left x:%f z:%f\n", zombieX[i], zombieZ[i]);
						return min(0, ans / 2);
					}
					if (zombieX[i] > 35 - speed)
					{
						printf("Z right x:%f z:%f\n", zombieX[i], zombieZ[i]);
						return max(0, ans / 2);
					}
				}

			if (zombieX[i] > 0 && zombieX[i] < 40)
				if (zombieZ[i] > -5 && zombieZ[i] < 5)
				{
					if (zombieX[i] < 0 + speed)
					{
						printf("bottom x:%f z:%f\n", zombieX[i], zombieZ[i]);
						return min(0, ans / 2);
					}
					if (zombieX[i] > 40 - speed)
					{
						printf("upper x:%f z:%f\n", zombieX[i], zombieZ[i]);
						return max(0, ans / 2);
					}

				}

			if (zombieX[i] > 60 && zombieX[i] < 115)
				if (zombieZ[i] > -5 && zombieZ[i] < 5)
				{
					if (zombieX[i] < 60 + speed)
					{
						printf("bottom x:%f z:%f\n", zombieX[i], zombieZ[i]);
						return min(0, ans / 2);
					}
					if (zombieX[i] > 115 - speed)
					{
						printf("upper x:%f z:%f\n", zombieX[i], zombieZ[i]);
						return max(0, ans / 2);
					}

				}
		}
	}
	if (pushed)
		return 	playerX < -35 ? max(0, ans / 2) : playerX>115 ? min(0, ans / 2) : ans / 2;
	else 
		return ans / 2;

}

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	//glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT2);



	GLfloat l2Diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat l2Ambient[] = { 0.0f, 0.0f, 0.1f, 1.0f };
	GLfloat l2Position[] = { 40.0f, 10.0f, 100.0f, 1.0f };
	GLfloat l2Direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, l2Diffuse);
	glLightfv(GL_LIGHT2, GL_AMBIENT, l2Ambient);
	glLightfv(GL_LIGHT2, GL_POSITION, l2Position);
	
	
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
	GLfloat specular[] = { 1.0f, 1.0f, 0.0f, 0.50f };
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
	Eye.x -= speedX();
	Eye.z -= speedZ();
	playerX -= speedX();
	playerZ -= speedZ();
	At.x -= speedX();
	At.z -= speedZ();

	stickX = 100;
	stickZ = 92;


	warhammerX = 74;
	warhammerZ = 92;


	
	swordX = 44;
	swordZ = -51;


	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(thirdPerson ? Eye.x : playerX, thirdPerson ? playerY + 3 : playerY + 0.5, thirdPerson ? Eye.z : playerZ, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	//InitLightSource();

	//InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	glColor3f(1, 1, 1);	// Dim the ground texture a bit

	glPushMatrix(); // sun
	glDisable(GL_LIGHTING);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glBindTexture(GL_TEXTURE_2D, sunTex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	//drawhere

	glTranslatef(sunX, sunY, sunZ);
	gluSphere(qobj, 6, 30, 30);
	gluDeleteQuadric(qobj);
	glEnable(GL_LIGHTING);

	glPopMatrix();

	
	
	if (!firstLevel) {
		glBindTexture(GL_TEXTURE_2D, redGround.texture[0]);	// Bind the ground texture
	}
	else {
		if(secondView)
			glBindTexture(GL_TEXTURE_2D, rockGround.texture[0]);	// Bind the ground texture
		else
			glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	}
	
	//glBindTexture(GL_TEXTURE_2D, tex_face.texture[0]);	// Bind the ground texture
	short groundX = -40;
	short groundZ = -175;
	short groundWX = 160;
	short groundWZ = 335;
	if (sunZ > 155)sunSpeed *= -1;
	if (sunZ < -170)sunSpeed *= -1;
	sunZ += sunSpeed;
	glEnable(GL_LIGHT2);


	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(groundX, 0, groundZ);
	glTexCoord2f(5, 0);
	glVertex3f(groundX + groundWX, 0, groundZ);
	glTexCoord2f(5, 5);
	glVertex3f(groundX + groundWX, 0, groundZ + groundWZ);
	glTexCoord2f(0, 5);
	glVertex3f(groundX, 0, groundZ + groundWZ);
	glEnd();
	glPopMatrix();
	GLfloat light_position[] = { sunX, sunY,sunZ, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat l0Direction[] = { 0.0, -1.0, 0.0 };
	if (keyarr['n'] == 1) {

		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 50.0);
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 128);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);
	}
	else {
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);

	}
	
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void drawScroll(short i) {
	scrollAng += scrollrotSpeed;
	glPushMatrix(); // scroll
	glTranslatef(scrollX[i], 3, scrollZ[i]);
	glRotated(scrollAng, 0, 1, 0);
	glScaled(1, 1, 0.1);
	glutSolidCube(3, 's');
	glPopMatrix();


}

void drawZombie(int i) {

	glPushMatrix();
	if (zombieDeathReact[i])
	{
		glTranslatef(zombieX[i], 0, zombieZ[i]);

		glRotated(-zombieDeathReactCounter[i], 0, 0, 1);
		glTranslatef(-zombieX[i], 0, -zombieZ[i]);

		zombieDeathReactCounter[i] = zombieDeathReactCounter[i] + 1;
		printf("%d in react\n", zombieDeathReactCounter[i]);

		if (zombieDeathReactCounter[i] > 100) {
			zombieDead[i] = true;
			remaining--;
			printf("zombie %d is deeeead \n", i);
			zombieDeathReact[i] = false;


		}
	}
	glPushMatrix();//											Zombie head
	glTranslatef(zombieX[i], playerY + 0.5, zombieZ[i]);
	glRotated(angInDegrees(Zang(i)), 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, texFace);
	if (playerX < zombieX[i]) {
		glutSolidCube(2, i == 0 && !firstLevel ? 4 : 0);
	}
	else
	glutSolidCube(2, i == 0 && !firstLevel ? 9 :8 );

	
	glPopMatrix();

	glPushMatrix();//											Zombie body
	glTranslatef(zombieX[i], playerY - 2, zombieZ[i]);
	glRotated(angInDegrees(Zang(i)), 0, 1, 0); // 
	glScalef(1, 2, 2);
	glutSolidCube(1.5, i == 0 && !firstLevel ? 7:3);
	glPopMatrix();
	float handXMargin = 1;
	float handZMargin = 2;

	glPushMatrix();//											Zombie Right Arm
	glTranslatef(zombieX[i] + handXMargin, playerY - 2 + 0.75, zombieZ[i] + handZMargin);
	glTranslatef(-handXMargin, 0, -handZMargin);
	glRotated(zombieX[i] > playerX ? angInDegrees(Zang(i)) + 180 : angInDegrees(Zang(i)), 0, 1, 0); // rotate around body
	glTranslatef(handXMargin, 0, handZMargin);
	limbAngZ += angIncZ;
	if (limbAngZ > 30)angIncZ *= -1;
	if (limbAngZ < -30)angIncZ *= -1;
	glScalef(2.5, 1, 1);
	glutSolidCube(1.5, i == 0 && !firstLevel ? 5 : 1);
	glPopMatrix();

	glPushMatrix();//											Zombie Left Arm
	glTranslatef(zombieX[i] + handXMargin, playerY - 2 + 0.75, zombieZ[i] - handZMargin);
	glTranslatef(-handXMargin, 0, handZMargin);
	glRotated(zombieX[i] > playerX ? angInDegrees(Zang(i)) + 180 : angInDegrees(Zang(i)), 0, 1, 0); // rotate around body
	glTranslatef(handXMargin, 0, -handZMargin);
	glScalef(2.5, 1, 1);
	glutSolidCube(1.5, i == 0 && !firstLevel ? 5:1);
	glPopMatrix();

	glPushMatrix();//											Zombie Right Leg
	glTranslatef(zombieX[i], playerY - 5, zombieZ[i] + 0.75);
	glTranslatef(0, 0, -0.75);
	//if(dist(i) < aggro)
	glRotated(angInDegrees(Zang(i)), 0, 1, 0); // 
	glTranslatef(0, 0, 0.75);
	glTranslatef(0, playerY - 3.5, 0);
	if (!zombieDeathReact[i])
		if (dist(i) < aggro)

		glRotated(-limbAngZ, 0, 0, 1); // rotate around body
	glTranslatef(0, -playerY + 3.5, 0);

	glScalef(1, 2, 1);
	glutSolidCube(1.5, i == 0 && !firstLevel ?6: 2);
	glPopMatrix();

	glPushMatrix();//											Zombie Left Leg
	glTranslatef(zombieX[i], playerY - 5, zombieZ[i] - 0.75);
	glTranslatef(0, 0, 0.75);
	glRotated(angInDegrees(Zang(i)), 0, 1, 0); // 
	glTranslatef(0, 0, -0.75);
	glTranslatef(0, playerY - 3.5, 0);
	if (!zombieDeathReact[i])
		if (dist(i) < aggro)

		glRotated(limbAngZ, 0, 0, 1); // rotate around body
	glTranslatef(0, -playerY + 3.5, 0);

	glScalef(1, 2, 1);
	glutSolidCube(1.5, i == 0 && !firstLevel ? 6:2);
	glPopMatrix();
	glPopMatrix();

}



void drawHuman() {
	glPushMatrix();
	//glTranslated(0,3,0);
	if (humanDeathReaction) {
		glTranslatef(playerX, 0, playerZ);

		glRotated(-humanDeathReactionCounter, 0, 0, 1);
		glTranslatef(-playerX, 0, -playerZ);
		humanDeathReactionCounter++;
		//	printf("%d in react\n", zombieDeathReactCounter[i]);

		if (humanDeathReactionCounter > 100) {
			mciSendString("play ./audio/opening.mp3", NULL, 0, NULL);

			died = true;
			//remaining--;
			//printf("zombie %d is deeeead \n", i);
			humanDeathReaction = false;
			humanDeathReactionCounter = 0;
			maxHP = 0.32;
			currHP= 0.32;

			playerX = 42;
			playerY = 7;

			playerZ = 122;

			At.x = 42;
			At.y = 7;
			At.z = 124;

			swordOn = false;
			stickOn= false;
			warhammerOn = false;
			equipedWeapon = myFist;
			if (firstLevel) {
				for (int i = 0; i < 7; i++)
				{
					zombieDead[i] = false;
				}
				for (int i = 0; i < 7; i++)
				{
					zombieHealth[i] = 0;
				}
				for (int i = 0; i < 7; i++)
				{
					zombieDeathReactCounter[i] = 0;
				}
				for (int i = 0; i < 3; i++)
				{
					scrollTaken[i] = false;
				}
				for (int i = 0; i < 7; i++)
				{
					zombieX[i] = zombieXlvl1[i];
					zombieZ[i] = zombieZlvl1[i];
				}
				remaining = 7;
				scrollN = 0;
			}
			else {
				for (int i = 0; i < 19; i++)
				{
					zombieDead[i] = false;
				}
				for (int i = 0; i < 19; i++)
				{
					zombieHealth[i] = 0;
				}
				for (int i = 0; i < 19; i++)
				{
					zombieDeathReactCounter[i] = 0;
				}
				for (int i = 0; i < 3; i++)
				{
					scrollTaken[i] = false;
				}
				for (int i = 0; i < 19; i++)
				{
					zombieX[i] = zombieXlvl2[i];
					zombieZ[i] = zombieZlvl2[i];
				}
				remaining = 19;
				scrollN = 0;

			}
			}
	}
	glPushMatrix();//											Human head
	glTranslatef(playerX, playerY + 0.5, playerZ);
	glRotated(angInDegrees(ang), 0, 1, 0);
	if (thirdPerson)
		glutSolidCube(2, '0');
	glPopMatrix();

	glPushMatrix();//											Human body
	glTranslatef(playerX, playerY - 2, playerZ);
	glRotated(angInDegrees(ang), 0, 1, 0); // 
	glScalef(1, 2, 2);
	glutSolidCube(1.5, '3');
	glPopMatrix();

	glPushMatrix();//											Right Arm
	glTranslatef(playerX, playerY - 2, playerZ + 2);
	glTranslatef(0, 0, -2);
	glRotated(angInDegrees(ang), 0, 1, 0); // rotate around body
	glTranslatef(0, 0, 2);
	if (moving)
	{
		limbAng += angInc;

		if (limbAng > 30)angInc *= -1;
		if (limbAng < -30)angInc *= -1;

		if (hittingReaction) {
			hittingAng += 2;
			glTranslatef(0, playerY - 5.5, 0);
			glRotated(hittingAng, 0, 1, 0); //rotate hand to left
			glRotated(90 - hittingAng, 0, 0, 1); // rotate hand downwards

			glTranslatef(0, -playerY + 5.5, 0);

			//q press = hit
		}
		else {
			glTranslatef(0, playerY - 3.5, 0);
			glRotated(limbAng, 0, 0, 1); // rotate around body
			glTranslatef(0, -playerY + 3.5, 0);
		}
	}
	else 		if (hittingReaction) {
		hittingAng += 2;
		glTranslatef(0, playerY - 5.5, 0);
		glRotated(hittingAng, 0, 1, 0); //rotate hand to left
		glRotated(90 - hittingAng, 0, 0, 1); // rotate hand downwards

		glTranslatef(0, -playerY + 5.5, 0);

		//q press = hit
	}
	if (equipedWeapon == myStick) { drawStick(playerX,playerZ-2); }
	else if (equipedWeapon == mySword) { drawSword(playerX, playerZ - 2); }
	else if (equipedWeapon == myWarhammer) {  drawWarhammer(playerX, playerZ - 2); }

	glScalef(1, 2, 1);
	glutSolidCube(1.5, '1');
	glPopMatrix();

	glPushMatrix();//											Left Arm
	glTranslatef(playerX, playerY - 2, playerZ - 2);
	glTranslatef(0, 0, 2);
	glRotated(angInDegrees(ang), 0, 1, 0); // 
	glTranslatef(0, 0, -2);

	if (moving)
	{
		glTranslatef(0, playerY - 3.5, 0);
		glRotated(-limbAng, 0, 0, 1); // rotate around body
		glTranslatef(0, -playerY + 3.5, 0);

	}


	glScalef(1, 2, 1);
	glutSolidCube(1.5, '1');
	glPopMatrix();

	glPushMatrix();//											Right Leg
	glTranslatef(playerX, playerY - 5, playerZ + 0.75);
	glTranslatef(0, 0, -0.75);
	glRotated(angInDegrees(ang), 0, 1, 0);
	glTranslatef(0, 0, 0.75);
	if (moving)
	{
		glTranslatef(0, playerY - 3.5, 0);
		glRotated(-limbAng, 0, 0, 1); // rotate around body
		glTranslatef(0, -playerY + 3.5, 0);

	}
	glScalef(1, 2, 1);
	glutSolidCube(1.5, '2');
	glPopMatrix();

	glPushMatrix();//											Left Leg
	glTranslatef(playerX, playerY - 5, playerZ - 0.75);
	glTranslatef(0, 0, 0.75);
	glRotated(angInDegrees(ang), 0, 1, 0); // 
	glTranslatef(0, 0, -0.75);
	if (moving)
	{
		glTranslatef(0, playerY - 3.5, 0);
		glRotated(limbAng, 0, 0, 1); // rotate around body
		glTranslatef(0, -playerY + 3.5, 0);

	}
	glScalef(1, 2, 1);
	glutSolidCube(1.5, '2');
	glPopMatrix();
	glPopMatrix();

}

void display(void)
{
	if (remaining == 0 && scrollN == 3) {

		keyarr['n'] = true;
		winState = true;
		mciSendString("play ./audio/victory.mp3", NULL, 0, NULL);

	}
	else {
		keyarr['n'] = false;}
	if (hittingReaction)
	{
		if (hittingReactionCounter < 22)hittingReactionCounter++;
		else { hittingReaction = false; hittingReactionCounter = 0; hittingAng = 0; }
	}
	x += 0.01;
	glLoadIdentity();
	gluLookAt(thirdPerson ? Eye.x : playerX, thirdPerson ? playerY + 3 : playerY + 0.5, thirdPerson ? Eye.z : playerZ, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	short lim = firstLevel ? 7 : 19;
	for (int i = 0; i < lim; i++)
	{
		if (zombieDead[i])
			continue;
		if (zombieHitReact[i]) {
			zombieHitReactCounter[i]++;

			zombieX[i] -= 3 * ZspeedX(i,false);
			zombieZ[i] -= 3 * ZspeedZ(i, false);

			if (zombieHitReactCounter[i] > 10)
			{
				zombieHitReact[i] = false;
				zombieHitReactCounter[i] = 0;
			}
			if (zombieDeathReact[i])continue;
		}
		else

			if (dist(i) > 4) { // if zombie is still far enough from human (zombie hit rang is 4)
				if (dist(i) < 6+range) {	//human hit rang is 6
					if (hittingReaction && hittingReactionCounter < 3) {// zombie is hit during the first 3 frames of the reaction
						mciSendString("play ./audio/zombieGetHit.mp3", NULL, 0, NULL);

						zombieHitReact[i] = true;
						zombieHealth[i] += 1 + damage;
						if (i == 0&&!firstLevel) {
							if (zombieHealth[i] > zombieHP + 38) {
								printf("@@Zombie %d is reacting\n", i);
								zombieDeathReact[i] = true;

							}
						}
						else {
							if (zombieHealth[i] > zombieHP) {
								printf("@@Zombie %d is reacting\n", i);
								zombieDeathReact[i] = true;

							}
						}
					}
					else {
						if (!zombieDeathReact[i]) {
							zombieX[i] += ZspeedX(i, false);
							zombieZ[i] += ZspeedZ(i, false);
						}
					}

				}
				else {
					if (!zombieDeathReact[i]) {

						zombieX[i] += ZspeedX(i,false);
						zombieZ[i] += ZspeedZ(i,false);
					}
				}
			}


			else {
				if (!zombieDeathReact[i] && !disableHitting) {
					
					
					if (maxHP <= 0) {
						mciSendString("close ./audio/opening.mp3", NULL, 0,NULL);

						mciSendString("play ./audio/playerDied.mp3", NULL, 0, NULL);

						humanDeathReaction = true;
					}
					else {

						mciSendString("play ./audio/playerGetHit.mp3", NULL, 0, NULL);

						maxHP -= 0.01;
					}
					zHitterIndex = i;
					printf("zombie %d hit you \n", i);
					humanReaction = true;
				}
			}

	}
	if (humanReaction) {
		if (humanReactionCounter < humanReactionCounterLimit) {
			humanReactionCounter++;
			for (byte i = 0; i < 5; i++)
			{

			
			Eye.x += ZspeedX(zHitterIndex,true);
			Eye.z += ZspeedZ(zHitterIndex,true);
			playerX +=  ZspeedX(zHitterIndex,true);
			playerZ +=  ZspeedZ(zHitterIndex,true);
			At.x +=  ZspeedX(zHitterIndex,true);
			At.z +=  ZspeedZ(zHitterIndex,true);
			}
		}
		else {
			humanReaction = false;
			humanReactionCounter = 0;
		}
	}
	else
		if (keyarr['w'] == 1) {


			Eye.x += speedX();
			Eye.z += speedZ();
			playerX += speedX();
			playerZ += speedZ();
			At.x += speedX();
			At.z += speedZ();

			//	printf();

		}
	if (keyarr['s'] == 1) {
		Eye.x -= speedXs();
		Eye.z -= speedZs();
		playerX -= speedXs();
		playerZ -= speedZs();
		At.x -= speedXs();
		At.z -= speedZs();

	}
	if (keyarr['n'] == 1) {
		playerY =40;
		At.y = 40;
		sunX = playerX;
		sunZ = playerZ;
		sunY = 60;
	//	drawTree(playerX,playerZ);
	}

	if (keyarr['a'] == 1) {
		ang -= 0.02;
		Eye.x = playerX - PointBehindHuman * rSpeedX();
		Eye.z = playerZ - PointBehindHuman * rSpeedZ();

		At.x = playerX + 200 * rSpeedX();
		At.z = playerZ + 200 * rSpeedZ();
		//view

	}
	if (keyarr['d'] == 1) {
		ang += 0.02;
		Eye.x = playerX - PointBehindHuman * rSpeedX();
		Eye.z = playerZ - PointBehindHuman * rSpeedZ();

		At.x = playerX + 200 * rSpeedX();
		At.z = playerZ + 200 * rSpeedZ();
	}

	//GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	//GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	// Draw Ground
	RenderGround();

	drawHuman();

	stickAng += scrollrotSpeed;
	
	//showStick(90,126);

	/*showWarhammer(80,126);
	drawWarhammer(90, 126);*/
	if (firstLevel) {
		if (!stickOn) {
			if (weaponDist(stickX, stickZ) < 3) {
				mciSendString("play ./audio/weaponPickUp.mp3", NULL, 0, NULL);

				stickOn = true;
				equipedWeapon = myStick;
				range = 1;
				damage = 1;
			}
			else {
				showStick(stickX, stickZ);
			}
		}
		if (!swordOn)
		{
			if (weaponDist(swordX, swordZ) < 3) {
				mciSendString("play ./audio/weaponPickUp.mp3", NULL, 0, NULL);

				swordOn = true;
				equipedWeapon = mySword;
				range = 4;
				damage = 3;
			}
			else {
				showSword(swordX, swordZ);
			}
		}
		else {
			glPushMatrix();

			drawSword(playerX, playerZ - 2);

			glPopMatrix();
		}
	}else

	if (!warhammerOn)
	{
		if (weaponDist(warhammerX, warhammerZ) < 3) {
			mciSendString("play ./audio/weaponPickUp.mp3", NULL, 0, NULL);

			warhammerOn = true;
			equipedWeapon = myWarhammer;
			range = 8;
			damage = 8;
		}
		else {
			showWarhammer(warhammerX, warhammerZ);
		}
	}
	else {
		glPushMatrix();

		drawSword(playerX, playerZ - 2);

		glPopMatrix();
	}



	




	//glPushMatrix(); // USELESS MOVING +Z
	//glTranslatef(0, 3, x);
	//glutSolidCube(2);
	//glPopMatrix();

	//glPushMatrix(); // USELESS MOVING +X
	//glTranslatef(x, 3, 0);
	//glutSolidSphere(2, 15, 15);
	//glPopMatrix();
	if (firstLevel) {
		for (int i = 0; i < 7; i++)
		{
			if (!zombieDead[i])
			{
				drawZombie(i);

			}
		}
	}
	else {
		for (int i = 0; i < 19; i++)
		{
			if (!zombieDead[i])
			{
				drawZombie(i);

			}
		}

	}


	for (short i = 0; i < 3; i++)
	{
		if (scrollTaken[i])continue;
		if (scrollDist(i) < 2)
		{
			mciSendString("close ./audio/scrollAquired.mp3", NULL, 0, NULL);

			mciSendString("play ./audio/scrollAquired.mp3", NULL, 0, NULL);
			scrollTaken[i] = true;
			scrollN++;
			if (maxHP < 0.3) { maxHP += 0.02; 
			currHP += 0.02;
			}
		}
		if (!scrollTaken[i])
		{
			drawScroll(i);

		}
	}

//											Limits
	if (keyarr['n'] == 1) {
		drawTree(playerX, playerZ);
	}
	for (short i = -40; i <= 120; i += 7)
	{
		drawTree(i, -175);
	}
	for (short i = -40; i <= 120; i += 7)
	{
		drawTree(i, 160);
	}
	for (short i = -175; i <= 160; i += 7)
	{
		drawTree(-40, i);
	}
	for (short i = -175; i <= 160; i += 7)
	{
		drawTree(120, i);
	}
//							first barrier

	if (firstLevel) {
		for (short i = -35; i <= 30; i += 7)
		{
			if (secondView)
				drawRock(i, 65);
			else
				drawTree(i, 65);
		}

		for (short i = 0; i <= 40; i += 7)
		{
			if (secondView)
				drawRock(i, 0);
			else

				drawTree(i, 0);
		}

		for (short i = 60; i <= 115; i += 7)
		{
			if (secondView)
				drawRock(i, 0);
			else

				drawTree(i, 0);
		}
	}

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 128.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);


	//																Sky

	glPushMatrix();
	glDisable(GL_LIGHTING);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 250, 100, 100);
	gluDeleteQuadric(qobj);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	draw2D();

	glutSwapBuffers();
	glutPostRedisplay();
}

void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");
	//model_tank.Load("Models/tank/sphere.3DS");

	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	rockGround.Load("Textures/sand.bmp");
	redGround.Load("Textures/redGround.bmp");

	zombieFace.Load("Textures/zombieFace.bmp");
	zombieFace2.Load("Textures/zombieFace2.bmp");
	zombieArm.Load("Textures/zombieArm.bmp");
	zombieLeg.Load("Textures/zombieLeg.bmp");
	zombieTorso.Load("Textures/zombieTorso.bmp");

	kingFace.Load("Textures/kingFace.bmp");
	kingFace2.Load("Textures/kingFace2.bmp");
	kingArm.Load("Textures/kingArm.bmp");
	kingLeg.Load("Textures/kingLeg.bmp");
	kingTorso.Load("Textures/kingTorso.bmp");

	humanFace.Load("Textures/humanFace.bmp");
	humanArm.Load("Textures/humanArm.bmp");
	humanLeg.Load("Textures/humanLeg.bmp");
	humanTorso.Load("Textures/humanTorso.bmp");

	treeTrunk.Load("Textures/treeTrunk.bmp" );
	redTree.Load("Textures/mushroom.bmp");
	hill.Load("Textures/stoneWall.bmp" );

	
	treeLeafe.Load("Textures/treeLeafe.bmp" );
	redLeafe.Load("Textures/redLeafe.bmp");

	rock.Load("Textures/rock.bmp");

	scroll.Load("Textures/scroll.bmp");
	fist.Load("Textures/fist.bmp");

	stick.Load("Textures/stick.bmp");
	swordHandle.Load("Textures/swordHandle.bmp");
	swordBlade.Load("Textures/swordBlade.bmp");

	warhammerHandle.Load("Textures/warhammerHandle.bmp");
	warhammerHead.Load("Textures/warhammerHead.bmp");


	stickIcon.Load("Textures/stickIcon.bmp");
	swordIcon.Load("Textures/swordIcon.bmp");
	warhammerIcon.Load("Textures/warhammerIcon.bmp");



	//texFace.Load("Textures/hitler.bmp");

	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
	//loadBMP(&texFace, "Textures/hitler.bmp", false);
	loadBMP(&sunTex, "Textures/sun.bmp", true);

	//loadBMP(&texFace, "Textures/hitler.bmp", true);

}

//=======================================================================
// Keyboard Function
//=======================================================================
void SpecialUp(int k, int x, int y)
{
	if (k == GLUT_KEY_UP) {
		keyarr[GLUT_KEY_UP] = 0;
		moving = false;
	}if (k == GLUT_KEY_LEFT)
	{
		keyarr[GLUT_KEY_LEFT] = 0;

	}
	if (k == GLUT_KEY_RIGHT)
		keyarr[GLUT_KEY_RIGHT] = 0;

	if (k == GLUT_KEY_DOWN)
	{
		keyarr[GLUT_KEY_DOWN] = 0;
		moving = false;

	}
	glutPostRedisplay();
}

void key(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'w':
		if (!humanDeathReaction||!keyarr['n']) {
			keyarr['w'] = 1;
			moving = true;

		}break;
	case 's':
		if (!humanDeathReaction || !keyarr['n']) {
			keyarr['s'] = 1;
			moving = true;
		}break;
	case 'k':
		secondView = true;
			break;
	case 'l':
		secondView = false;
		break;

		break;

	case '2':
		if (keyarr['n']) {
			keyarr['n'] = 0;
			winState = false;
			firstLevel = false;
		//	LoadAssets();
			playerX = 42;
			playerY = 7;

			playerZ = 122;
			
			At.x = 42;
			At.y = 7;
			At.z = 124;
			
			for (int i = 0; i < 19; i++)
			{
				zombieDead[i] = false;
			}
			for (int i = 0; i < 19; i++)
			{
				zombieHealth[i] = 0;

			}

			for (int i = 0; i < 19; i++)
			{
				zombieDeathReactCounter[i] = 0;

			}
			for (int i = 0; i < 3; i++)
			{
				scrollTaken[i] = false;
				scrollX[i] = i * 15 + 30;
				scrollZ[i] = -153;

			}
			for (int i = 0; i < 19; i++)
			{
				zombieX[i] = zombieXlvl2[i];
				zombieZ[i] = zombieZlvl2[i];
			}

			remaining = 19;
			scrollN = 0;
		}break;


	case 'n':
		keyarr['n'] = 1;
		onTree=true;
		break;
	case 'a':
		if (!humanDeathReaction) {
			keyarr['a'] = 1;
		}
		break;
	case 'd':
		if (!humanDeathReaction) 
		keyarr['d'] = 1;
		break;

	case 'f':
		thirdPerson = !thirdPerson;
		glLoadIdentity();
		gluLookAt(thirdPerson ? Eye.x : playerX, thirdPerson ? playerY + 3 : playerY + 0.5, thirdPerson ? Eye.z : playerZ, At.x, At.y, At.z, Up.x, Up.y, Up.z);

		break;

	case 27:
		exit(0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}


void keyup(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'w':
		keyarr['w'] = 0;
		moving = false;
		break;
	case 's':
		keyarr['s'] = 0;
		moving = false;
		break;

	case 'a':
		keyarr['a'] = 0;
		break;

	case 'd':
		keyarr['d'] = 0;
		break;
	}
}
//=======================================================================
// Motion Function
//=======================================================================

void actM(int button, int state, int x, int y)//mouse function takes 4 parameters: button: which button has been clicked (GLUT_RIGHT_BUTTON or GLUT_LEFT_BUTTON),
											//state wether the button is clicked or released (GLUT_UP or GLUT_DOWN)
											// x and y are the position of the mouse cursor
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//if the left button has been clicked then translate the square to the mouse position
	{
		hittingReaction = true;
		printf("fighting\n");
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)//if the right button has been clicked, translate  the square to the origin (0,0)
	{
	}

	glutPostRedisplay();//redisplay to update the screen with the new paraeters
}

void SpecialInput(int k, int x, int y)
{

	if (k == GLUT_KEY_UP)
	{
		keyarr[GLUT_KEY_UP] = 1;
		moving = true;
	}
	if (k == GLUT_KEY_DOWN)
	{
		keyarr[GLUT_KEY_DOWN] = 1;
		moving = true;
	}

	if (k == GLUT_KEY_LEFT)
		keyarr[GLUT_KEY_LEFT] = 1;
	if (k == GLUT_KEY_RIGHT)
		keyarr[GLUT_KEY_RIGHT] = 1;

}
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

	//GLfloat light_position[] = { Eye.x, Eye.y, Eye.z, 1.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

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
		printf("x,:%d  y:%d\n", x, y);
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
	glLoadIdentity();	gluLookAt(thirdPerson ? Eye.x : playerX, thirdPerson ? playerY + 3 : playerY + 0.5, thirdPerson ? Eye.z : playerZ, At.x, At.y, At.z, Up.x, Up.y, Up.z);

}

//=======================================================================
// Assets Loading Function
//=======================================================================

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	mciSendString("play ./audio/opening.mp3", NULL, 0, NULL);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);


	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(0, 0);

	glutCreateWindow(title);

	glutDisplayFunc(display);

	glutKeyboardFunc(key);

	glutKeyboardUpFunc(keyup);
	//glutKeyboardDownFunc(keydown);

	glutMotionFunc(myMotion);

	glutMouseFunc(actM);

	glutSpecialFunc(SpecialInput);
	glutSpecialUpFunc(SpecialUp);
	glutReshapeFunc(myReshape); //trigered when u resize the window

	myInit();

	LoadAssets();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_LIGHTING); // start light stuff from removing this here
//	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT2);

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}










