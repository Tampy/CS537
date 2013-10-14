//
// A GLSL "Small Labirynth" 
// On a 3 by 3 grid
//
// We use the default orthographic projection
//
//GLSL version 110



#include "Angel.h"
#include "hwk1.h"

void m_glewInitAndVersion(void);

GLfloat t1 = -0.5;
GLfloat t2 = 0.0;
GLfloat t3 = 0.5;

int randomExit1;
int randomExit2;

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

//several inner walls
GLuint buffers[6];
GLuint exitBuffer[16];
GLuint vPosition;


//line segment 0
point4 points0[2] = {
    point4(-1.0, t2,  0.0, 1.0 ),
    point4(t1, t2,  0.0, 1.0 )
};

//line segment 1
point4 points1[4] = {
	point4(t1, -1.0, 0.0, 1.0),
	point4(t1, t1, 0.0, 1.0),
	point4(t2, t1, 0.0, 1.0),
	point4(t2, t2, 0.0, 1.0)
};

//line segment 2
point4 points2[3] = {
	point4(t1, 1.0, 0.0, 1.0),
	point4(t1, t3, 0.0, 1.0),
	point4(t2, t3, 0.0, 1.0)
};

//line segment 3
point4 points3[2] = {
	point4(t3, 1.0, 0.0, 1.0),
	point4(t3, t3, 0.0, 1.0)
};

//line segment 4
point4 points4[2] = {
	point4(1.0, t2, 0.0, 1.0),
	point4(t3, t2, 0.0, 1.0)
};

//line segment 5
point4 points5[2] = {
	point4(t3, -1.0, 0.0, 1.0),
	point4(t3, t1, 0.0, 1.0)
};

// RGBA colors
color4 blue_opaque = color4( 0.0, 0.0, 1.0, 1.0 );

//uniform variable locations
GLuint color_loc;
GLuint model_view_loc;

mat4 ctmat = Angel::mat4(1.0);

//----------------------------------------------------------------------------
void randomDisplacement(GLfloat magnitude, GLfloat &side1, GLfloat &side2)
{
	GLfloat angle = ((GLfloat)rand()/(GLfloat)RAND_MAX) * (2 * PI);
	side1 = magnitude * cos(angle);
	side2 = magnitude * sin(angle);
}

//this will count all the nodes after head;
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


pointNode* getRandomStart(GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax)
{
	struct pointNode* retVal;
	GLfloat xLen = xMax - xMin;
	GLfloat yLen = yMax - yMin;

	GLfloat startX = ((GLfloat)rand()/(GLfloat)RAND_MAX) * xLen + xMin;
	GLfloat startY = ((GLfloat)rand()/(GLfloat)RAND_MAX) * xLen + xMin;

	retVal = (pointNode*)malloc(sizeof(pointNode));
	retVal->x = startX;
	retVal->y = startY;
	retVal->next = NULL;

	return retVal;

}

pointNode* AddNode(struct pointNode* node, GLfloat x, GLfloat y)
{
	struct pointNode* tmp = NULL;
	while(node->next != NULL)
	{
		node = node->next;
	}
	
	tmp = (pointNode *) malloc(sizeof(pointNode));
	tmp->x = x;
	tmp->y = y;

	tmp->next = NULL;
	node->next = tmp;

	return tmp;
}

//this function will determine the length of the displacement vectors
//it will be 1/50 the shortest side.
GLfloat calcDisplacement(GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax)
{
	GLfloat lenX = xMax - xMin;
	GLfloat lenY = yMax - yMin;

	if(lenX < lenY)
	{
		return lenX/50.0;
	}
	else
	{
		return lenY/50.0;
	}
}

//this function will check to see if the node is withing the box
bool checkNode(struct pointNode * curr, GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax)
{
	if(curr->x < xMin || curr->y < yMin || curr->x > xMax || curr->y > yMax )
	{
		return false;
	}
	else
	{
		return true;
	}
}

void collisionDetect(struct pointNode * curr)
{
	GLfloat xColli = curr->x;
	GLfloat yColli = curr->y;
	
	if(xColli == t1)
		if(yColli > 0.0 && yColli < t1 || yColli < 1.0 && yColli > t3)
			collision();
	if(xColli == t2)
		if(yColli > t1 && yColli < t2)
			collision();
	if(xColli == t3)
		if(yColli > 0.0 && yColli < t1 || yColli < 1.0 && yColli > t3)
			collision();
	if(yColli == t1)
		if(xColli > t1 && xColli < t2)
			collision();
	if(yColli == t2)
		if(xColli > 0.0 && xColli < t1 || xColli > t3 && xColli < 1.0)
			collision();
	if(yColli == t3)
		if(xColli > t1 || xColli < t2)
			collision();
}


GLfloat* copyToArray(struct pointNode * head)
{
	GLfloat * retVal; 
	pointNode * tmp;
	count = pointCount(head);
	int i = 0;
	
	count *= 2;

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
		tmp = tmp->next;
		i+=2;
	}
	return retVal;
}

void animate(int i)
{
	GLfloat x, y;
	pointNode * last;
	if(!bPaused && !bComplete)
	{
		GLfloat displacement = calcDisplacement(minX, maxX, minY, maxY);
		
		last = curr;
		randomDisplacement(displacement, x, y);
		curr = AddNode(curr, curr->x + x, curr->y + y);
		count = pointCount(head);

		glutPostRedisplay();
		//We only want to keep going if 
		if(checkNode(curr, minX, maxX, minY, maxY))
		{
			//keep a roughly constat fps
			glutTimerFunc(17, animate, 0);
			collisionDetect(curr);
		}else
		{
			collisionDetect(curr);
			findExitPoint(last, curr);
			bComplete = true;
		}
	}
}

void keyboard (unsigned char key, int x, int y)
{
	if(key == 'p')
	{
		bPaused = !bPaused;
		if(!bPaused)
			animate(0);
	}
}

void findExitPoint(struct pointNode * prev, struct pointNode * last)
{
	GLfloat slope;
	GLfloat b; // y -intercept of  the line y = slope*x + b

	GLfloat yExit;
	GLfloat xExit;
	
	//this will make sure one of the nodes is inside the other is outside
	if(checkNode(prev, minX, maxX, minY, maxY) && !checkNode(last, minX, maxX, minY, maxY))
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
		
	}
}

void collision()
{
	exit(0);
}
//----------------------------------------------------------------------------

void makeExit()
{
	GLuint endLoop = 0;
	GLfloat x1, y1, x2, y2;
	x1 = -1.0;
	y1 = -1.0;
	x2 = -1.0;
	y2 = -0.5;

	glGenBuffers( 16, &exitBuffer[0] );

	randomExit1 = rand() % 16;
	randomExit2 = rand() % 16;

	while(randomExit1 == randomExit2)
		randomExit2 = rand() % 16;
	for(int i=0;i<16;i++)
	{
		if(i != randomExit1 && i != randomExit2)
		{
			point4 exitWalls[2] = {
				point4(x1, y1, 0.0, 1.0),
				point4(x2, y2, 0.0, 1.0)
			};
			
			glBindBuffer( GL_ARRAY_BUFFER, exitBuffer[i] );
			glBufferData( GL_ARRAY_BUFFER, sizeof(exitWalls),  exitWalls, GL_STATIC_DRAW );
		}
		if(x1 == -1.0 && x2 == -1.0 && endLoop != 1)
		{
			if(y2 != 1.0)
			{
				y1 = y2;
				y2 = y2 + t3;
			}
			else if(y2 == 1.0)
			{
				y1 = y2;
				x2 = x2 + t3;
			}
		}
		else if(y1 == 1.0 && y2 == 1.0)
		{
			if(x2 != 1.0)
			{
				x1 = x2;
				x2 = x2 + t3;
			}
			else if(x2 == 1.0)
			{
				x1 = x2;
				y2 = y2 - t3;
			}
		}
		else if(x1 == 1.0 && x2 == 1.0)
		{
			if(y2 != -1.0)
			{
				y1 = y2;
				y2 = y2 - t3;
			}
			else if(y2 == -1.0)
			{
				y1 = y2;
				x2 = x2 - t3;
			}
		}
		else if(y1 == -1.0 && y2 == -1.0)
		{
			if(x2 != -1.0)
			{
				x1 = x2;
				x2 = x2 - t3;
			}
			else if(x2 == -1.0)
			{
				x1 = x2;
				x2 = x2 - t3;
				endLoop = 1;
			}
		}
	}
}
				
//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	
	makeExit();

    glClearColor (0.0, 0.0, 0.0, 0.0);

    //Starte the Brownian motion
	head = getRandomStart(minX, maxX, minY, maxY);
	curr = head;

		
	glGenBuffers( 2, &flyBuffers[0] );
    glGenBuffers( 6, &buffers[0] );

	glBindBuffer( GL_ARRAY_BUFFER, flyBuffers[0] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(bBox), bBox, GL_STATIC_DRAW );
	
    glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points0),  points0, GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, buffers[1] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points1),  points1, GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, buffers[2] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points2),  points2, GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, buffers[3] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points3),  points3, GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, buffers[4] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points4),  points4, GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, buffers[5] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points5),  points5, GL_STATIC_DRAW );

   // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader00_v110.glsl", "fshader00_v110.glsl" );
    glUseProgram( program );
	projmat_loc = glGetUniformLocation(program, "projmat");
	modelview_loc = glGetUniformLocation(program, "modelview");
	draw_color_loc = glGetUniformLocation(program, "vColor");
    
    vPosition = glGetAttribLocation( program, "vPosition" );
	
    glEnableVertexAttribArray( vPosition );
    
	GLuint flyVPosition = glGetAttribLocation( program, "flyVPosition" );
	glEnableVertexAttribArray( flyVPosition );
    glVertexAttribPointer( flyVPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	
	width = maxX - minX;
	height = maxY - minY;
	
	color_loc = glGetUniformLocation(program, "color");
	model_view_loc = glGetUniformLocation(program, "modelview");

    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------


void
display1( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUniform4fv(color_loc, 1, blue_opaque);
	
	ctmat = Angel::mat4(1.0);
	projmat = Angel::mat4(1.0);

	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, ctmat);

	glBindBuffer( GL_ARRAY_BUFFER, flyBuffers[0] );

	for(int i = 0; i<16;i++)
	{
		if(i != randomExit1 && i != randomExit2)
		{
			glBindBuffer( GL_ARRAY_BUFFER, exitBuffer[i] );
			glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
			glDrawArrays( GL_LINE_STRIP, 0, 2);
		}
	}

	glBindBuffer( GL_ARRAY_BUFFER, buffers[5] );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glDrawArrays( GL_LINE_STRIP, 0, 2);

	glBindBuffer( GL_ARRAY_BUFFER, buffers[4] );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glDrawArrays( GL_LINE_STRIP, 0, 2);

	glBindBuffer( GL_ARRAY_BUFFER, buffers[3] );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glDrawArrays( GL_LINE_STRIP, 0, 2);

	glBindBuffer( GL_ARRAY_BUFFER, buffers[2] );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glDrawArrays( GL_LINE_STRIP, 0, 3);

	glBindBuffer( GL_ARRAY_BUFFER, buffers[1] );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glDrawArrays( GL_LINE_STRIP, 0, 4);

	glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glDrawArrays( GL_LINE_STRIP, 0, 2 );

	//Draw box
	modelview = Angel::mat4(1.0);
	glUniformMatrix4fv(modelview_loc, 1, GL_TRUE, modelview);
	glVertexAttribPointer( flyVPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glUniform4fv(draw_color_loc, 1, yelow_box_edge);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	//Mark initial location
	modelview = Angel::mat4(1.0)*Angel::Translate(vec4(head->x,head->y, 0.0, 0.0))*Angel::Scale(width/25.0, height/25.0, 1.0);
	glUniformMatrix4fv(modelview_loc, 1, GL_TRUE, modelview);
	glVertexAttribPointer( flyVPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glUniform4fv(draw_color_loc, 1, green_start_marker);
	glDrawArrays(GL_QUADS, 0, 4);

	//Draw trajectory
	modelview = Angel::mat4(1.0);
	glUniformMatrix4fv(modelview_loc, 1, GL_TRUE, modelview);
	//copy the trajectory into a buffer 
	GLfloat * trajectoryBuffer = copyToArray(head);
	glBindBuffer( GL_ARRAY_BUFFER, flyBuffers[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat)*2*pointCount(head), trajectoryBuffer, GL_STREAM_DRAW);
	glVertexAttribPointer( flyVPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glUniform4fv(draw_color_loc, 1, blue_trajectory);
	glDrawArrays(GL_LINE_STRIP, 0, pointCount(head));
	
	glutSwapBuffers();

	delete[] trajectoryBuffer;

	//glFlush();
}

//----------------------------------------------------------------------------
/*
void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033:  // Escape key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
    }
}
*/
//----------------------------------------------------------------------------

int
main( int argc, char **argv )	
{
	srand(time(NULL));

	struct pointNode* tmp = NULL;

	height = maxY - minY;
	width  =  maxX - minX;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize( 256, 256 );

    int noscaled = glutCreateWindow( "CS 537 Homework 1 Question 3" );
	glutInitWindowPosition(50, 50);
	m_glewInitAndVersion();
	init();
	glutDisplayFunc( display1 );

	glutTimerFunc(1000, animate, 0);
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