/*******************************************************************
								   Multi-Part Model Construction and Manipulation
********************************************************************/

#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <GL/glew.h>
#include <gl/glut.h>
#include "Vector3D.h"
#include "QuadMesh.h"
#include <conio.h>
static void initShader(const GLchar* vShaderFile, const GLchar* fShaderFile);

bool terrainCollide(QuadMesh* quadMesh, float subY, float subX, float subZ);
/////////////////////////////////////////////////////////////////////////////////////////////////
GLfloat textureMap[64][64][3];
GLfloat textureMapObjects[64][64][3];
GLfloat textureMapSub[64][64][3];
GLfloat enemyTextureMap[64][64][3];
GLuint texObjects[2];
GLuint tex[2];
GLuint texSub[2];
GLuint texEnemy[2];

void assignColor(GLfloat col[3], GLfloat r, GLfloat g, GLfloat b) {
	col[0] = r;
	col[1] = g;
	col[2] = b;
}

int isEven(int x) {
	if (x % 2 == 0)
		return 1;
	else
		return 0;
}
void makeTextureMapObjects()
{
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 64; j++) {
			if (isEven(i / 2 + j / 2))
				assignColor(textureMapObjects[i][j], 0, 0, 0);
			else
				assignColor(textureMapObjects[i][j], 1, 1, 1);
		}
}
void makeTextureMap()
{
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 64; j++) {
			if (isEven(i / 8 + j / 8))
				assignColor(textureMap[i][j], 0.9, 0.9, 0.9);
			else
				assignColor(textureMap[i][j], 0.7, 0.7, 0.7);
		}
}

void makeTextureMapSub()
{
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 64; j++) {
			if (isEven(i / 8 + j / 4))
				assignColor(textureMapSub[i][j], 0, 1, 0);
			else if( i < 56 && i > 32) 
				assignColor(textureMapSub[i][j], 0.1, 0.5, 0.8);
			else
				assignColor(textureMapSub[i][j], 0, 0, 0);
		}
}

void makeTextureMapEnemy()
{
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 64; j++) {
			if (isEven(i / 8 + j / 8))
				assignColor(enemyTextureMap[i][j], 1, 0, 0);
			else
				assignColor(enemyTextureMap[i][j], 0, 0, 1);
		}
}

void makeTexturesObjects()
{
	GLfloat planes[] = { 0.0, 0.0, 0.3, 0.0 };
	GLfloat planet[] = { 0.0, 0.3, 0.0, 0.0 };

	glGenTextures(2, texObjects);

	glBindTexture(GL_TEXTURE_2D, texObjects[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, textureMapObjects);

	/************************************ */
	glBindTexture(GL_TEXTURE_2D, texObjects[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, planes);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, planet);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, textureMapObjects);
}

void makeTexturesSub()
{
	GLfloat planes[] = { 0.0, 0.0, 0.3, 0.0 };
	GLfloat planet[] = { 0.0, 0.3, 0.0, 0.0 };

	glGenTextures(2, texSub);

	glBindTexture(GL_TEXTURE_2D, texSub[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, textureMapSub);

	/************************************ */
	glBindTexture(GL_TEXTURE_2D, texSub[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, planes);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, planet);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, textureMapSub);
}

void makeTextures()
{
	GLfloat planes[] = { 0.0, 0.0, 0.3, 0.0 };
	GLfloat planet[] = { 0.0, 0.3, 0.0, 0.0 };

	glGenTextures(2, tex);

	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, textureMap);

	/************************************ */
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, planes);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, planet);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, textureMap);
}

void makeTexturesEnemy()
{
	GLfloat planes[] = { 0.0, 0.0, 0.3, 0.0 };
	GLfloat planet[] = { 0.0, 0.3, 0.0, 0.0 };

	glGenTextures(2, texEnemy);

	glBindTexture(GL_TEXTURE_2D, texEnemy[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, enemyTextureMap);

	/************************************ */
	glBindTexture(GL_TEXTURE_2D, texEnemy[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, planes);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, planet);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT, enemyTextureMap);
}




/////////////////////////////////////////////////////////////////////////////////////////////////


int crashCheck = 0;
float score = 0.0;
bool xFor = false;
bool xRev = false;
float enemyAngle = 0;
void enemyPathing();
void animationHandlerPathing(int param);

float enemyX = -20.0;
float enemyY = 14;
float enemyZ = 0.0;

float totalTime = 0;
bool follow = true;
bool reloaded = true;
float periscopeY = 2;
bool scopeUp = false;
float zFar = 125.0;
float fov = 60.0;
bool periscope = false;
bool floorCol = false;
bool debrisCol = false;
float hullx = -12;
float hullz = 0;
void animationHandlerBullet(int param);
void updateTime();
void animationHandlerWaves(int param);

bool bulletHelp = false;
float bulletX = 2;
float bulletY = 3;
float bulletZ = -5;
bulletAngle = 0;

float dxx = 2;
float dyy = 0;
float dzz = -10;

bool fireTorpedo = false;
bool firingMode = false;
bool terrainCollide(QuadMesh* quadMesh, float subY);
bool gettingDamaged = false;
bool crashed = false;
int health = 100;  
Vector3D location;
bool undo = false;
bool editMode = false;
bool modifyMode = false;
bool arcCam = false;
bool modifyContinious = false;
//float angle = 0.0f;
float deltaAngle = 0.0f;
float deltaAngley = 0.0f;
float ww = 0.0;
float wh = 0.0;
int currentVertex = 0;
float temp = 0.0;
float tempy = 0.0;
float counter = 0;
////////////////////////

int randTheta0 = 35;
int randTheta1 = -5;
int randTheta2 = 0;
int randTheta3 = 0;
int randx0 = 1;
int randx1 = 1;
int randx2 = 0;
int randx3 = 0;
int randy0 = 1;
int randy1 = 1;
int randy2 = 0;
int randy3 = 0;
////////////////////////
double blobx = 0.0;
double bloby = 0.0;
double blobz = 0.0;

double blobA = 5;
double blobB = 1.0;

double oldX = 0.0;
double oldY = 0.0;
double oldZ = 0.0;

double oldA = 1.0;
double oldB = 1.0;



static int portwidth;
static int portheight;

int xOrigin = -1;
int yOrigin = -1;

////////
int xpos = 0;
int ypos = 0;
float xt = 0;
float yt = 0;
float zt = 0;
float eyex = 0.0f;
double eyey = 25.0f;
float eyez = 5.0f;

double d = 8;
double theta = 0.0f;
double thetay = 0.0f;



int last_mx = 0, last_my = 0, cur_mx = 0, cur_my = 0;
int arcball_on = false;

float distance;      // Straight line distance between the camera and look at point

float camX = 0;// distance * -sinf(theta* (M_PI / 180))* cosf((camAngleY)* (M_PI / 180));
float camY = 0;// distance * -sinf((theta)* (M_PI / 180));
float camZ = 0;// -distance * cosf((theta)* (M_PI / 180))* cosf((camAngleY)* (M_PI / 180));

const int meshSize = 160;    // Default Mesh Size
const int vWidth = 2000;     // Viewport width in pixels
const int vHeight = 1000;    // Viewport height in pixels

// Viewport height in pixels

static int currentButton;
static unsigned char currentKey;

// Lighting/shading and material properties for drone - upcoming lecture - just copy for now

// Light properties
static GLfloat light_position0[] = { -6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_position1[] = { 0.0F,0.0F, 100.0F, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

// Material properties
static GLfloat drone_mat_ambient[] = { 0.4F, 0.2F, 0.0F, 1.0F };
static GLfloat drone_mat_specular[] = { 0.1F, 0.1F, 0.0F, 1.0F };
static GLfloat drone_mat_diffuse[] = { 0.9F, 0.5F, 0.0F, 1.0F };
static GLfloat drone_mat_shininess[] = { 0.0F };

// A quad mesh representing the ground
static QuadMesh groundMesh;
static QuadMesh oceanMesh;


// Structure defining a bounding box, currently unused
//struct BoundingBox {
//    Vector3D min;
//    Vector3D max;
//} BBox;

// Prototypes for functions in this module
void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void idleMouse(int x, int y);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void releaseSpecialKey(int key, int x, int y);
Vector3D ScreenToWorld(int x, int y);

static int value = 0;
static int window;
static int menu_id;
static int submenu_id;




//animation handler and idle funciton
void animationHandler(int param);
void animationHandlerPeriscope(int param);

void idleFunction();

//booleans for engine and reverse mechanic
bool engineStart = false;
bool slowing = false;
bool reverse = false;

//function identifiers for submarine draw functions
void drawSubmarine();
void drawEnemySub();
void drawHull();
void drawTower();
void drawTowerFin();
void drawAntenna();
void drawPropeller1();
void drawPropeller2();
void drawFin1();
void drawFin2();
void drawArtifactUp();
void drawPeriscope();
void drawArtifactFallen();

//sub dimensions and angle
float submarineWidth = 6.0;
float submarineHeight = 1.0;
float submarineDepth = 0.4;
float subAngle = 0.0;
float subAngle1 = 0.0;

//rotor angle, and direction x/y/z information
float rotorAngle = 0.0;
float dx = 2;
float dy = 1;
float dz = 2;

//camera x/y/z info
float camdx = -24.0;
float camdy = 15.0;
float camdz = 22.0;

//angles for fins for going up/down/eft/right
float finAngle1 = 0;
float finAngle2 = 90;
float finAngle2R = 0;

float fin1x = 1;
float fin1z = 1;
float fin2x = 1;
float fin2z = 0;

//for creating su windows, side project
float winSub;
float winMain;
int what;

///////////////////////////////////////////////////////////////
bool periscopeMode = false;
// Camera direction
float periscopeZoom = 1.0;
float lx = 0.0, ly = -5; // camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngleP = 0.0; // additional angle change when dragging

// Camera position

float x =0, y = 0; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts
void makeMap();

void makeMap()
{
	for (int i  = 0; i < meshSize /4; i++)
	{
		for (int j = 0; j < meshSize / 4; j++)
		{
			if ((i % 5 == 0 && j % 5 == 0 && i > 0))
			{
				changeY(&groundMesh, (float)i, (float)j, 1.0, 0.1);

			}

			if ((i % 5 == 0 && j % 5 == 0 && i > 0))
			{
				changeY(&groundMesh, (float)-i, (float)j, 1.0, 0.1);

			}

			if ((i % 5 == 0 && j % 5 == 0 && i > 0))
			{
				changeY(&groundMesh, (float)i, (float)-j, 1.0, 0.1);

			}

			if ((i % 5 == 0 && j % 5 == 0 && i > 0))
			{
				changeY(&groundMesh, (float)-i, (float)-j, 1.0, 0.1);

			}
			
			

		}

	}

}
void update(void)
{
	if (deltaMove) { // update camera position
		x += deltaMove * lx * 0.1;
		y += deltaMove * ly * 0.1;
	}
	glutPostRedisplay(); // redisplay everything
}



void menu(int num) {
	if (num == 0) {
		glutDestroyWindow(window);
		exit(0);
	}
	else {
		value = num;
	}
	glutPostRedisplay();
}

void createMenu(void) {
	submenu_id = glutCreateMenu(menu);
	glutAddMenuEntry("HELP", 6);
	glutAddMenuEntry("ARC BALL", 2);
	glutAddMenuEntry("EDIT MESH", 3);
	glutAddMenuEntry("EDIT BLOB", 4);
	glutAddMenuEntry("RESET MESH", 5);


	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Clear", 1);
	glutAddSubMenu("MODE", submenu_id);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

int main(int argc, char** argv)
{

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(vWidth, vHeight);
	glutInitWindowPosition(200, 30);
	window = glutCreateWindow("Assignment 1");
	createMenu();
	glewInit();
	initShader("vFragD.glsl", "fD.glsl");

	//glEnable(GL_BLEND);
	//glClearColor(0.0, 0.0, 0.0, 0.0);

	// Initialize GL
	initOpenGL(vWidth, vHeight);


	// Register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutIdleFunc(update); // incremental update 

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(functionKeys);
	glutSpecialUpFunc(releaseSpecialKey); // process special key release

	//printf("%f\n", mid(2.0, 5.0));

	glutTimerFunc(10, animationHandlerBullet, 0);
	glutTimerFunc(10, animationHandler, 0);
	glutTimerFunc(100, animationHandlerWaves, 0);
	glutTimerFunc(100, animationHandlerPeriscope, 0);
	glutTimerFunc(100, animationHandlerPathing, 0);
	



	glutMainLoop();

	return 0;
}


// Set up OpenGL. For viewport and projection setup see reshape(). */
void initOpenGL(int w, int h)
{
	// Set up and enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);   // This light is currently off
	//glEnable(GL_COLOR_MATERIAL);

	// Other OpenGL setup
	glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
	glShadeModel(GL_SMOOTH);
	
	// Use smooth shading, makes boundaries between polygons harder to see
	glClearColor(0.6F, 0.6F, 0.6F, 0.0F);  // Color and depth for glClear
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective
	glEnable(GL_TEXTURE_2D);

	makeTextureMapObjects();
	makeTexturesObjects();

	makeTextureMap();
	makeTextures();

	makeTextureMapEnemy();
	makeTexturesEnemy();

	makeTextureMapSub();
	makeTexturesSub();
	// Set up ground quad mesh
	Vector3D origin = NewVector3D(-60.0f, 0.0f, 60.0f);
	Vector3D dir1v = NewVector3D(8.0f, 0.0f, 0.0f);
	Vector3D dir2v = NewVector3D(0.0f, 0.0f, -8.0f);
	groundMesh = NewQuadMesh(meshSize);
	InitMeshQM(&groundMesh, meshSize, origin, 16.0, 16.0, dir1v, dir2v);

	Vector3D ambient = NewVector3D(0.0f, 0.05f, 0.0f);
	Vector3D diffuse = NewVector3D(0.4f, 0.8f, 0.4f);
	Vector3D specular = NewVector3D(0.04f, 0.04f, 0.04f);
	SetMaterialQM(&groundMesh, ambient, diffuse, specular, 0.2);

	Vector3D originO = NewVector3D(-60.0f, 0.0f, 60.0f);
	Vector3D dir1vO = NewVector3D(8.0f, 0.0f, 0.0f);
	Vector3D dir2vO = NewVector3D(0.0f, 0.0f, -8.0f);
	oceanMesh = NewQuadMesh(meshSize);
	InitMeshQM(&oceanMesh, meshSize, originO, 16.0, 16.0, dir1vO, dir2vO);
	
	Vector3D ambientO = NewVector3D(0.0f, 0.4f, 0.5f);
	Vector3D diffuseO = NewVector3D(0.7f, 0.7f, 0.7f);
	Vector3D specularO = NewVector3D(0.04f, 0.04f, 0.04f);
	SetMaterialQM(&oceanMesh, ambientO, diffuseO, specularO, 0.2);
	
	glEnable(GL_CULL_FACE);
	//Set(&groundMesh.vertices[36].position.y ,1,2,3);
	// Set up the bounding box of the scene
	// Currently unused. You could set up bounding boxes for your objects eventually.
	//Set(&BBox.min, -8.0f, 0.0, -8.0);
	//Set(&BBox.max, 8.0f, 6.0,  8.0);
	//getAllVertexHeights(groundMesh, 6);
	//makeMap();
	//Set(&groundMesh->vertices[1].position, meshpt.x, meshpt.y, meshpt.z);
	glClearColor(0 / 255.0, 119 / 255.0, 190 / 255.0, 0);
}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
	// Clear all pixels
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0 / 255.0, 119 / 255.0, 190 / 255.0, 0);
	glutPostRedisplay();
	// Set sub material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT, drone_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, drone_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, drone_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, drone_mat_shininess);

	//load matrix identity

	//We want to draw a sub consisting of one hull "body" spehere and, various fin rectangle, the tower rectangle, the
	//propeller rectangle. We will do this by first thinking about the body
	//rectangle in its own modeling (object) coordinate system and positioning the
	//propellers, fins, tower with respect to the default position of the body. Then eventually we position the periscope
	//with respect to the tower in later assignment
	//Then, we want to move the whole sub around in the world so we want any translations,
	//rotations  that are applied to the body to be "inherited" propellers, fins and tower (i.e.
	//these global transformations are also applied to the other parts)

	//Perform Modeling Transformations
	//Current transformation matrix is set to identity I
	//CTM = I
	glLoadIdentity();

	
	if (!periscopeMode)
	{
		gluLookAt(d * eyex, eyey, d * eyez, cos(theta), 6, -sin(theta), 0, 1, 0);

	}
	if (periscopeMode)
	{
		gluLookAt(
			(x+dx), dy+6.2, (y+dz), (dx + lx+x),dy+6.2 , (dz+ly+y), 0.0, 1, 0);
	}
	

	if (follow)
	{
		//follow = true;
		gluLookAt(bulletX, bulletY, bulletZ, 0, 0, 0, 0, 1, 0);
	}

	

	//glPushMatrix();
	//glTranslatef(dx,dy , dz);
	//glRotatef(subAngle, 0, 1, 0);
	//glTranslatef(dx, dy, dz-12);
	//glTranslatef(0,0, 0);
	//glutSolidSphere(1.05, 40, 4);
	//glPopMatrix();
	//for camera view, variables to change based on desired x/y/z axis
	if (modifyContinious)
	{
		glPushMatrix();
		glTranslatef(blobx, bloby, blobz);
		glScalef(1, 1, 1);
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		glEnable(GL_BLEND);
		//modifyContinious = false;
		glutSolidSphere(1, 20, 20);

	}
	if (!modifyContinious)
	{

		glDisable(GL_BLEND);
	}

	glPopMatrix();


	// Draw ground meshgl
	//Color3f(255,255,255);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDepthMask(GL_FALSE);
	if (value == 2)
	{
		arcCam = true;
		editMode = false;
		modifyMode = false;
		modifyContinious = false;

	}
	if (value == 3)
	{
		arcCam = false;
		editMode = true;
		modifyMode = false;
		modifyContinious = true;


	}
	if (value == 4)
	{
		arcCam = false;
		editMode = false;
		modifyMode = true;
		modifyContinious = false;

	}
	if (value == 5) {
		resetTerrain(&groundMesh);
	}
	if (value == 6)
	{
		printf("DISCLAIMR--- PROGRAM TAKES ~10 SECONDS TO LAUNCH BECUASE OF MY MAP LOADIG FUNCTION WHICH CREATES A PRE-LOADED MAP WITH FEATURES IN FLOOR MESH\n");
		printf("===============================HELPER FUNCTIONS============================ \n");
		printf("MY PROGRAM IS MADE SO THAT YOU TOGGLE WHICH MODE YOU WOULD LIKE (ARCBALL,EDITMODE,MODIFYBLOBMODE \n");
		printf("+++++++++++++++++++++++++++++++++++++++++++++ARCBALL MODE[F2]+++++++++++++++++++++++++++++++++++ \n");
		printf("------------------------------------------------------------------------------------------------- \n");
		printf("IN THIS MODE YOU ARE ABLE TO MOVE THE CAMERA AROPUND AN IMAGINAY DOME LIKE OBJECT WITH \n");
		printf("RESTRICTIONS TO HOW HIGH OR LOW(Y VAL) \n");
		printf("LEFT MOUSE DOWN IS TO CONTROL XZ ORBIT AND MIDDLE MOUSE DOWN IS Y ORBIT, CURSOR UP/DOWN IS ZOOM IN/OUT \n");
		printf("RIGHT MOUSE DOWEN IS TO CONTROL Y ORBIT. IN THIS MODE ONCE YOU PLACE A BLOB YOU CAN UNDO MULIPLE BLOBS WITH KEY 'U'\n");
		printf("OR REDO MULTIPLE BLOBS WITH 'R' OR UNDO MOST RECENT WITH 'Y' \n");
		printf("\n");
		printf("\n");
		printf("+++++++++++++++++++++++++++++++++++EDIT TERRAIN MODE[F3]++++++++++++++++++++++++++++++++++++++++++ \n");
		printf("------------------------------------------------------------------------------------------------- \n");
		printf("IN THIS MODE YOU ARE ABLE TO MOVE BLOB WITH KEYBOARD  AND USE 'T' TO PLACE A BLOB IN THIS MODE \n");
		printf("'t' -> PLACE BLOB\n");
		printf("BLOB XZ LOCATION AND PLACEMENT IN EDIT MODE \n");
		printf("YOU ARE ALSO ABLE TO USE MOUSE TO EDIT TERRAIN WHICH WORKS BY LEFT MOUSE CLICK IN THIS MODE\n");
		printf("ALSO IN THIS MODE YOU ARE ABLE TO UNDO A CHIAN OF PLACED BLOBS WITH CONTINIOUS 'U' OR CONTINIOUS REDO WITH 'R' \n");
		printf("OR ALSO THE ABILITY TO JUST INDO THE MOST RECENT PLACED BLOB WITH 'Y' \n");
		printf("\n");
		printf("\n");
		printf("++++++++++++++++++++++++++++++++++++++++++++++MODIFY BLOB MODE[F4]++++++++++++++++++++++++++++++++++++++++++ \n");
		printf("IN THIS MODE YOU ARE ABLE TO EDIT ATTRIBUTES OF BLOB LIKE A VALUE, B VALUE WHICH IN TURN MODIFIES WIDTH, HEIGHT,\n");
		printf("MAKE BLOB TALLER: UP KEY IN THIS MODE  \n");
		printf("MAKE BLOB SMALLER: DOWN KEY IN THIS MODE (NEGATIVE B VALUE TO MAKE VALLEYS) \n");
		printf("MAKE BLOB WIDER: RIGHT KEY IN THIS MODE \n");
		printf("MAKE BLOB THINNER: LEFT KEY IN THIS MODE \n");
		printf("+++++++++++++++++++++++++++++++++++RESET TERRAIN[F5]++++++++++++++++++++++++++++++++++++++++++ \n");
		printf("------------------------------------------------------------------------------------------------- \n");
		printf("+++++++++++++++++++++++++++++++++++PERSICOPE MODE[F6]++++++++++++++++++++++++++++++++++++++++++ \n");
		printf("------------------------------------------------------------------------------------------------- \n");
		printf("IN THIS MODE YOU SWITCH YOUR GLU LOOK AT TO VIEW OCEAN FROM PERISCOPE IN WHICH YOU ARE ABLE TO\n");
		printf("LOOK AROUND FREELY 360 DEGREES AND ALSO A DEPTH-OF-FIELD EFFECT TO MIMIC BEING UNDERWATER AND MOVING\n");
		printf("YOU CAN USE CURSOPR TO ZOOM AND ALSO IF SHADER IS SWITCHED OFF(PRESS F9) YOU ALSO SEE A LENS EFFECT\n");
		printf("OF THE PERISCOPE USING BLENDING AND SUCH\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("=================================OTHER KEYBOARD INPUTS===============================================\n");
		printf("SPACEBAR: IS TO SHHOT TORPEDO\n");
		printf("TAB: IS TO TOGGLE TRAJECTORY HELPER\n");
		printf("K: TO TOGGLE FOLLOW/HIGHLIGHT WHERE BULLET IS GOING\n");
		printf("P: TO TOGGLE PERISCOPE ANIMATION UP/DOWN (AUTOMATICALLY COMES UP IN PERSICOPE MODE\n");
		printf("AWSD: MOVE SUB\n");
		printf("G: TO RESET SUB LOCATION AD ATTRIBUTES AKA HEALT, ANGLES, ETC TO DEFAULT (ALSO ACTIVATED IF HEALTH BELOW -50\n");
		printf("F AND E: ASCEND/DESEND SUB\n");
		printf("q: START AND INCREASE ENGINE ROTORS\n");
		printf("IF EDIT MODE: T TO PLACE BLOB, R TO REDO, U TO UNDO\n");
		printf("\n");
		printf("||||||||||||||||||||||||||||||||||ADDICTIONAL NOTES||||||||||||||||||||||||||||||||||||||||||||||||\n");
		printf("IF YOU CRASH, YOUR HEALTH DEPLETS ACCORDINGLY AND IF U KEEP TRYING TO GO DOWN, IT TOGGLES FORCE RESET\n");
		printf("YOU START WITH 100 HP AND DESCENDING WHILE CRASHED DEPLETS HEALTH BY 1 EACH PRESS OF KEY=F\n");
		printf("@ 0 HP SUB DISAPEARS AND MSG APPEARS @ -50 FORCE RESETS\n");
		printf("CRASHING MAKES SCREEN BLINK RED, HITTING OCEAN FLOOR OBJECTS OR ENEMY FLASHED GREEN(A HIT)\n");
		printf("SCORE SYSTEM:::: YOUR SCORE INCEARSES EACH TIME BULLET HITS ENEMY SUB OR FLOOR OBJECTS, SCORE\n");
		printf("DECREASES IF CRASHED.\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("To close the program sub window, press EXIT \n");
		printf("++++++++++++++++++++++++++++++++++++++++++++++++++++[BONUS]++++++++++++++++++++++++++++++++++++++++++ \n");
		printf("ADDICTIONAL BONUS/MISCELLANEOUS FEATURES\n");
		printf("1-> AN OCEAN TOP WHICH IS ANIMATED TO SIMULATE PSEDO-WATER FLUID PHYSICS \n");
		printf("2-> SCORE SYSTEM/HEALTH SYSYEM  \n");
		printf("3-> A SEMI-ADVANCED SHADER PROGRAM WHICH DYNAMICALLY DIFFUSES LIGHT ON TOP OF TEXTURES WITH SHINIENESS  \n");
		printf("4-> CAN TURN OFF SHADER ONCE USING KEY=F9 WHICH THEN ALLOWS YOU TO SEE A SIMUALTED LENS EFFECT IN PERISCOPE MODE KEY=F6  \n");
		printf("5-> GREEN/RED COLOR FLASH UPON SCORING OR CRASHING  \n");
		printf("6->   \n");
		printf("6-> REDO FEATURE USING STRUCT ARAYS POINTER AND INCREMENTER  \n");
		printf("7-> TRANSPARENT OCEAN FLOOR TO ALLOW TO SEEING CIRCLE FOR EDITING AND ALSO MIMICS UNDERWATER  \n");
		printf("8-> A MENU WHICH CAN BE ACTIVATED WITH MIDDLE MOUSE BUTTON(HELD DOWN)  \n");

	}
	//draw the wub
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, texSub[0]);
	drawSubmarine();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);


	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, texEnemy[0]);
	drawEnemySub();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	
	
	
	
	//enemyPathing();
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, texObjects[0]);
	drawArtifactUp();
	drawArtifactFallen();	
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	//draw the static artifacts on ocean floor
	

	

	
	//bool xMaxed = false;
	//bool xmax = false;

	
	if (periscopeMode)
	{
		glPushMatrix();
		glColor4f(0.0f, 0.0f, 0.0f, 0.1f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_ZERO);

		glTranslatef(dx, dy+6, dz);
		glRotatef(subAngle, 0, 1, 0.0);
		glRotatef(ly, 0, 1, 0.0);
		//glutSolidSphere(0.8, 50, 50);

		glutSolidTorus(0.7, 1.5, 10, 50);
		glDisable(GL_BLEND);	

		glPopMatrix();

		glPushMatrix();	
		glColor4f(0.0f, 0.0f, 0.0f, 0.1f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);

		glTranslatef(dx, dy + 6, dz);
		//glScalef(2, 1, 2);
		glutSolidSphere(0.7, 50, 50);
		glDisable(GL_BLEND);

		glPopMatrix();
	}
	
		
	
	//xMaxed = !xMaxed;
	
	//xMaxed = false;

	if (fireTorpedo && reloaded)
	{
		//updateTime();
		

		glPushMatrix();

		glTranslatef(bulletX, bulletY+2, bulletZ);

		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glBindTexture(GL_TEXTURE_2D, texEnemy[0]);

		glutSolidSphere(0.5, 50.0, 50.0);

		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);

		glPopMatrix();
		//bulletTimer++;

	}
	//fireTorpedo = false;
	firingMode = true;
	//reloaded = false;
	//glPushMatrix();


	
	//glPopMatrix();
	//            Draw ground mesh

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	DrawMeshQM(&groundMesh, meshSize);

	//glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	glEnable(GL_BLEND);
	if (!editMode)
	{
		//glBindTexture(GL_TEXTURE_2D,0);

		DrawMeshQMOcean(&oceanMesh, meshSize);

	}

	glPopMatrix();


	//updateTime();
	xRev = !xRev;
	glutSwapBuffers();   // Double buffering, swap buffers
}

//draw artifacts
void drawArtifactUp()
{
	glPushMatrix();
	glTranslatef(-7, 0, -15);
	glScalef(5, 1, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7, 5, -15);
	glScalef(5, 1, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, 2, -15);
	glScalef(1, 5, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7, 2, -15);
	glScalef(1, 5, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-9, 2, -15);
	glScalef(1, 5, 1);
	glutSolidCube(1.0);
	glPopMatrix();
	
}

//draw artifacts: fallen
void drawArtifactFallen()
{
	glPushMatrix();
	glTranslatef(17, 0, 4);
	glRotatef(90, 1, 1, 0.0);
	glScalef(5, 1, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(13, 2, 4);
	glRotatef(60, 1, 1, 0.0);
	glScalef(5, 1, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15, 2, 4);
	glRotatef(90, 1, 1, 0.0);
	glScalef(1, 5, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(17, 2, 4);
	glRotatef(100, 1, 1, 0.0);
	glScalef(1, 5, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(19, 2, 4);
	glRotatef(80, 1, 1, 0.0);
	glScalef(1, 5, 1);
	glutSolidCube(1.0);
	glPopMatrix();

}


//draw the sub
void drawSubmarine()
{
	glPushMatrix();

	// T1: Translate so that the center of the hull body is at center of clip rectangle.
	// This is the last thing that will happen (and therefore the first function that is called).
	// We want to apply this to propellers, fins, and towers too (the propellers, fins, and towers should inherit this translation).
	// CTM = I * T1
	glTranslatef(dx, dy + 2, dz);

	// R1: Rotate in relation to hull : the center of the hull body rectangle
	// we will want to rotate the propellers, fins, and towers by this much as well

	// CTM = I * T1 * R1

	if (gettingDamaged && floorCol)
	{
		
		glRotatef(-90, 0.0, 1, 0.0);

		glRotatef(randTheta0, randx0, randy0, 0);
		glRotatef(randTheta1, randx1, randy1, 1);
		glRotatef(randTheta2, randx2, randy2, 0);
		glRotatef(randTheta3, randx3, randy3, 1);

		//dx = 0;
		dy = -0.08;
		//dz = 0;
	}
	if (gettingDamaged && debrisCol)
	{
		

		glRotatef(-90, 0.0, 1, 0.0);

		glRotatef(randTheta0, randx0, randy0, 0);
		glRotatef(randTheta1, randx1, randy1, 1);
		glRotatef(randTheta2, randx2, randy2, 0);
		glRotatef(randTheta3, randx3, randy3, 1);

		
		dy = -0.08;
		
	}
	if (!gettingDamaged)
	{
		glRotatef(-90, 0.0, 1, 0.0);




		glRotatef(subAngle, 0.0, 1, 0.0);
		glRotatef(subAngle1, 0.0, 0, 1);


	}
	if (!crashed)
	{
		drawHull();
		drawTower();
		drawTowerFin();
		drawAntenna();
		drawPropeller1();
		drawPropeller2();
		drawFin1();
		drawFin2();
		drawPeriscope();


	}
	if (bulletHelp)
	{
		for (int i = 0; i < 100; i++) {
			glPushMatrix();

			glTranslatef(-i, 0, 1);

			glutSolidSphere(0.2, 50.0, 50.0);

			glPopMatrix();
		}
	}
	
	glPopMatrix();

	
}
float getEnemyAngle(float dx, float dy, float dz, float dx1, float dy1, float dz1)
{
	 Vector3D playerPos = NewVector3D(dx, dy, dz);
	 Vector3D enemyPos = NewVector3D(dx1, dy1, dz1);
	 Normalize(&playerPos);
	 Normalize(&enemyPos);
	return 	acos(DotProduct(&playerPos, &enemyPos));
}

void animationHandlerPathing(int param)
{
	srand(time(NULL));

	counter++;
	//printf("%f\n", enemyAngle);
	//enemyX = sin(3.14 * enemyAngle / 180) * 1;
	//enemyZ = cos(3.14 * enemyAngle / 180) * 1;
	if (xFor || xRev) {
		// Going down

		enemyX += sin(3.14 * enemyAngle / 180) * 0.01;
		enemyZ += cos(3.14 * enemyAngle / 180) * 0.01;
		if (enemyX > 30 || enemyX < -30)
		{
			xFor = false;

			enemyAngle = (rand() % 120 + 1);
		}

	}
	else if (!xFor || !xRev) {
		// Going up

		enemyX -= sin(3.14 * enemyAngle / 180) * 0.01;
		enemyZ -= cos(3.14 * enemyAngle / 180) * 0.01;

		if (enemyZ > 30 || enemyZ < -30)
		{
			xFor = true;
			//srand(time(NULL));

			enemyAngle = (rand() % 1 + -120);
		}

	}
	float enemyDist = sqrt(pow(enemyX - 0, 2) +
		pow(enemyZ - 0, 2) * 1.0);

	if ((int)counter%1000 == 0 || enemyDist >60)
	{
		enemyX = enemyZ = dx+10;
		enemyY = rand() % 8 + 7;
		enemyAngle = (rand() % 45 + 1);
		enemyX += sin(3.14 * enemyAngle / 180) * 0.01;
		enemyZ += cos(3.14 * enemyAngle / 180) * 0.01;
		//printf("%f\n", enemyY);

		//srand(time(NULL));
		enemyAngle = (rand() % 90 + 1);
		xFor = xRev = true;	


	}
	//xmax = false;
	glutPostRedisplay();
	glutTimerFunc(10, animationHandlerPathing, 0);
	
} 

void enemyPathing()
{
	//bool xMaxed = false;
	//bool xmax = false;

	if (enemyX < 10 && enemyX >= 0)
	{
		enemyX += 0.5;

	}
	if (enemyX > -10 && enemyX < 0)
	{
		enemyX -= 0.5; 
	}
	

}
void drawEnemySub()
{
	
	

	glPushMatrix();

	// T1: Translate so that the center of the hull body is at center of clip rectangle.
	// This is the last thing that will happen (and therefore the first function that is called).
	// We want to apply this to propellers, fins, and towers too (the propellers, fins, and towers should inherit this translation).
	// CTM = I * T1
	glTranslatef(enemyX, enemyY, enemyZ);

	// R1: Rotate in relation to hull : the center of the hull body rectangle
	// we will want to rotate the propellers, fins, and towers by this much as well

	// CTM = I * T1 * R1

	
	//float enemyTheta = tan(dx/dz);
		//getEnemyAngle(dx, dy, dz, 1, 1, 1) * 180/3.14;
	
		/*glRotatef(-90, 0.0, 1, 0.0);
		glRotatef(enemyTheta * 180 / 3.14, 0.0, 1, 0.0);
		glRotatef(subAngle1, 0.0, 0, 1);
		//printf("x, %f \n", enemyTheta * 180 / 3.14);*/
		
		glRotatef(-90, 0.0, 1, 0.0);




		glRotatef(enemyAngle, 0.0, 1, 0.0); 
		glRotatef(0, 0.0, 0, 1);
	
		
		drawHull();
		drawTower();
		drawTowerFin();
		drawAntenna();
		drawPropeller1();
		drawPropeller2();
		drawFin1();
		drawFin2();
		
	glPopMatrix();

	
}


//draw the hull body
void drawHull()
{
	//Now we begin by drawing the hull body. We are starting with a sphere.
	//S1: scale hull body sphere to desired size
	//We will surround the drawing  of the hull body base with glPushMatrix()/glPopMatrix()
	//glPushMatrix will *copy* the current CTM (which is currently I*T1*R1) and
	//place the copy on the stack
	//CTM = I * T1 * R1
	glPushMatrix();

	//Next, we scale the hull base to the desired size
	//CTM = I * T1 * R1 * S1
	glScalef(submarineWidth, submarineHeight, submarineDepth);

	//Now draw the hull body sphere
	//Each new point p' will be calculated by opengl by multiplying old point p by
	//current transformation matrix:
	//p' = CTM * p
	//p' = I * T1 * R1 * S1 * p
	//Scale S1 will be done first, then R1, then T1 etc
	glutSolidSphere(2.0, 50.0, 50.0);

	glPopMatrix();
}

//draw the first propeller
void drawPropeller1()
{
	// Now we transform and draw the propeller rectangle
	// We want the global transformations that were done to the hull body to be inherited
	// by propeller as well. When we finish the propeller scales, rotates etc we want to
	// restore the global transformations so that they can be applied to the other propeller.
	// In other words, any specific transformation we apply to the first propeller must be
	// isolated from the other propeller (opengl is a big state machine)
	// So, once again save the current CTM (I*T1*R1) (the global transformations),
	glPushMatrix();

	// T2: translate the propeller so that it is on the end of the hull body
	// at the origin of its own local (called "modeling") coordinate system
	// We are performing this translation with respect to the local coordinate system
	// of the hull body (we know the size of the hull body using the global variables).
	glTranslatef(11.5, 0, 0.0);

	// R2: rotate propeller rectangle
	// this is the angle it will make with respect to the base sphere (hull body)
	glRotatef(rotorAngle, 1, 0.0, 0);

	// S2 : scale the propeller rectangle
	// Make the propeller proportional to hull body
	glScalef(0.15, 2.0, 0.25);

	// Draw the propeller
	// Each new point p' will be given by multiplying old point p by
	// composite transformation matrix:
	// p' = I * T1 * R1 * T2 * R2 * S2 * p
	// That is, all the transformations we did to the hull body sphere (I*T1*R1) will
	// also be applied to the propeller rectangle,(T3*R2*T4*S2). This will cause the propeller rectangle to
	// rotate along with the hull body, keeping the same relative angle between them
	glutSolidCube(1.0);

	// Now we pop the matrix from the matrix stack. This will become the new CTM
	// Remember we saved the transformations that were done to the body rectangle
	// so the new CTM once again becomes I*T1*R1
	glPopMatrix();
}

//draw the second propeller
void drawPropeller2()
{

	// Now we repeat for the other propeller. We change the translation and the rotation
	// to be specific to this propeller, which is 90 degrees to the first.
	glPushMatrix();

	glTranslatef(11.5, 0, 0.0);
	glRotatef(rotorAngle + 90, 1, 0.0, 0);
	glScalef(0.15, 2.0, 0.25);

	glutSolidCube(1.0);
	glPopMatrix();

}
//draw the tower on top of hull body
void drawTower()
{
	// Now we transform and draw the propeller rectangle
	// We want the global transformations that were done to the hull body to be inherited
	// by tower as well. So once again save the current CTM (I*T1*R1) (the global transformations),
	glPushMatrix();

	// T2: translate the tower so that it is on the middle-top of the hull body
	// at the origin of its own local (called "modeling") coordinate system
	// We are performing this translation with respect to the local coordinate system
	// of the hull body (we know the size of the hull body using the global variables).
	glTranslatef(-3, 1.5, 0.0);

	// R2: rotate tower rectangle
	// this is the angle it will make with respect to the base sphere (hull body)
	glRotatef(90.0, 0.0, 1.0, 0.0);

	// S2 : scale the tower rectangle
	// Make the tower proportional to hull body
	glScalef(submarineWidth - 5, submarineHeight * 2.5, submarineDepth * 8.5);

	// Draw the tower
	// Each new point p' will be given by multiplying old point p by
	// composite transformation matrix:
	// p' = I * T1 * R1 * T2 * R2 * S2 * p
	// That is, all the transformations we did to the hull body sphere (I*T1*R1) will
	// also be applied to the propeller rectangle,(T3*R2*T4*S2). This will cause the propeller rectangle to
	// rotate along with the hull body, keeping the same relative angle between them
	glutSolidCube(1.0);

	// Now we pop the matrix from the matrix stack. This will become the new CTM
	// Remember we saved the transformations that were done to the body rectangle
	// so the new CTM once again becomes I*T1*R1
	glPopMatrix();

}

void drawPeriscope()
{
	glPushMatrix();
	glTranslatef(0, periscopeY-1, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.2,3,0.2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1, periscopeY +0.5, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.2, 0.2);
	glutSolidCube(2);
	glPopMatrix();

	

}
/////////////////////////////////SAME GENERAL PROCESS OF HOW MODEL VIEW MATRIX IS BEING CONSTRUCTED////////////////////////////////////
//draw the tower fin
void drawTowerFin()
{
	glPushMatrix();
	glTranslatef(-3, 2.3, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(3, 0.1, 0.5);
	glutSolidCube(1.0);
	glPopMatrix();
}


	

//draw the antennas on top tower
void drawAntenna()
{
	
	
	glPushMatrix();
	glTranslatef(-1.5, 2.8, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.05, 2, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.9, 3, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.05, 2, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.7, 2.4, 0.2);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.05, 2, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 2.8, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.3, 0.3, 0.3);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.3, 2.8, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.6, 0.6, 0.6);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.8, 2.1, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.8, 0.8, 0.8);
	glutSolidSphere(1.05, 40, 4);
	glPopMatrix();

	
}



//draw the first fin
void drawFin1()
{
	glPushMatrix();
	glTranslatef(10, 0, 0.0);

	glRotatef(finAngle1, 0, 0, fin1z);

	glScalef(1, 0.05, 4);
	glutSolidCube(1.0);
	glPopMatrix();

}

//draw the second fin
void drawFin2()
{
	glPushMatrix();
	glTranslatef(10, 0, 0.0);


	glRotatef(finAngle2, fin2x, 0, 0);
	glRotatef(finAngle2R, 0, 0, fin2z);

	glScalef(1, 0.05, 4);
	glutSolidCube(1.0);
	glPopMatrix();

}


// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
	portwidth = w;
	portheight = h;
	// Set up viewport, projection, then change to modelview matrix mode -
	// display function will then set up camera and do modeling transforms.
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (GLdouble)w / h, 0.2, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
}

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
	

	case 'k':
	{
		follow = !follow;
		printf("heh");
		break;
	}
	case 'p':
	{
		
		scopeUp = !scopeUp;
		break;
	}
		//move sub forward
	case 'w':
	{
		//printf("%f\n", dx);
		subAngle1 = 0.0;
		finAngle1 = 0;
		finAngle2R = 0;
		fin1x = 1;
		fin1z = 1;
		if (periscopeMode)
		{
			fov = 150;
			reshape(vWidth, vHeight);

		}
		if (!gettingDamaged)
		{			
			dx -= sin(3.14 * subAngle / 180) * 0.23;
			dz -= cos(3.14 * subAngle / 180) * 0.23;
			//deltaMove += 1.0;
		}
		

		//dxx -= sin(3.14 * subAngle / 180) * 1;
		//dzz -= (cos(3.14 * subAngle / 180) )* (1);

		//printf("%f\n", dx);
		//printf("%f\n", dz);
		float d = sqrt(pow(-7 - dx, 2) +
			pow(-15 - dz, 2) * 1.0);

		float df = sqrt(pow(17 - dx, 2) + 
			pow(4 - dz, 2) * 1.0);
		//printf("%f\n", d);
		//printf("%f\n", dzz);
		//printf("\n");

		if (((df > 0  && df <7) || ((dx > 14 && dx < 21) && (dz > 0 && dz < 5.5))) && dy < 3.2 ) {
			gettingDamaged = true;
			debrisCol = true;
			
			crashCheck++;
			if (crashCheck == 1)
			{
				glClearColor(1, 0, 0, 1);
				glutPostRedisplay();

				score--;
				printf("Oh no! you've crashed into an object, your score is now:    %f \n", score);
			}

		}
		
		if (((d > 0 && d < 4) || ((dx > -10.3 && dx < -3.4) && (dz > -16 && dz < -14))) && dy < 3.2) {
			gettingDamaged = true;
			debrisCol = true;

			crashCheck++;
			if (crashCheck == 1)
			{
				glClearColor(1, 0, 0, 1);
				glutPostRedisplay();
				score--;
				printf("Oh no! you've crashed into an object, your score is now:    %f \n", score);
			}
			
			
		}
		

		//		if ((dx > 11 && dx < 24 && dz > 0 && dz < 6.5 && dy < 3.2) || (dx > -10.3 && dx < -3.4 && dz > -16 && dz < -14))

		//

		reverse = false;
		//glutTimerFunc(1, animationHandler, 2);
		break;
	}

	//move sub backward
	case 's':
	{
		if (periscopeMode)
		{
			fov = 150;
			reshape(vWidth, vHeight);

		}
		if (!gettingDamaged)
		{
			//deltaMove += -1.0;
			periscopeZoom = 1;

			
			dx += sin(3.14 * subAngle / 180) * 1;
			dz += cos(3.14 * subAngle / 180) * 1;
		}
		//dxx += sin(3.14 * subAngle / 180) * 1;
		//dzz += (cos(3.14 * subAngle / 180) )* (1);


		
		//glutTimerFunc(1, animationHandler, 0);
		break;
	}

	//start engine
	case 'q':
	{

		slowing = false;
		glutTimerFunc(1, animationHandler, 0);

		break;
	}


	//move sub up (ascend)
	case 'e':
	{
		
		if (!crashed && !debrisCol)
		{
			finAngle2R = 0;
			if (!periscopeMode)
			{
				subAngle1 = -4;

			}
			finAngle1 = 10;
			if (dy < 11.7)
			{
				dy += 0.08;

			}
			//dy += 0.08;

		}
		
		break;
	}
	case 'g':
	{
		crashCheck = 0;
		debrisCol = false;
		floorCol = false;
		firingMode = false;
		finAngle2R = 0;
		subAngle1 = 0;
		finAngle1 = 0;
		gettingDamaged = false;
		crashed = false;
		health = 100;
		dx = dy = dz = 2;
		subAngle = 0;
		glutPostRedisplay();
		break;
	}
	//move sub down (descend)
	case 'f':
	{
		
		if (!crashed && !debrisCol)
		{
			finAngle2R = 0;

			if (!periscopeMode)
			{
				subAngle1 = 4;

			}			finAngle1 = -10;
			dy -= 0.08;

		
		}
		if (terrainCollide(&groundMesh,dy,dx,dz) && !debrisCol) {
			if (health == 100)
			{
				glClearColor(0.5, 0, 0, 1);
				glutPostRedisplay();

				score--;
				printf("Yikes!you've been grounded captain 0.0 !, your score is now:    %f \n", score);
			}
			//printf("Yikes");
			health -= 1;
			gettingDamaged = true;
			floorCol = true;
			if (health >= 0)
			{
				glClearColor(0.5, 0, 0, 1);
				glutPostRedisplay();

				printf("We are taking damage the more you try to descend captain 0.o! HEALTH:  %i\n", health);

			}
			if (health < 0)
			{
				glClearColor(0, 0, 0, 1);
				glutPostRedisplay();

				printf("%i\n", health);
			}
		}

		

		if (health == 0)
		{
			glClearColor(0, 0, 0, 1);
			glutPostRedisplay();

			printf("Your sub marine is totaled, continue force descent to restart or force-restart with key 'g' ^-^ %i\n", health);
			 srand(time(NULL));

		    randTheta0 = rand() % 40 + 1;
			 randTheta1 = rand() % 10 + 1;
			 randTheta2 = rand() % 1 + -40;
			 randTheta3 = rand() % 1 + -10;
			 randx0 = (rand() % 3 + 1) % 2;
			 randx1 = (rand() % 3 + 1) % 2;
			 randx2 = (rand() % 3 + 1) % 2;
			 randx3 = (rand() % 3 + 1) % 2;
			 randy0 = (rand() % 3 + 1) % 2;
			 randy1 = (rand() % 3 + 1) % 2;
			 randy2 = (rand() % 3 + 1) % 2;
			 randy3 = (rand() % 3 + 1) % 2;
			 
			crashed = true;
			
			//gettingDamaged = false;
			//crashed = false;
			//health = 3;
		}
		if (health == -50)
		{
			glClearColor(0, 1, 0, 1);
			glutPostRedisplay();

			printf("Submarine is fully repaired and fully functional, lets try to keep it that way this time ... -.- %i\n", health);
			finAngle2R = 0;
			subAngle1 = 0;
			finAngle1 = 0;
			gettingDamaged = false;
			crashed = false;
			health = 100;
			crashCheck = 0;

		}
		break;
	}

	//move sub left
	case 'a':
	{
		
		periscopeZoom = 1;

		finAngle1 = 0;

		finAngle2R = 10;
		fin2z = 1;
		subAngle += 0.6;
		//printf("%f\n", subAngle);
		lx = cos(7.87+subAngle*0.017 );
		ly = -sin(7.87+subAngle*0.017);
		//dxx = sin(3.14 * subAngle / 180) * 1;
		//dzz = cos(3.14 * subAngle / 180) * 1;
		//dxx = dxx * sin(subAngle) - dzz * cos(subAngle);
		//dzz = dxx * cos(subAngle) + dzz * sin(subAngle);

		break;
	}
	//case 'o': {
	//	periscopeZoom = periscopeZoom - 0.5;
	//	glMatrixMode(GL_PROJECTION); // You had GL_MODELVIEW
	//	glOrtho(-1.5 + periscopeZoom, 1.0 - periscopeZoom, -8.0 + periscopeZoom, 0.5 , -5.0, 3.5); // Changed some of the signs here
	//	break;
	//}
	//move sub right
	case 'd':
	{
		
		periscopeZoom = 1;

		finAngle1 = 0;

		finAngle2R = -10;
		fin2z = 1;
		subAngle -=0.6;
		//printf("%f\n", subAngle);
		lx = -sin(3.2 + subAngle * -0.017);
		ly = cos(3.2 + subAngle * -0.017);
		//dxx = sin(3.14 * subAngle / 180) * 1;
		//dzz = cos(3.14 * subAngle / 180) * 1;
		//if ((dx > 11 && dx < 22 && dz > 0 && dz < 6.5 && dyy < 4.2) || (dx > -10.3 && dx < -3.4 && dz > -16 && dz < -14)) {
		//	crashed = true;
		//}
		//else
		//{
		//	crashed = false;
		//}

		break;
	}

	}
	if (editMode)
	{
		switch (key)
		{
		case 't':
			changeY(&groundMesh, blobx, blobz, blobB, blobA);
			break;
			//changeY(&groundMesh, 14, 2, 2, 2);

		case 'u':
			undoY(&groundMesh);


			break;
		case 'r':
			redoY(&groundMesh);


			break;
		}

	}
	if (undo)
	{
		switch (key)
		{


		case 'y':
			undo = false;
			undoY(&groundMesh, oldX, oldZ, oldB, oldA);
			break;

		}

	}
	if (periscopeMode)
	{
		switch (key)
		{


		case '1':
		{
			subAngle +=  angle;
			break;
		}
		}

		}
	
	if (key == 32)
	{	
		//reloaded = true;
		fireTorpedo = true;
		firingMode = true;
		bulletX = dx;
		bulletY = dy;
		bulletZ = dz;
		bulletAngle = subAngle;
		//follow = true;
		//Sleep(3000);
	}
	//exit game
	if (key == 27)
	{
		exit(0);
	}
	if (key == 9) {
		bulletHelp = !bulletHelp;
		subAngle1 = 0;
		finAngle1 = 0;
		finAngle2R = 0;
		fin1x = 1;
		fin1z = 1;
	}



	glutPostRedisplay();   // Trigger a window redisplay
}
void updateTime()
{
	totalTime += 1.0 / 60.0; //increase time

	if (totalTime > 10)
	{
		reloaded = true;
		printf("%f\n", 10.0);

	}
	if (totalTime < 10 && !fireTorpedo)
	{
		reloaded = false;
		printf("%f\n", 0.0);
	}
	
		

	//if (totalTime > 10.0)
	//{
		//totalTime = 0.0;
	//}
	// 3.
	

}

void animationHandlerWaves(int param)
{
	//glDisable(GL_TEXTURE_2D);


	waves(&oceanMesh);

	glutPostRedisplay();
	glutTimerFunc(1000, animationHandlerWaves, 0);
}

void animationHandlerPeriscope(int param)
{
	if (scopeUp && periscopeY < 4)
	{
		periscopeY += 0.035;
	}

	if (!scopeUp && periscopeY > 0)
	{
		periscopeY -= 0.035;
	}
	
	
	glutPostRedisplay();
	glutTimerFunc(10, animationHandlerPeriscope, 0);
}

void animationHandlerBullet(int param)
{
	//updateTime();

	if (fireTorpedo && firingMode)
	{
		//printf("hi");

		bulletX -= sin(3.14 * bulletAngle / 180) * 0.4;
		bulletZ -= cos(3.14 * bulletAngle / 180) * 0.4;

		if ((bulletX > 11 && bulletX < 22 && bulletZ > 0 && bulletZ < 6.5 && dy < 4.2) || (bulletX > -10.3 && bulletX < -3.4 && bulletZ > -16 && bulletZ < -14 && dy < 5.6))
		{
			fireTorpedo = false;
			reloaded = true;
			//glColor3f(0, 1, 0);
				

			glClearColor(0, 0.7,0, 1);
			glutPostRedisplay();

			score++;
			printf("Nice shot, your score is now:    %f \n", score);

			//totalTime = 0.0;
		}

		if (bulletX > enemyX -2 && bulletX < enemyX +2 && bulletZ > enemyZ -11 && bulletZ < enemyZ + 11 && bulletY > enemyY - 2.5 && bulletY < enemyY +2.5)
		{
			fireTorpedo = false;
			reloaded = true;
			enemyX = enemyZ = 30;

			glClearColor(0, 0.7, 0, 1);
			glutPostRedisplay();

			score++;
			printf("Enemy destroyed, score:  %f\n", score);
			//totalTime = 0.0;
		}
	}
	float euDisSub = sqrt(pow(dx - bulletX, 2) +
		pow(dz - bulletZ, 2) * 1.0);
	
	if (euDisSub > 100 || !fireTorpedo)
	{
		fireTorpedo = false;
		follow = false;
		//totalTime = 0.0;
	}
	glutPostRedisplay();
	glutTimerFunc(10, animationHandlerBullet, 0);
}

//animation handler to make animations
void animationHandler(int param)
{
	
	
	if (!slowing)
	{
		rotorAngle += 0.2;
	}
	if (!reverse)
	{
		rotorAngle += 0.01;
	}
	if (reverse)
	{
		rotorAngle += -0.3;
	}

	


	//fireTorpedo = false;
	glutPostRedisplay();
	glutTimerFunc(10, animationHandler, 0);
}

//idle function
void idleFunction()
{
	animationHandler(0);
}
// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
	// Help key
	if (key == GLUT_KEY_F1)
	{
		printf("DISCLAIMR--- PROGRAM TAKES ~10 SECONDS TO LAUNCH BECUASE OF MY MAP LOADIG FUNCTION WHICH CREATES A PRE-LOADED MAP WITH FEATURES IN FLOOR MESH\n");
		printf("===============================HELPER FUNCTIONS============================ \n");
		printf("MY PROGRAM IS MADE SO THAT YOU TOGGLE WHICH MODE YOU WOULD LIKE (ARCBALL,EDITMODE,MODIFYBLOBMODE \n");
		printf("+++++++++++++++++++++++++++++++++++++++++++++ARCBALL MODE[F2]+++++++++++++++++++++++++++++++++++ \n");
		printf("------------------------------------------------------------------------------------------------- \n");
		printf("IN THIS MODE YOU ARE ABLE TO MOVE THE CAMERA AROPUND AN IMAGINAY DOME LIKE OBJECT WITH \n");
		printf("RESTRICTIONS TO HOW HIGH OR LOW(Y VAL) \n");
		printf("LEFT MOUSE DOWN IS TO CONTROL XZ ORBIT AND MIDDLE MOUSE DOWN IS Y ORBIT, CURSOR UP/DOWN IS ZOOM IN/OUT \n");
		printf("RIGHT MOUSE DOWEN IS TO CONTROL Y ORBIT. IN THIS MODE ONCE YOU PLACE A BLOB YOU CAN UNDO MULIPLE BLOBS WITH KEY 'U'\n");
		printf("OR REDO MULTIPLE BLOBS WITH 'R' OR UNDO MOST RECENT WITH 'Y' \n");
		printf("\n");
		printf("\n");
		printf("+++++++++++++++++++++++++++++++++++EDIT TERRAIN MODE[F3]++++++++++++++++++++++++++++++++++++++++++ \n");
		printf("------------------------------------------------------------------------------------------------- \n");
		printf("IN THIS MODE YOU ARE ABLE TO MOVE BLOB WITH KEYBOARD  AND USE 'T' TO PLACE A BLOB IN THIS MODE \n");
		printf("'t' -> PLACE BLOB\n");
		printf("BLOB XZ LOCATION AND PLACEMENT IN EDIT MODE \n");
		printf("YOU ARE ALSO ABLE TO USE MOUSE TO EDIT TERRAIN WHICH WORKS BY LEFT MOUSE CLICK IN THIS MODE\n");
		printf("ALSO IN THIS MODE YOU ARE ABLE TO UNDO A CHIAN OF PLACED BLOBS WITH CONTINIOUS 'U' OR CONTINIOUS REDO WITH 'R' \n");
		printf("OR ALSO THE ABILITY TO JUST INDO THE MOST RECENT PLACED BLOB WITH 'Y' \n");
		printf("\n");
		printf("\n");
		printf("++++++++++++++++++++++++++++++++++++++++++++++MODIFY BLOB MODE[F4]++++++++++++++++++++++++++++++++++++++++++ \n");
		printf("IN THIS MODE YOU ARE ABLE TO EDIT ATTRIBUTES OF BLOB LIKE A VALUE, B VALUE WHICH IN TURN MODIFIES WIDTH, HEIGHT,\n");
		printf("MAKE BLOB TALLER: UP KEY IN THIS MODE  \n");
		printf("MAKE BLOB SMALLER: DOWN KEY IN THIS MODE (NEGATIVE B VALUE TO MAKE VALLEYS) \n");
		printf("MAKE BLOB WIDER: RIGHT KEY IN THIS MODE \n");
		printf("MAKE BLOB THINNER: LEFT KEY IN THIS MODE \n");
		printf("+++++++++++++++++++++++++++++++++++RESET TERRAIN[F5]++++++++++++++++++++++++++++++++++++++++++ \n");
		printf("------------------------------------------------------------------------------------------------- \n");
		printf("+++++++++++++++++++++++++++++++++++PERSICOPE MODE[F6]++++++++++++++++++++++++++++++++++++++++++ \n");
		printf("------------------------------------------------------------------------------------------------- \n");
		printf("IN THIS MODE YOU SWITCH YOUR GLU LOOK AT TO VIEW OCEAN FROM PERISCOPE IN WHICH YOU ARE ABLE TO\n");
		printf("LOOK AROUND FREELY 360 DEGREES AND ALSO A DEPTH-OF-FIELD EFFECT TO MIMIC BEING UNDERWATER AND MOVING\n");
		printf("YOU CAN USE CURSOPR TO ZOOM AND ALSO IF SHADER IS SWITCHED OFF(PRESS F9) YOU ALSO SEE A LENS EFFECT\n");
		printf("OF THE PERISCOPE USING BLENDING AND SUCH\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("=================================OTHER KEYBOARD INPUTS===============================================\n");
		printf("SPACEBAR: IS TO SHHOT TORPEDO\n");
		printf("TAB: IS TO TOGGLE TRAJECTORY HELPER\n");
		printf("K: TO TOGGLE FOLLOW/HIGHLIGHT WHERE BULLET IS GOING\n");
		printf("P: TO TOGGLE PERISCOPE ANIMATION UP/DOWN (AUTOMATICALLY COMES UP IN PERSICOPE MODE\n");
		printf("AWSD: MOVE SUB\n");
		printf("G: TO RESET SUB LOCATION AD ATTRIBUTES AKA HEALT, ANGLES, ETC TO DEFAULT (ALSO ACTIVATED IF HEALTH BELOW -50\n");
		printf("F AND E: ASCEND/DESEND SUB\n");
		printf("q: START AND INCREASE ENGINE ROTORS\n");
		printf("IF EDIT MODE: T TO PLACE BLOB, R TO REDO, U TO UNDO\n");
		printf("\n");
		printf("||||||||||||||||||||||||||||||||||ADDICTIONAL NOTES||||||||||||||||||||||||||||||||||||||||||||||||\n");
		printf("IF YOU CRASH, YOUR HEALTH DEPLETS ACCORDINGLY AND IF U KEEP TRYING TO GO DOWN, IT TOGGLES FORCE RESET\n");
		printf("YOU START WITH 100 HP AND DESCENDING WHILE CRASHED DEPLETS HEALTH BY 1 EACH PRESS OF KEY=F\n");
		printf("@ 0 HP SUB DISAPEARS AND MSG APPEARS @ -50 FORCE RESETS\n");
		printf("CRASHING MAKES SCREEN BLINK RED, HITTING OCEAN FLOOR OBJECTS OR ENEMY FLASHED GREEN(A HIT)\n");
		printf("SCORE SYSTEM:::: YOUR SCORE INCEARSES EACH TIME BULLET HITS ENEMY SUB OR FLOOR OBJECTS, SCORE\n");
		printf("DECREASES IF CRASHED.\n");
		printf("\n");
		printf("\n");
		printf("\n");
		
		printf("To close the program sub window, press EXIT \n");
		printf("++++++++++++++++++++++++++++++++++++++++++++++++++++[BONUS]++++++++++++++++++++++++++++++++++++++++++ \n");
		printf("ADDICTIONAL BONUS/MISCELLANEOUS FEATURES\n");
		printf("1-> AN OCEAN TOP WHICH IS ANIMATED TO SIMULATE PSEDO-WATER FLUID PHYSICS \n");
		printf("2-> SCORE SYSTEM/HEALTH SYSYEM  \n");
		printf("3-> A SEMI-ADVANCED SHADER PROGRAM WHICH DYNAMICALLY DIFFUSES LIGHT ON TOP OF TEXTURES WITH SHINIENESS  \n");
		printf("4-> CAN TURN OFF SHADER ONCE USING KEY=F9 WHICH THEN ALLOWS YOU TO SEE A SIMUALTED LENS EFFECT IN PERISCOPE MODE KEY=F6  \n");
		printf("5-> GREEN/RED COLOR FLASH UPON SCORING OR CRASHING  \n");
		printf("6->   \n");
		printf("6-> REDO FEATURE USING STRUCT ARAYS POINTER AND INCREMENTER  \n");
		printf("7-> TRANSPARENT OCEAN FLOOR TO ALLOW TO SEEING CIRCLE FOR EDITING AND ALSO MIMICS UNDERWATER  \n");
		printf("8-> A MENU WHICH CAN BE ACTIVATED WITH MIDDLE MOUSE BUTTON(HELD DOWN)  \n");

	}

	//open second helper window (NOTE THIS IS PARTLY SOURCE CODE SO NOT PART OF ASSIGNMENT,BUT EXTRA WORK, to avoid plagiarism)
	//if (key == GLUT_KEY_F7)
	//{
		//initOpenGL(vWidth, vHeight);
		//glutInitWindowSize(1000, 1000);
		//glutCreateWindow("GLUT fonts");
		//glutDisplayFunc(my_display    );
		//glutReshapeFunc(my_reshape);
		//glutMotionFunc(mouseMove);
		//glutKeyboardFunc(keyboard);
		//glutSpecialFunc(functionKeys);

	//}

	//exit seocndary helper window
	if (key == GLUT_KEY_F8)
	{
		glutHideWindow();
	}

	if (key == GLUT_KEY_F2 || value == 2)
	{
		arcCam = true;
		editMode = false;
		modifyMode = false;
		modifyContinious = false;
		periscopeMode = false;

	}
	if (key == GLUT_KEY_F3 || value == 3)
	{
		arcCam = false;
		editMode = true;
		modifyMode = false;
		modifyContinious = true;
		periscopeMode = false;
		 

	}
	if (key == GLUT_KEY_F4 || value == 4)
	{
		arcCam = false;
		editMode = false;
		modifyMode = true;
		modifyContinious = false;
		periscopeMode = false;

	}

	if (key == GLUT_KEY_F6 || value == 6)
	{
		arcCam = false;
		editMode = false;
		modifyMode = false;
		modifyContinious = false;
		periscopeMode = true;
		//periscope = true;
		scopeUp = true;
		//subAngle += 0.0001;
		lx = -sin(3.2 + subAngle * -0.017);
		ly = cos(3.2 + subAngle * -0.017);
		fov = 130;
		reshape(vWidth, vHeight);

	}
	if (key == GLUT_KEY_F9 || value == 9)
	{
		glUseProgramObjectARB(0);
	}

	if (key == GLUT_KEY_LEFT && editMode)
	{
		blobx -= 1;
	}
	if (key == GLUT_KEY_RIGHT && editMode)
	{
		blobx += 1;

	}
	if (key == GLUT_KEY_UP && editMode)
	{
		blobz -= 1;

	}
	if (key == GLUT_KEY_DOWN && editMode)
	{
		blobz += 1;
	}

	if (key == GLUT_KEY_LEFT && periscopeMode)
	{
		blobx -= 1;
	}
	if (key == GLUT_KEY_RIGHT && periscopeMode)
	{
		blobx += 1;

	}
	if (key == GLUT_KEY_UP && periscopeMode )
	{
		deltaMove = 1.0;
	}
	if (key == GLUT_KEY_DOWN && periscopeMode)
	{
		deltaMove = -1.0;
	}



	if (key == GLUT_KEY_RIGHT && blobA <= 10.0 && modifyMode)
	{
		blobA += 0.1;
		printf("more, %f \n", blobA);

	}
	if (key == GLUT_KEY_LEFT && blobA > 0.2 && modifyMode)
	{
		blobA -= 0.1;
		printf(" less, %f \n", blobA);

	}
	if (key == GLUT_KEY_UP && modifyMode)
	{
		if (blobB == -0.1) blobB = 0.1;
		blobB += 0.1;
		printf(" up, %f \n", blobB);

	}
	if (key == GLUT_KEY_DOWN && modifyMode)
	{
		if (blobB == 0.1) blobB = -0.1;
		blobB -= 0.1;
		printf(" down, %f \n", blobB);

	}

	if (key == GLUT_KEY_F5) {
		resetTerrain(&groundMesh);
	}
	if (!periscopeMode)
	{
		fov = 60;
		reshape(vWidth, vHeight);
	}

	glutPostRedisplay();   // Trigger a window redisplay
}

void releaseSpecialKey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: deltaMove = 0.0; break;
	case GLUT_KEY_DOWN: deltaMove = 0.0; break;
	}
}

void mouseButton(int button, int state, int x, int y) {

	if (arcCam) {
		// only start motion if the left button is pressed
		if (button == GLUT_LEFT_BUTTON) {

			

			// when the button is released
			if (state == GLUT_UP) {
				theta += deltaAngle;
				xOrigin = -1;
			}
			else {// state = GLUT_DOWN
				xOrigin = x;
				//yOrigin = y;
			}
		}
		if (button == GLUT_RIGHT_BUTTON) {

			// when the button is released
			if (state == GLUT_UP) {
				thetay += deltaAngley;
				yOrigin = -1;
			}
			else {
				yOrigin = y;
			}
		}
		if (button == 3)
		{
			if (d > 4)
			{
				d -= 0.1;
			}
		}
		if (button == 4)
		{
			if (d < 8)
			{
				d += 0.1;
			}


		}

	}
	if (editMode)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			location = ScreenToWorld(x, y);
			oldX = location.x;
			oldZ = location.z;
			oldA = blobA;
			oldB = blobB;
			undo = true;
			printf("x, %f \n", location.x);
			printf("z, %f \n", location.z);
			changeY(&groundMesh, location.x, location.z, blobB, blobA);
		}
	}

	if (periscopeMode)
	{
		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) { // left mouse button pressed
				isDragging = 1; // start dragging
				xDragStart = x; // save x where button first pressed
			}
			else { /* (state = GLUT_UP) */
				angle += deltaAngleP; // update camera turning angle
				isDragging = 0; // no longer dragging
			}
		}
		//zoom out
		if (button == 3)
		{
			if (fov >20)
			{
				fov -= 1;
				reshape(vWidth, vHeight);
			}
		}
		//zoom in
		if (button == 4)
		{
			if (fov <150)
			{
				fov += 1;
				reshape(vWidth,vHeight);
			}


		}
	}
	glutPostRedisplay();   // Trigger a window redisplay




}


// Mouse motion callback - use only if you want to
void mouseMove(int x, int y) {
	Vector3D location = ScreenToWorld(x, y);
	//changeY(&groundMesh, location.x, location.z, 1.0, 1.0);

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		temp = deltaAngle;
		deltaAngle = (x - xOrigin) * 0.0001f;


		if ((temp - deltaAngle) < 0)
		{
			theta -= 0.028;
		}
		else
		{
			theta += 0.028;
		}
		eyex = sin(theta + deltaAngle) * 5;
		eyez = cos(theta + deltaAngle) * 5;
	}
	if (yOrigin >= 0) {

		tempy = deltaAngley;
		deltaAngley = (y - yOrigin) * 0.00001f;
		if ((tempy - deltaAngley) > 0 && eyey > -1)
		{
			eyey -= 0.5;
		}
		if ((tempy - deltaAngley) < 0 && eyey < 25.0)
		{
			eyey += 0.5;
		}
	}

	if (periscopeMode)
	{
		if (isDragging) { // only when dragging
		// update the change in angle
			deltaAngleP = (x - xDragStart) * 0.005;

			// camera's direction is set to angle + deltaAngle

			lx = -sin(angle + deltaAngleP);
			ly = cos(angle + deltaAngleP);

			

		}
	}
	
	glutPostRedisplay();   // Trigger a window redisplay

}

void idleMouse(int x, int y)
{
	Vector3D location = ScreenToWorld(x, y);
	changeY(&groundMesh, location.x, location.z, blobB, blobA);
}
Vector3D ScreenToWorld(int x, int y)
{

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = x;
	winY = (float)viewport[3] - y;
	glReadPixels(x, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return NewVector3D(posX, posY, posZ);


}

//for printing text in opengl
void print_bitmap_string(/*void* font,*/ char* s)
{

	while (*s) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s);
		s++;
	}

}

//reshape funciton for helper function
void my_reshape(int w, int h)
{
	GLdouble size;
	GLdouble aspect;

	/* Use the whole window. */
	glViewport(0, 0, w, h);

	/* We are going to do some 2-D orthographic drawing. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	size = (GLdouble)((w >= h) ? w : h) / 2.0;
	if (w <= h) {
		aspect = (GLdouble)h / (GLdouble)w;
		glOrtho(-size, size, -size * aspect, size * aspect, -100000.0, 100000.0);
	}
	else {
		aspect = (GLdouble)w / (GLdouble)h;
		glOrtho(-size * aspect, size * aspect, -size, size, -100000.0, 100000.0);
	}

	/* Make the world and window coordinates coincide so that 1.0 in */
	/* model space equals one pixel in window space.                 */
	glScaled(aspect, aspect, 1.0);

	/* Now determine where to draw things. */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

//draw the text in helkper function
void draw_text()
{
	char* bitmap_font_names[7] = { "===============================HELPER FUNCTIONS============================" };
	char* bitmap_font_names1[7] = { "MOVEMENTS --> * moving the sub is realistic in the sense that the propeller" };
	char* bitmap_font_names2[7] = { "                                    does not magically move at a constant speed, but rather slowly gets faster." };
	char* bitmap_font_names3[7] = { "                                    Also when going backlwards, the propeller is significantly less speed.*" };
	char* bitmap_font_names4[7] = { "                                    When you move up/down/left/right, the fins move accordingly and also the sub is" };
	char* bitmap_font_names5[7] = { "                                    angled up and down based on ascending/descending" };
	char* bitmap_font_names6[7] = { "----------------------------------------------------------------------------" };
	char* bitmap_font_names7[7] = { "UP:w     |     DOWN:s     |     LEFT:a     |     RIGHT:d    |    ASCEND:r    |    DESCEND:f" };
	char* bitmap_font_names8[7] = { "" };
	char* bitmap_font_names9[7] = { "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" };
	char* bitmap_font_names10[7] = { "CAMERA MOVEMENTS --> *this is to manipulate the x/y/z camera perspecctive" };
	char* bitmap_font_names11[7] = { "                                      which makes it easier to see the sub when its moving out of the camera view.*" };
	char* bitmap_font_names12[7] = { "----------------------------------------------------------------------------" };
	char* bitmap_font_names13[7] = { "MOVE CAMERA X axis plane:LEFT/RIGHT ARROW KEYS" };
	char* bitmap_font_names14[7] = { "MOVE CAMERA Y axis plane:UP/DOWN ARROW KEYS" };
	char* bitmap_font_names15[7] = { "MOVE CAMERA Z axis plane:F4/F5 FUNCTION KEYS" };
	char* bitmap_font_names16[7] = { "----------------------------------------------------------------------------" };
	char* bitmap_font_names17[7] = { "To start the engine you hold down e and gradually the propellers start spinning faster" };
	char* bitmap_font_names18[7] = { "----------------------------------------------------------------------------" };
	char* bitmap_font_names19[7] = { "START ENGINE: e" };
	char* bitmap_font_names20[7] = { "" };
	char* bitmap_font_names21[7] = { "" };
	char* bitmap_font_names22[7] = { "To close the main sub window, press EXIT, ands to close the helper function window, press F3" };
	char* bitmap_font_names23[7] = { "" };






	/* Draw the strings, according to the current mode and font. */
	glTranslatef(-220, 220, 0);
	glColor4f(1.0, 1.0, 0.0, 0.0);

	glRasterPos2f(0, 0);
	print_bitmap_string(bitmap_font_names[0]);

	glRasterPos2f(0, -15);
	print_bitmap_string(bitmap_font_names1[0]);

	glRasterPos2f(0, -30);
	print_bitmap_string(bitmap_font_names2[0]);

	glRasterPos2f(0, -45);
	print_bitmap_string(bitmap_font_names3[0]);

	glRasterPos2f(0, -60);
	print_bitmap_string(bitmap_font_names4[0]);

	glRasterPos2f(0, -75);
	print_bitmap_string(bitmap_font_names5[0]);

	glRasterPos2f(0, -90);
	print_bitmap_string(bitmap_font_names6[0]);

	glRasterPos2f(0, -105);
	print_bitmap_string(bitmap_font_names7[0]);

	glRasterPos2f(0, -120);
	print_bitmap_string(bitmap_font_names8[0]);

	glRasterPos2f(0, -135);
	print_bitmap_string(bitmap_font_names9[0]);

	glRasterPos2f(0, -150);
	print_bitmap_string(bitmap_font_names10[0]);

	glRasterPos2f(0, -165);
	print_bitmap_string(bitmap_font_names12[0]);

	glRasterPos2f(0, -180);
	print_bitmap_string(bitmap_font_names13[0]);

	glRasterPos2f(0, -195);
	print_bitmap_string(bitmap_font_names14[0]);

	glRasterPos2f(0, -210);
	print_bitmap_string(bitmap_font_names15[0]);

	glRasterPos2f(0, -225);
	print_bitmap_string(bitmap_font_names16[0]);

	glRasterPos2f(0, -240);
	print_bitmap_string(bitmap_font_names17[0]);

	glRasterPos2f(0, -255);
	print_bitmap_string(bitmap_font_names18[0]);

	glRasterPos2f(0, -270);
	print_bitmap_string(bitmap_font_names19[0]);

	glRasterPos2f(0, -285);
	print_bitmap_string(bitmap_font_names20[0]);

	glRasterPos2f(0, -300);
	print_bitmap_string(bitmap_font_names21[0]);

	glRasterPos2f(0, -315);
	print_bitmap_string(bitmap_font_names22[0]);

	glRasterPos2f(0, -330);
	print_bitmap_string(bitmap_font_names23[0]);


}
//to display helper function on secondary screen
void my_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	draw_text();

	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.5, 1.0);

	glVertex3f(0.5f, 0.0f, -4.0f);
	glVertex3f(1.0f, 0.5f, -4.0f);
	glVertex3f(0.5f, 0.5f, -4.0f);

	glEnd();

	glutSwapBuffers();
}

const GLdouble nearVal = 1.0;
const GLdouble farVal = 20.0;
const GLfloat  lightPos[4] = { 3.0f, 3.0f, 3.0f, 1.0f };
GLuint         program = 0;
GLint          texMapLocation;
GLint          normalLocation;
GLint          ligntPosLocation;

/* shader reader */
/* creates null terminated string from file */

static char* readShaderSource(const char* shaderFile)
{
	struct stat statBuf;
	FILE* fp = fopen(shaderFile, "r");
	char* buf;

	stat(shaderFile, &statBuf);
	buf = (char*)malloc((statBuf.st_size + 1) * sizeof(char));
	fread(buf, 1, statBuf.st_size, fp);
	buf[statBuf.st_size] = '\0';
	fclose(fp);
	return buf;
}

/* error printing function */

static void checkError(GLint status, const char* msg)
{
	if (!status)
	{
		printf("%s\n", msg);
		exit(EXIT_FAILURE);
	}
}
static void initShader(const GLchar* vShaderFile, const GLchar* fShaderFile)
{
	GLint status = glGetError() == GL_NO_ERROR;
	GLchar* vSource, * fSource;
	GLuint vShader, fShader;
	GLuint texMapLocation;

	/* read shader files */

	vSource = readShaderSource(vShaderFile);
	checkError(status, "Failed to read vertex shader");

	fSource = readShaderSource(fShaderFile);
	checkError(status, "Failed to read fragment shader");

	/* create program and shader objects */

	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();

	/* attach shaders to the program object */

	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	/* read shaders */

	glShaderSource(vShader, 1, (const GLchar * *)& vSource, NULL);
	glShaderSource(fShader, 1, (const GLchar * *)& fSource, NULL);

	/* compile shaders */

	glCompileShader(vShader);
	glCompileShader(fShader);

	/* error check */

	glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
	//checkError(status, "Failed to compile the vertex shader.");

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
	//checkError(status, "Failed to compile the fragment shader.");

	/* link */

	glLinkProgram(program);
	glGetShaderiv(program, GL_LINK_STATUS, &status);
	//checkError(status, "Failed to link the shader program object.");

	/* use program object */

	glUseProgram(program);

	/* set up uniform parameter */

	//texMapLocation = glGetUniformLocation(program, "texMap");
}

