#include <stdio.h>
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <ctime>
#include "Angel.h"

using namespace std;

#define PI 3.14159265
#define MIN_X 0
#define MIN_Y 1
#define MAX_X 2
#define MAX_Y 3

GLuint buffers[2];
GLuint buffersO[2];

GLfloat width;
GLfloat height;

GLuint vPosition;
GLuint program; 
GLuint projmat_loc;
GLuint modelview_loc;
GLuint draw_color_loc;

Angel::mat4 modelview;
Angel::mat4 perspective;
Angel::mat4 rotation = Angel::identity();

GLfloat scale = 1.25f;
GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;
GLfloat angleZ = 0.0f;

GLfloat minX=-0.5, minY=-0.5, maxX=0.5, maxY=0.5, minZ = -0.5, maxZ = 0.5;

vec4 point_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
vec4 start_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
vec4 blue_trajectory = vec4(0.0f, 0.0f, 1.0f, 1.0f);
vec4 base_color = vec4(0.0f, 1.0f, 0.0f, .10f);
vec4 collide_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
vec4 colors[11];

int num_verts = 18;
int num_sides = 9;
vec4* vertices;
GLuint* edges;
GLuint**  sides;

int count;
struct pointNode* head = NULL;
struct pointNode* curr = NULL;

bool bPaused = false;
bool bComplete = false;
bool bStarted = false;

int moving = 0;
int startx = 0;
int starty = 0;
int angle = 0;
int angle2 = 0;

float displacement;

void m_glewInitAndVersion(void)
{
   fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
   fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
   fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   }
   fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}


void randomDisplacement(GLfloat magnitude, GLfloat &x, GLfloat &y, GLfloat &z);
int pointCount(struct pointNode* head);
void printData(struct pointNode* head);
pointNode* getRandomStart(GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, GLfloat zMin, GLfloat zMax);
pointNode* AddNode(struct pointNode* node, GLfloat x, GLfloat y, GLfloat z);
void calcDisplacement();
bool checkNode(struct pointNode * curr, GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, GLfloat zMin, GLfloat zMax);
void init ();
void display();
GLfloat* copyToArray(struct pointNode * head);
void animate(int i);
void keyboard (unsigned char key, int x, int y);
void findExitPoint(struct pointNode * prev, struct pointNode * last);
void buildRectangle();
void reshape(GLint w, GLint h);
static void mouse(int button, int state, int x, int y);
static void motion(int x, int y);


struct pointNode
{
	GLfloat x;
	GLfloat y;
	GLfloat z;

	pointNode *next;
};
