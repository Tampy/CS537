#include "final.h"

void importModels()
{
	import[0] = glmReadOBJ(".\car\natla car.obj");
	glmUnitize(import[0]);
	glmFacetNormals(import[0]);
	glmVertexNormals(import[0], 90.0);
	glmLoadInVBO(import[0]);
}

void roll(GLfloat num)
{
	GLfloat x,y,z;
	x = v[0];
	y = v[1];
	z = v[2];

	v[0] = cosf(num) * x - sinf(num) * u[0];
	v[1] = cosf(num) * y - sinf(num) * u[1];
	v[2] = cosf(num) * z - sinf(num) * u[2];
	u[0] = sinf(num) * x + cosf(num) * u[0];
	u[1] = sinf(num) * y + cosf(num) * u[1];
	u[2] = sinf(num) * z + cosf(num) * u[2];
}

void pitch(GLfloat num)
{
	GLfloat x,y,z;
	x = v[0];
	y = v[1];
	z = v[2];

	v[0] = cosf(num)*x - sinf(num)*n[0];
	v[1] = cosf(num)*y - sinf(num)*n[1];
	v[2] = cosf(num)*z - sinf(num)*n[2];
	n[0] = sinf(num)*x + cosf(num)*n[0];
	n[1] = sinf(num)*y + cosf(num)*n[1];
	n[2] = sinf(num)*z + cosf(num)*n[2];
}

void yaw(GLfloat num)
{
	GLfloat x,y,z;
	x = u[0];
	y = u[1];
	z = u[2];

	u[0] = cosf(num)*x - sinf(num)*n[0];
	u[1] = cosf(num)*y - sinf(num)*n[1];
	u[2] = cosf(num)*z - sinf(num)*n[2];
	n[0] = sinf(num)*x + cosf(num)*n[0];
	n[1] = sinf(num)*y + cosf(num)*n[1];
	n[2] = sinf(num)*z + cosf(num)*n[2];
}

void init()
{
	importModels();

	// Load shaders and use the resulting shader program
	program = InitShader( "vshader23_v110.glsl","fshader23_v110.glsl" );
	glUseProgram( program );

	modelView = glGetUniformLocation( program, "modelView" );
	modelViewObj = glGetUniformLocation(program, "ModelViewObj");
	Projection = glGetUniformLocation( program, "projection" );
	normalTrans = glGetUniformLocation( program, "normalTransformation" );
	
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 currentLoc = LookAt(persp, persp - n, v);
	glUniformMatrix4fv(modelView, 1, GL_TRUE, currentLoc);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'Z':
			roll(ROTATE);
			break;
		case 'z':
			roll(-ROTATE);
			break;
		case 'X':
			pitch(ROTATE);
			break;
		case 'x':
			pitch(-ROTATE);
			break;
		case 'C':
			yaw(ROTATE);
			break;
		case 'c':
			yaw(-ROTATE);
			break;
	}

	glutPostRedisplay();
}

void arrow(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			persp -= ROTATE * n;
			break;
		case GLUT_KEY_DOWN:
			persp += ROTATE * n;
			break;
		case GLUT_KEY_LEFT:
			persp -= ROTATE * u;
			break;
		case GLUT_KEY_RIGHT:
			persp += ROTATE * u;
			break;
	}

	glutPostRedisplay();
}

void reshape( int width, int height )
{
 glViewport( 0, 0, width, height );
  GLfloat aspect = GLfloat(width)/height;
  mat4  projection = Perspective( 45.0, aspect, 0.0001, 300.0 );
  glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
}

int main(int argc, char** argv)
{
	srand((unsigned int) time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowSize (512, 512); 

	glutCreateWindow ("Final Project");
	glutInitWindowPosition (100, 100);
	glewInit();
	getGLinfo();
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow);
	glutReshapeFunc(reshape);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutMainLoop();
	return 0;
}


// Get OpenGL version information
void getGLinfo()
{
  cout << "GL Vendor   : " << glGetString(GL_VENDOR) << endl;
  cout << "GL Renderer : " << glGetString(GL_RENDERER) << endl;
  cout << "GL Version  : " << glGetString(GL_VERSION) << endl;
}

/*void m_glewInitAndVersion(void)
{
   fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
   fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
   GLenum err = glewInit();
   if (GLEW_OK != err)
	fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}*/