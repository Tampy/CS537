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
	int i, j;
	vec4 a, b, end;
	end.x = 0.0f;
	end.y = 0.0f;
	end.z = 0.0f;
	end.w = 1.0f;

	i = rand() % (num_verts);
	j = rand() % (num_verts);
	a = normalize(vertices[i]);
	b = normalize(vertices[j]);
	end = end + a * ((rand() % 100) / rand() % 100);
	end = end + b * ((rand() % 100) / rand() % 100);

	retVal = (pointNode*)malloc(sizeof(pointNode));
	retVal->x = end.x;
	retVal->y = end.y;
	retVal->z = end.z;
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
	int count, j;
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
		}else
		{
			j = findExitPoint(last, curr);
			colors[j] = edgeColor;
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
	return 0;
}

void init()
{
	int i, j;
	GLfloat theta, rotateTheta, x, y, num_lines;
	num_lines = num_sides + 2;
	vertices = (vec4*)malloc(num_verts*sizeof(vec4));
	sides = (GLuint**)malloc((num_lines)*sizeof(GLuint*));
	edges = (GLuint*)malloc((5*num_sides+1) * sizeof(GLuint));

	for(i=0; i < num_lines;i++)
		colors[i] = faceColor;

	theta = 0;
	rotateTheta = (2.0 * PI) / (GLfloat) num_sides;

	for(i=0; i < num_verts; i += 2)
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
			sides[i] = new GLuint[num_sides];
			for(j=0; j < num_sides;j++)
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
	sides[i][0] = (num_sides - 1)*2;
	sides[i][1] = (num_sides - 1)*2+1;
	sides[i][2] = 0;
	sides[i][3] = 1;

	edges[0] = 0;

	for (i = 0, j = 1; j < 46 ; 1)
	{
		edges[j++] = (i + 1) % num_verts;
		edges[j++] = (i + 3) % num_verts;
		edges[j++] = (i + 2) % num_verts;
		edges[j++] = (i) % num_verts;
		edges[j++] = (i += 2) % num_verts;
	}

	calcDisplacement();
	//Start the Brownian motion
	head = getRandomStart();
	curr = head;
	
	srand((unsigned int) time(NULL));

	// Load shaders and use the resulting shader program
	program = InitShader( "vshader23_v110.glsl","fshader23_v110.glsl" );
	glUseProgram( program );

	glGenBuffers( 2, buffers );

	glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
	glBufferData(GL_ARRAY_BUFFER, num_verts * sizeof(vec4), vertices, GL_STATIC_DRAW);

	modelview_loc = glGetUniformLocation(program, "modelview");
	draw_color_loc = glGetUniformLocation(program, "vColor");

	/** set up vertex attributes arrays*/
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	width = maxX - minX;
	height = maxY - minY;
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
	for (i=0;i<num_sides+2;i = i+1)
	{
		glUniform4fv(draw_color_loc, 1, colors[i]);
		glUniformMatrix4fv(modelview_loc, 1, GL_TRUE, modelview);
		if (i<2)
		{
			glDrawElements(GL_POLYGON, num_sides, GL_UNSIGNED_INT, sides[i]);
		}
		else
		{
			glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, sides[i]);
		}
	}
	
	//Edges
	glUniform4fv(draw_color_loc, 1, edgeColor);
	glDrawElements(GL_LINE_STRIP, num_sides * 5 + 1, GL_UNSIGNED_INT, edges);

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
			startx = x;
			starty = y;
		}
		if(state == GLUT_UP)
		{	
			moveCamera = 0;
		}
	}
}


static void motion(int x, int y)
{
	if (moveCamera)
	{
		angle = angle + (x - startx);
		angle2 = angle2 + (y - starty);
		rotation = RotateX(angle2) * RotateY(angle);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	struct pointNode* tmp = NULL;

	height = maxY - minY;
	width  =  maxX - minX;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowSize (512, 512); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Project 2 Question 1");
	m_glewInitAndVersion();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	//glutReshapeFunc(reshape);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutMainLoop();
	delete[] vertices;
	delete[] sides;
	delete[] edges;
	return 0;
}