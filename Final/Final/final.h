#include <stdio.h>
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <ctime>
#include "Angel.h"
#include "glm.h"
#include "mat.h"
#include "vec.h"
#include "CheckError.h"

using namespace std;

#define PI 3.14159265
#define MIN_X 0
#define MIN_Y 1
#define MAX_X 2
#define MAX_Y 3
#define ROTATE 0.1

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

GLMmodel* models[1];
int VBO_num_vert;
int VBO_num_norm;
int num_vert;

#define SCALE_VECTOR    0.5

GLuint program;
GLuint ModelViewCam, ModelViewObj, Projection, LightPosition, NormalTransformation;

GLuint vPosition;
GLuint vNormal;
int winHeight = 480;
int winWidth = 640;
bool mouseDown = false;
float xrot = -20;
float yrot = 0;
float xdiff = 0;
float ydiff = 0;
GLfloat step = 5;
// Camera Coordianate System
vec4 u = vec4(1, 0, 0, 0);
vec4 v = vec4(0, 1, 0, 0);
vec4 n = vec4(0, 0, 1, 0);
vec4 eye = vec4(0,2,10,1);

void roll(GLfloat num);
void pitch(GLfloat num);
void yaw(GLfloat num);
void init();
void display();
void keyboard(unsigned char key, int x, int y);
void arrow(int key, int x, int y);
void reshape(GLint w, GLint h);
int main(int argc, char** argv);
void getGLinfo();