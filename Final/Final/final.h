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

#define X               0
#define Y               1
#define Z               2
#define SCALE_VECTOR    1.0
#define SCALE_ANGLE     1.0

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

GLMmodel* models[1];
int VBO_num_vert;
int VBO_num_norm;
int num_vert;

#define SCALE_VECTOR    0.5

GLuint program[3]; 
GLuint VAO[3];
GLuint buffers[5];

GLuint ModelViewCam, ModelViewObj, Projection, LightPosition, NormalTransformation;
GLuint color_loc;
GLuint proj_loc;
GLuint model_view_loc;

enum {cube_vertices, cube_indeces, wire_indeces, cube_vertices_explicit};

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

//checks if we're in helicopter or car camera
bool heliCam;

void roll(GLfloat num);
void pitch(GLfloat num);
void yaw(GLfloat num);
void init();
void skyboxInit();
void display();
void displaySkybox();
void keyboard(unsigned char key, int x, int y);
void arrow(int key, int x, int y);
void reshape(GLint w, GLint h);
void idle();
int main(int argc, char** argv);
void getGLinfo();

//all that follows is skybox information
point4 cube_vertices_positions[24] = {
	      //x_positive
         point4( 1.0f, -1.0f, -1.0f, 1.0f),
         point4( 1.0f, -1.0f,  1.0f, 1.0f),
         point4( 1.0f,  1.0f,  1.0f, 1.0f),
         point4( 1.0f,  1.0f, -1.0f, 1.0f),
		 //x-negative
         point4(-1.0f, -1.0f,  1.0f, 1.0f),
         point4(-1.0f, -1.0f, -1.0f, 1.0f),
         point4(-1.0f,  1.0f, -1.0f, 1.0f),
         point4(-1.0f,  1.0f,  1.0f, 1.0f),
		 //y-positive
         point4(-1.0f,  1.0f, -1.0f, 1.0f),
         point4( 1.0f,  1.0f, -1.0f, 1.0f),
         point4( 1.0f,  1.0f,  1.0f, 1.0f),
         point4(-1.0f,  1.0f,  1.0f, 1.0f),
		 //y-negatve
         point4(-1.0f, -1.0f,  1.0f, 1.0f),
         point4( 1.0f, -1.0f,  1.0f, 1.0f),
         point4( 1.0f, -1.0f, -1.0f, 1.0f),
         point4(-1.0f, -1.0f, -1.0f, 1.0f),
		 //z-positive
         point4( 1.0f, -1.0f,  1.0f, 1.0f),
         point4(-1.0f, -1.0f,  1.0f, 1.0f),
         point4(-1.0f,  1.0f,  1.0f, 1.0f),
         point4( 1.0f,  1.0f,  1.0f, 1.0f),
		 //z-negative
         point4(-1.0f, -1.0f, -1.0f, 1.0f),
         point4( 1.0f, -1.0f, -1.0f, 1.0f),
         point4( 1.0f,  1.0f, -1.0f, 1.0f),
         point4(-1.0f,  1.0f, -1.0f, 1.0f)
};

GLubyte indices[]= {
1, 0, 3, 
1, 3, 2, 
2, 3, 7, 
2, 7, 6, 
3, 0, 4, 
3, 4, 7, 
6, 5, 1, 
6, 1, 2, 
4, 5, 6, 
4, 6, 7, 
5, 4, 0, 
5, 0, 1
};

class MatrixStack {
    int    _index;
    int    _size;
    mat4*  _matrices;
    
public:
    MatrixStack( int numMatrices = 32):_index(0), _size(numMatrices)
	{	_matrices = new mat4[numMatrices]; }
    
    ~MatrixStack()
	{ delete[]_matrices; }
    
    void push( const mat4& m ) {
        assert( _index + 1 < _size );
        _matrices[_index++] = m;
    }
    
    mat4& pop( void ) {
        assert( _index - 1 >= 0 );
        _index--;
        return _matrices[_index];
    }
};

//Skybox struc
GLuint texture;
typedef struct
{
   
    int topWidth;
    int topHeight;
    GLubyte *top;
    
    int bottomWidth;
    int bottomHeight;
    GLubyte *bottom;
    
    int leftHeight;
    int leftWidth;
    GLubyte *left;
    
    
    int rightWidth;
    int rightHeight;
    GLubyte *right;
    
    int frontWidth;
    int frontHeight;
    GLubyte *front;
    
    int backWidth;
    int backHeight;
    GLubyte *back;
    
} CubeMap;

CubeMap skybox;


//make sure you start with the default coordinate system
mat4 projmat = mat4(1.0);

MatrixStack mvstack;

mat4 model_view = Angel::mat4(1.0);

mat4 modelviewStackTop = mat4(1.0);
mat4 modelviewStackBottom=  mat4(1.0);