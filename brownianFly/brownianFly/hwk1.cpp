//
// CS 537: G. Kamberov
// A sample brownian motion simulation stub. 
// Game play: 
// A particle is released inside a box with corners:  
//         vec2(minX, minY), vec2(maxX, minY), vec2(maxX, maxY), vec2(minX, maxY)
//         The area of the random intial location is marked with a green square
//         The partilce trajectory is drawn until the particle hits one of the box walls.
//         The exit condition test and teh final position are computed in the fucntions animate and 
//         findExitPoint.
//
// Note that to minimize the size of the buffers all points are stored as 2d vectors. 
// The vertex shader makes sure to interopet the 2D vertex postion attributes  
// as 4D vectors. 
//
// To compile you will need the enclosed header file hwk1.h  in the same directory as this source file. 
//
//
// We use the default orthographic projection
//
//
//GLSL version 110 shaders: 
//   vshader23_v110.glsl
//   fshader23_v110.glsl
//
//
//

#include "hwk1.h"

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

void init () 
{

	glClearColor (0.0, 0.0, 0.0, 0.0);

    //Starte the Brownian motion
	head = getRandomStart(minX, maxX, minY, maxY);
	curr = head;

		
	glGenBuffers( 2, &buffers[0] );
	


    glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(bBox), bBox, GL_STATIC_DRAW );

	// Load shaders and use the resulting shader program
    program = InitShader( "vshader23_v110.glsl","fshader23_v110.glsl" );
    glUseProgram( program );
	projmat_loc = glGetUniformLocation(program, "projmat");
	modelview_loc = glGetUniformLocation(program, "modelview");
	draw_color_loc = glGetUniformLocation(program, "vColor");


    // set up vertex attributes arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	
	width = maxX - minX;
	height = maxY - minY;
}


void display()
{
	glClear (GL_COLOR_BUFFER_BIT);
	projmat = Angel::mat4(1.0);
	glUniformMatrix4fv(projmat_loc, 1, GL_TRUE, projmat);

	
	glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );

	//Draw box
	modelview = Angel::mat4(1.0);
	glUniformMatrix4fv(modelview_loc, 1, GL_TRUE, modelview);
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glUniform4fv(draw_color_loc, 1, yelow_box_edge);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	//Mark initial location
	modelview = Angel::mat4(1.0)*Angel::Translate(vec4(head->x,head->y, 0.0, 0.0))*Angel::Scale(width/25.0, height/25.0, 1.0);
	glUniformMatrix4fv(modelview_loc, 1, GL_TRUE, modelview);
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glUniform4fv(draw_color_loc, 1, green_start_marker);
	glDrawArrays(GL_QUADS, 0, 4);

	//Draw trajectory
	modelview = Angel::mat4(1.0);
	glUniformMatrix4fv(modelview_loc, 1, GL_TRUE, modelview);
	//copy the trajectory into a buffer 
	GLfloat * trajectoryBuffer = copyToArray(head);
	glBindBuffer( GL_ARRAY_BUFFER, buffers[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat)*2*pointCount(head), trajectoryBuffer, GL_STREAM_DRAW);
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glUniform4fv(draw_color_loc, 1, blue_trajectory);
	glDrawArrays(GL_LINE_STRIP, 0, pointCount(head));

	glutSwapBuffers();

	delete[] trajectoryBuffer;
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
		}else
		{
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



int main(int argc, char** argv)
{

	struct pointNode* tmp = NULL;

	srand( time(NULL));
	rand();

	height = maxY - minY;
	width  =  maxX - minX;
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (int(width*400), int(height*400)); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Brownian Motion in 1 by 1 Box");
	m_glewInitAndVersion();
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	//wait a bit before we start
	glutTimerFunc(1000, animate, 0);
	glutMainLoop();
	return 0; 
}
                                   