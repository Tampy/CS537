#include "proj2.h"

void randomDisplacement(GLfloat magnitude, GLfloat &side1, GLfloat &side2, GLfloat &side3)
{
	GLfloat angle = ((GLfloat)rand()/(GLfloat)RAND_MAX) * (2 * PI);
	side1 = magnitude * cos(angle);
	side2 = magnitude * sin(angle);
	side3 = magnitude * tan(angle);
}

//counts the numbere of points after the provided pointNode
int pointCount(struct pointNode* head)
{
	pointNode* tmp;
	tmp = head;
	int count = 0;

	while(tmp != NULL)
	{
		count++;
		tmp = tmp->next;
	}
	return count;
}

pointNode* getRandomStart()
{
	struct pointNode* retVal;
	GLfloat xLen = maxX - minX;
	GLfloat yLen = maxY - minY;
	GLfloat zLen = maxZ - minZ;

	GLfloat startX = ((GLfloat)rand()/(GLfloat)RAND_MAX) * xLen + minX;
	GLfloat startY = ((GLfloat)rand()/(GLfloat)RAND_MAX) * xLen + minX;
	GLfloat startZ = ((GLfloat)rand()/(GLfloat)RAND_MAX) * xLen + minX;

	retVal = (pointNode*)malloc(sizeof(pointNode));
	retVal->x = startX;
	retVal->y = startY;
	retVal->z = startZ;
	retVal->next = NULL;

	return retVal;
}

pointNode* AddNode(struct pointNode* node, GLfloat x, GLfloat y, GLfloat z)
{
	struct pointNode* tmp = NULL;
	while(node->next != NULL)
	{
		node = node->next;
	}

	tmp = (pointNode *) malloc(sizeof(pointNode));
	tmp->x = x;
	tmp->y = y;
	tmp->z = z;

	tmp->next = NULL;
	node->next = tmp;

	return tmp;
}

//this function will determine the length of the displacement vectors
//it will be 1/50 the shortest side.
GLfloat calcDisplacement()
{
	GLfloat lenX = maxX - minX;
	GLfloat lenY = maxY - minY;
	GLfloat lenZ = maxZ - minZ;

	if(lenX < lenY && lenX < lenZ)
		return lenX / 50.0;
	else if (lenY < lenX && lenY < lenZ)
		return lenY / 50.0;
	else
		return lenZ / 50.0;
}

void animate(int i)
{
	GLfloat x, y, z;
	int count, exitPt;
	pointNode * last;
	if(!pause && !stopFly)
	{
		GLfloat displacement = calcDisplacement();
		
		last = curr;
		randomDisplacement(displacement, x, y, z);
		curr = AddNode(curr, curr->x + x, curr->y + y, curr->z + z);
		count = pointCount(head);

		glutPostRedisplay();
		//We only want to keep going if 
		if(checkNode(curr) == 1)
		{
			//keep a roughly constat fps
			glutTimerFunc(17, animate, 0);
		}
		else
		{
			exitPt = findExitPoint(last, curr);
			colors[exitPt] = edgeColor;
			stopFly = true;
		}
	}
}

bool checkNode(struct pointNode * curr)
{
	if(curr->x < minX || curr->y < minY || curr->x > maxX || curr->y > maxY || curr->z < minZ || curr->z > maxZ )
	{
		return false;
	}
	else
	{
		return true;
	}
}

int findExitPoint(struct pointNode * prev, struct pointNode * last)
{
	vec3 crossPt, test, point1, point2, point3;
	int i, outside;
		
	test = vec3(curr->x, curr->y, curr->z);
	
	outside = -1;

	if (curr->z > vertices[0].z)
		outside = 0;
	else if (curr->z < vertices[1].z)
		outside = 1;
	else 
	{
		for (i=2;i<numSides+2;i++)
		{
			point1 = vec3(vertices[sides[i][0]].x, vertices[sides[i][0]].y, vertices[sides[i][0]].z);
			point2 = vec3(vertices[sides[i][1]].x, vertices[sides[i][1]].y, vertices[sides[i][1]].z);
			point3 = vec3(vertices[sides[i][2]].x, vertices[sides[i][2]].y, vertices[sides[i][2]].z);
			crossPt = cross(point3 - point1, point2 - point1);
			if (dot(point1-test, crossPt) >= 0)
			{
				outside = i;
				break;
			}
		}
	}
	
	return outside;
	/* Non-working code
	GLfloat slope;
	GLfloat b; // y -intercept of  the line y = slope*x + b

	GLfloat yExit;
	GLfloat xExit;
	
	//this will make sure one of the nodes is inside the other is outside
	if(checkNode(prev) && !checkNode(last))
	{
		slope = (last->y - prev->y)/(last->x - prev->x);
		b = prev->y - slope*prev->x;

		if(last->x > maxX)
		{
			yExit = slope * maxX + b;
			xExit = maxX;
			if(yExit < minY)
			{
				yExit = minY;
				xExit = (yExit - b) / slope;
			}
			if(yExit > maxY)
			{
				yExit = maxY;
				xExit = (yExit - b) / slope;
			}
		}
		else if(last->x < minX)
		{
			yExit = slope * minX + b;
			xExit = minX;
			if(yExit < minY)
			{
				yExit = minY;
				xExit = (yExit - b) / slope;
			}
			if(yExit > maxY)
			{
				yExit = maxY;
				xExit = (yExit - b) / slope;
			}
		}
		else if(last->y > maxY)
		{
			yExit = maxY;
			xExit = (yExit - b) / slope;
		}
		else if(last->y < minY)
		{
			yExit = minY;
			xExit = (yExit - b) / slope;
		}
		return 1;
	}
	return 0;*/
}

GLfloat* copyToArray(struct pointNode * head)
{
	GLfloat * retVal; 
	pointNode * tmp;
	count = pointCount(head);
	int i = 0;
	
	count *= 3;

	tmp = head;
	if(count > 0 )
	{
		//retVal = (GLfloat *)malloc(sizeof(GLfloat) * count);
		retVal = new GLfloat[count];
	}
	else
	{
		return NULL;
	}

	while(i < count)
	{
		retVal[i] = tmp->x;
		retVal[i+1] = tmp->y;
		retVal[i+2] = tmp->z;
		tmp = tmp->next;
		i+=3;
	}
	return retVal;
}
void init()
{
	int i, j;
	GLfloat theta, rotateTheta, x, y, num_lines;
	num_lines = numSides + 2;
	vertices = (vec4*)malloc(numVerts*sizeof(vec4));
	sides = (GLuint**)malloc((num_lines)*sizeof(GLuint*));
	edges = (GLuint*)malloc((5*numSides+1) * sizeof(GLuint));

	for(i=0; i < num_lines;i++)
		colors[i] = faceColor;

	theta = 0;
	rotateTheta = (2.0 * PI) / (GLfloat) numSides;

	for(i=0; i < numVerts; i += 2)
	{
		x = cos(theta);
		y = sin(theta);

		vertices[i] = vec4(x, y, maxZ, 1.0);
		vertices[i + 1] = vec4(x, y, minZ, 1.0);

		theta += rotateTheta;
	}

	for(i=0; i < num_lines - 1; i++)
	{
		if(i < 2)
		{
			sides[i] = new GLuint[numSides];
			for(j=0; j < numSides;j++)
				sides[i][j] = j*2+1;
		}
		else
		{
			sides[i] = new GLuint[4];
			sides[i][0] = 2*(i-2);
			sides[i][1] = 2*(i-2)+1;
			sides[i][2] = 2*(i-2)+2;
			sides[i][3] = 2*(i-2)+3;
		}
	}
	
	sides[i] = new GLuint[4];
	sides[i][0] = (numSides - 1)*2;
	sides[i][1] = (numSides - 1)*2+1;
	sides[i][2] = 0;
	sides[i][3] = 1;

	edges[0] = 0;

	for (i = 0, j = 1; j < 46 ; 1)
	{
		edges[j++] = (i + 1) % numVerts;
		edges[j++] = (i + 3) % numVerts;
		edges[j++] = (i + 2) % numVerts;
		edges[j++] = (i) % numVerts;
		edges[j++] = (i += 2) % numVerts;
	}

	calcDisplacement();
	//Start the Brownian motion
	head = getRandomStart();
	curr = head;

	// Load shaders and use the resulting shader program
	program = InitShader( "vshader23_v110.glsl","fshader23_v110.glsl" );
	glUseProgram( program );

	glGenBuffers( 2, buffers );

	glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(vec4), vertices, GL_STATIC_DRAW);

	modelview_loc = glGetUniformLocation(program, "modelview");
	draw_color_loc = glGetUniformLocation(program, "vColor");

	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	width = maxX - minX;
	height = maxY - minY;

	//Mark initial location
	modelview = Angel::mat4(1.0)*Angel::Translate(vec4(head->x,head->y, 0.0, 0.0))*Angel::Scale(width/25.0, height/25.0, 1.0);
	glUniformMatrix4fv(modelview_loc, 1, GL_TRUE, modelview);
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glUniform4fv(draw_color_loc, 1, green_start_marker);
	glDrawArrays(GL_QUADS, 0, 4);

	//Draw trajectory
	if(startFly)
	{
		modelview = perspective * Translate(0.0f, 0.0f, -5.0f) * rotation * Translate(vec4(curr->x,curr->y, curr->z, 0.0));
		glUniform4fv(draw_color_loc, 1, green_start_marker);
		glUniformMatrix4fv(modelview_loc, 1, GL_TRUE, modelview);
		glutSolidCube(0.05f);
	}
	//copy the trajectory into a buffer 
	GLfloat * trajectoryBuffer = copyToArray(head);
	glBindBuffer( GL_ARRAY_BUFFER, buffers[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat)*3*pointCount(head), trajectoryBuffer, GL_STREAM_DRAW);
	glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glUniform4fv(draw_color_loc, 1, blue_trajectory);
	glDrawArrays(GL_LINE_STRIP, 0, pointCount(head));

	glutSwapBuffers();

	delete[] trajectoryBuffer;
}

void display()
{
	int i;
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );

	modelview = perspective * Translate(0.0f, 0.0f, -5.0f) * rotation;
	glUniformMatrix4fv(modelview_loc, 1, GL_TRUE, modelview);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	
	//Faces of the object
	for (i=0;i<numSides+2;i = i+1)
	{
		glUniform4fv(draw_color_loc, 1, colors[i]);
		glUniformMatrix4fv(modelview_loc, 1, GL_TRUE, modelview);
		if (i<2)
			glDrawElements(GL_POLYGON, numSides, GL_UNSIGNED_INT, sides[i]);
		else
			glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, sides[i]);
	}
	
	//Edges
	glUniform4fv(draw_color_loc, 1, edgeColor);
	glDrawElements(GL_LINE_STRIP, numSides * 5 + 1, GL_UNSIGNED_INT, edges);

	glutSwapBuffers();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':
	case 'S':
		glutTimerFunc(0, animate, 0);
		pause = false;
		startFly = true;
		break;
	case 'i':
	case 'I':
		pause = !pause;
		glutTimerFunc(0, animate, 0);
		break;
	}
}

static void mouse(int button, int state, int x, int y)
{

	if(button == GLUT_LEFT)
	{
		if (state == GLUT_DOWN)
		{
			moveCamera = 1;
			startX = x;
			startY = y;
		}
		if(state == GLUT_UP)
			moveCamera = 0;
	}
}


static void motion(int x, int y)
{
	if (moveCamera)
	{
		angle = angle + (x - startX);
		angle2 = angle2 + (y - startY);
		rotation = RotateX(angle2) * RotateY(angle);
		startX = x;
		startY = y;
		glutPostRedisplay();
	}
}

void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	perspective = Ortho(-2.0, 2.0, -2.0, 2.0, 2.0, 10.0);
}

int main(int argc, char** argv)
{
	srand((unsigned int) time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowSize (256, 256); 

	glutCreateWindow ("Project 2 Question 1");
	glutInitWindowPosition (100, 100);
	m_glewInitAndVersion();
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutMainLoop();
	return 0;
}

void m_glewInitAndVersion(void)
{
   fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
   fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
   GLenum err = glewInit();
   if (GLEW_OK != err)
	fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}
