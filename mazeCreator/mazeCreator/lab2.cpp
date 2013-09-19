//
// A GLSL "Small Labirynth" 
// On a 3 by 3 grid
//
// We use the default orthographic projection
//
//GLSL version 110



#include "Angel.h"
void m_glewInitAndVersion(void);

GLfloat t1 = 1.0/4.0;
GLfloat t2 = 2.0/4.0;
GLfloat t3 = 3.0/4.0;

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

bool rightClicked;

//two walls: one has 1 segment the other has 7 segments
GLuint squareBuffers[16];
GLuint vPosition;
GLuint numSquares = 0;

// RGBA colors
color4 lineColor = color4( 1.0, 0.0, 0.0, 1.0 );

//uniform variable locations
GLuint color_loc;
GLuint model_view_loc;

mat4 ctmat = Angel::mat4(1.0);

//---------------------------------------------------------------------------
void drawSquares(GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1, GLint xlimit, GLint ylimit)
{
	glGenBuffers(16, &squareBuffers[0]);
	//creates square cells
	GLfloat width = (x1-x0)/xlimit;
	GLfloat height = (y1-y0)/ylimit;
	GLint x, y;
	for(y = 0; y < ylimit; y++) {
		for (x = 0; x < xlimit; x++) {
			point4 square[5] = {
				point4(x0 + (x * width), y0 + (y * height), 0.0, 1.0),
				point4(x0 + (x * width), y0 + ((y+1) * height), 0.0, 1.0),
				point4(x0 + ((x+1) * width), y0 + ((y+1) * height), 0.0, 1.0),
				point4(x0 + ((x+1) * width), y0 + (y * height), 0.0, 1.0),
				point4(x0 + (x * width), y0 + (y * height), 0.0, 1.0)
			};


			glBindBuffer( GL_ARRAY_BUFFER, squareBuffers[numSquares] );
			glBufferData( GL_ARRAY_BUFFER, sizeof(square),  square, GL_STATIC_DRAW );

			numSquares++;
		}
	}
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	
	numSquares = 0;

	drawSquares(-1, -1, 1, 1, 4, 4);

    // Create and initialize a buffer object

   // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader00_v110.glsl", "fshader00_v110.glsl" );
    glUseProgram( program );

    
    vPosition = glGetAttribLocation( program, "vPosition" );
	
    glEnableVertexAttribArray( vPosition );
    
	
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

	glUniform4fv(color_loc, 1, lineColor);
	
	ctmat = Angel::mat4(1.0);

	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, ctmat);

	for(numSquares = 0; numSquares < 16; numSquares++) {
		glBindBuffer( GL_ARRAY_BUFFER, squareBuffers[numSquares] );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		glDrawArrays( GL_LINE_STRIP, 0, 5);
	};
	
	glFlush();
}



//----------------------------------------------------------------------------

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

//----------------------------------------------------------------------------

void
mouse(int button, int state, int x, int y)
{
	GLuint xLoc = -1;
	GLuint yLoc = -1;

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		xLoc = x;
		yLoc = y;
		rightClicked = true;
		std::cout<<x<<"\n";
		std::cout<<y<<"\n";
	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && rightClicked == true){
		xLoc = x;
		yLoc = y;
		rightClicked = false;
	}

	if(state == GLUT_UP)
	{
		xLoc = -1;
		yLoc = -1;
	}
}
//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize( 256, 256 );

    int noscaled = glutCreateWindow( "CS 537 Idiot's Labyrinth" );
	glutInitWindowPosition(50, 50);
	m_glewInitAndVersion();
	init();
	glutDisplayFunc( display1 );
    glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );

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