#include "hwk3.h"

//the following 3 methods rotate the camera according to roll, pitch, and yaw.
void roll(GLfloat num)
{
	GLfloat x, y, z;
	x = camY[0];
	y = camY[1];
	z = camY[2];

	camY[0] = cosf(num) * x - sinf(num) * camX[0];
	camY[1] = cosf(num) * y - sinf(num) * camX[1];
	camY[2] = cosf(num) * z - sinf(num) * camX[2];
	camX[0] = sinf(num) * x + cosf(num) * camX[0];
	camX[1] = sinf(num) * y + cosf(num) * camX[1];
	camX[2] = sinf(num) * z + cosf(num) * camX[2];
}

void pitch(GLfloat num)
{
	GLfloat x, y, z;
	x = camY[0];
	y = camY[1];
	z = camY[2];
	
	camY[0] = cosf(num)*x - sinf(num)*camZ[0];
	camY[1] = cosf(num)*y - sinf(num)*camZ[1];
	camY[2] = cosf(num)*z - sinf(num)*camZ[2];
	camZ[0] = sinf(num)*x + cosf(num)*camZ[0];
	camZ[1] = sinf(num)*y + cosf(num)*camZ[1];
	camZ[2] = sinf(num)*z + cosf(num)*camZ[2];
}

void yaw(GLfloat num)
{
	GLfloat x, y, z;
	x = camX[0];
	y = camX[1];
	z = camX[2];
	
	camX[0] = cosf(num)*x - sinf(num)*camZ[0];
	camX[1] = cosf(num)*y - sinf(num)*camZ[1];
	camX[2] = cosf(num)*z - sinf(num)*camZ[2];
	camZ[0] = sinf(num)*x + cosf(num)*camZ[0];
	camZ[1] = sinf(num)*y + cosf(num)*camZ[1];
	camZ[2] = sinf(num)*z + cosf(num)*camZ[2];
}

void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	camMat = Perspective(60.0, (GLfloat)width/(GLfloat)height, 0.1, 10.0);
}

void createEnv(int houseCount, int mountCount, int treeCount)
{
	int i, j;
	numVerts = 50;

	houses = (model*)malloc(houseCount*sizeof(model));
	mountains = (model*)malloc(mountCount*sizeof(model));
	trees = (model*)malloc(treeCount*sizeof(model));
	
	verts = new vec4[numVerts];
	backColor = new vec4[numVerts];

	houseColor = (vec4*)malloc(8*sizeof(GLushort));
	mountainColor = (vec4*)malloc(4*sizeof(GLushort));
	treeColor = (vec4*)malloc(12*sizeof(GLushort));

	houseIndex = (GLushort**)malloc(8*sizeof(GLushort*));
	mountainIndex = (GLushort**)malloc(4*sizeof(GLushort*));
	treeIndex = (GLushort**)malloc(12*sizeof(GLushort*));

	houseVerts = (GLushort*)malloc(8*sizeof(GLushort*));
	mountainVerts = (GLushort*)malloc(4*sizeof(GLushort*));
	treeVerts = (GLushort*)malloc(12*sizeof(GLushort*));

	for(i=0;i<houseCount;i++)
	{
		houses[i].rotation = RotateX(0)*RotateY(0)*RotateZ(30*i);
		houses[i].translation = Translate(0.75*i, 0.75*i, 0.0);
		houses[i].scale = Scale(1.0, 1.0, 1.0);
	}

	for(i=0;i<mountCount;i++)
	{
		mountains[i].rotation = RotateX(0)*RotateY(0)*RotateZ(30*i);
		mountains[i].translation = Translate(-1.0*(i+1), 1.0*(i+1), 0.0);
		mountains[i].scale = Scale(2.0, 2.0, 2.0);
	}

	for(i=0;i<treeCount;i++)
	{
		trees[i].rotation = RotateX(0)*RotateY(0)*RotateZ(30*i);
		trees[i].translation = Translate(0.75*(i+1), 0.75*i, 0.0);
		trees[i].scale = Scale(0.6, 0.6, 0.6);
	}

	for(i=0;i<8;i++)
	{
		verts[2*i] = vec4(-0.25 + 0.5*(i/2), -0.25 + 0.5*(((i+1)/2)%2), 0.5, 1.0);
		verts[2*i+1] = vec4(-0.25 + 0.5*(i/2), -0.25 + 0.5*(((i+1)/2)%2), 0, 1.0);
	}
	verts[2*i] = vec4(0.0, -0.25, 0.75, 1.0);
	verts[(2*i)+1] = vec4(0.0, 0.25, 0.75, 1.0);

	for(i=0;i<8;i++)
		verts[10+i] = vec4(-0.25 + 0.5*(i/2), -0.25 + 0.5*(((i+1)/2)%2), 0, 1.0);
	verts[10+i] = vec4(0.0, 0.0, 0.3, 1.0);

	for(i=0;i<5;i++)
	{
		verts[15+i] = vec4(-0.25 + 0.5*(i/2), -0.25 + 0.5*(((i+1)/2)%2), 0.3, 1.0);
		verts[30+i] = vec4(-0.25 + 0.5*(i/2), -0.25 + 0.5*(((i+1)/2)%2), 0.6, 1.0);
		verts[45+i] = vec4(-0.1 + 0.2*(i/2), -0.1 + 0.2*(((i+1)/2)%2), 0.0, 1.0);
	}
	verts[15+i] = vec4(0.0, 0.0, 0.6, 1.0);
	verts[30+i] = vec4(0.0, 0.0, 0.9, 1.0);

	for(i=0;i<4;i++)
	{
		mountainIndex[i] = new GLushort[3];
		mountainVerts[i] = 3;
		mountainColor[i] = vec4(0.9, 0.9, 0.9, 1.0);
		mountainIndex[i][0] = 10 + i;
		mountainIndex[i][1] = 10 + (i+1)%4;
		mountainIndex[i][2] = 14;
	}

	for(i=0;i<4;i++)
	{
		treeIndex[i+8] = new GLushort[3];
		treeVerts[i+8] = 3;
		treeColor[i+8] = vec4(0.0, 0.75, 0.0, 1.0);
		treeIndex[i+8][0] = 15 + i;
		treeIndex[i+8][1] = 15 + (i+1)%4;
		treeIndex[i+8][2] = 19;

		treeIndex[i+4] = new GLushort[3];
		treeVerts[i+4] = 3;
		treeColor[i+4] = vec4(0.0, 0.75, 0.0, 1.0);
		treeIndex[i+4][0] = 20 + i;
		treeIndex[i+4][1] = 20 + (i+1)%4;
		treeIndex[i+4][2] = 24;
		
		treeIndex[i] = new GLushort[3];
		treeVerts[i] = 3;
		treeColor[i] = vec4(0.5, 0.3, 0.2, 1.0);
		treeIndex[i][0] = 25 + i;
		treeIndex[i][1] = 25 + (i+1)%4;
		treeIndex[i][2] = 19;
	}

	for(i=0;i<4;i++)
	{
		houseIndex[i] = new GLushort[4];
		houseVerts[i] = 4;
		houseColor[i] = vec4(0.0, 0.5, 0.0, 1.0);
		for (j=2*i;j<2*i+4;j++)
			houseIndex[i][j-2*i] = j%8;
	}

	for(i=4;i<6;i++)
	{
		houseIndex[i] = new GLushort[4];
		houseVerts[i] = 4;
		houseColor[i] = vec4(0.5, 0.0, 0.0, 1.0);
		houseIndex[i][0] = 4*(i-4);
		houseIndex[i][1] = 4*(i-4)+2;
		houseIndex[i][2] = 4+i;
		houseIndex[i][3] = 13-i;
	}

	for(i=6;i<8;i++)
	{
		houseIndex[i] = new GLushort[3];
		houseVerts[i] = 3;
		houseColor[i] = vec4(0.5, 0.0, 0.0, 1.0);
		houseIndex[i][0] = (4*(i-6)+2);
		houseIndex[i][1] = (4*(i-6)+4)%8;
		houseIndex[i][2] = 15-i;
	}

}

void init()
{
	numHouses = 5;
	numMountains = 2;
	numTrees = 3;

	createEnv(numHouses, numMountains, numTrees);

	program = InitShader("vertex_shader.glsl", "fragment_shader.glsl");
	glUseProgram(program);
	color_loc = glGetUniformLocation(program, "color");
	model_view_loc = glGetUniformLocation(program, "modelview");

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(vec4), verts, GL_STATIC_DRAW);

	glutPostRedisplay();
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
		case GLUT_KEY_UP:
			camPosition -= 0.1*camZ;
			break;
		case GLUT_KEY_DOWN:
			camPosition += 0.1*camZ;
			break;
		case GLUT_KEY_LEFT:
			camPosition -= 0.1*camX;
			break;
		case GLUT_KEY_RIGHT:
			camPosition += 0.1*camX;
			break;
	}

	glutPostRedisplay();
}

void display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	ctmat = camMat*LookAt(camPosition, camPosition - camZ, camY);

	mat4 saved_mat = ctmat;
	int i, j;

	for (i=0;i<numHouses;i++)
	{
		ctmat = saved_mat;
		ctmat *= houses[i].translation;
		ctmat *= houses[i].rotation;
		ctmat *= houses[i].scale;

		for (j=0;j<8;j++)
		{
			glUniform4fv(color_loc, 1, houseColor[j]);
			glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, ctmat);
			glDrawElements(GL_TRIANGLE_STRIP, houseVerts[j], GL_UNSIGNED_SHORT, houseIndex[j]);
		}
	}

	for (i=0;i<numMountains;i++)
	{
		ctmat = saved_mat;
		ctmat *= mountains[i].translation;
		ctmat *= mountains[i].rotation;
		ctmat *= mountains[i].scale;
		for (j=0;j<4;j++)
		{
			glUniform4fv(color_loc, 1, mountainColor[j]);
			glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, ctmat);
			glDrawElements(GL_TRIANGLE_STRIP, mountainVerts[j], GL_UNSIGNED_SHORT, mountainIndex[j]);
		}
	}
	for (i=0;i<numTrees;i++)
	{
		ctmat = saved_mat;
		ctmat *= trees[i].translation;
		ctmat *= trees[i].rotation;
		ctmat *= trees[i].scale;
		for (j=0;j<12;j++)
		{
			glUniform4fv(color_loc, 1, treeColor[j]);
			glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, ctmat);
			glDrawElements(GL_TRIANGLE_STRIP, treeVerts[j], GL_UNSIGNED_SHORT, treeIndex[j]);
		}
	}

	glutSwapBuffers();
}


int main( int argc, char **argv )	
{
	srand(time(NULL));

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize( 512, 512 );

    int noscaled = glutCreateWindow( "CS 537 Homework 3" );
	glutInitWindowPosition(50, 50);
	m_glewInitAndVersion();

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}


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