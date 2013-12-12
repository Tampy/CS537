#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "Angel.h"

using namespace std;

#define PI 3.14159265
#define MIN_X 0
#define MIN_Y 1
#define MAX_X 2
#define MAX_Y 3

#define ROTATE 0.1

struct model{
	mat4 translation;
	mat4 rotation;
	mat4 scale;
};

GLfloat width;
GLfloat height;

int numVerts;
int numHouses;
int numMountains;
int numTrees;

vec4 camPosition = vec4(1.0, -1.0, 2.0, 1.0);
vec4 camX = vec4(1.0, 0.0, 0.0, 0.0);
vec4 camY = vec4(0.0, 1.0, 0.0, 0.0);
vec4 camZ = vec4(0.0, 0.0, 1.0, 0.0);

mat4 ctmat;
mat4 projmat;
mat4 modelview;
mat4 camMat;

GLuint vbo[2];
GLuint color_loc;
GLuint model_view_loc;
GLuint program;

vec4* backColor;
vec4* houseColor;
vec4* treeColor;
vec4* mountainColor;
vec4* verts;

GLushort** houseIndex;
GLushort** mountainIndex;
GLushort** treeIndex;
GLushort* houseVerts;
GLushort* mountainVerts;
GLushort* treeVerts;

model* houses;
model* mountains;
model* trees;

void roll(GLfloat);
void pitch(GLfloat);
void yaw(GLfloat);
void draw_models();
void createEnv();
void init();
void reshape(GLint width, GLint height);
void keyboard(unsigned char key, int x, int y);
void display();
void m_glewInitAndVersion();