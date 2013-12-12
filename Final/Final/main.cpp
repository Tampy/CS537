#include "final.h"

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

  // Load the OBJ models from file
	cout<<"Test\n";
  models[0] = glmReadOBJ("car/car.obj");
  cout<<"Test2\n";
  if (!models[0]) exit(0);

  // Normilize vertices
  glmUnitize(models[0]);
  // Compute facet normals
  glmFacetNormals(models[0]);
  // Comput vertex normals
  glmVertexNormals(models[0], 90.0);  
  // Load the model (vertices and normals) into a vertex buffer
  glmLoadInVBO(models[0]);

  // Setup some sample materials
  color4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
  color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
  color4 light_specular( 1.0, 1.0, 1.0, 1.0 );
  color4 material_ambient( 1.0, 0.0, 1.0, 1.0 );
  color4 material_diffuse( 1.0, 0.8, 0.0, 1.0 );
  color4 material_specular( 1.0, 0.8, 0.0, 1.0 );
  float  material_shininess = 100.0;
  color4 ambient_product = light_ambient * material_ambient;
  color4 diffuse_product = light_diffuse * material_diffuse;
  color4 specular_product = light_specular * material_specular;
  //LIGHT POSITION
  vec4 light_position_distant = vec4(1.0, -1.0, -1.0, 0.0);

  // Load shaders and use the resulting shader program
  program = InitShader( "shader_vert.glsl", "shader_frag.glsl" );
  glUseProgram( program );
  // set up vertex arrays
  glBindVertexArray( models[0]->vao );

  glUseProgram( program );

  glUniform4fv( glGetUniformLocation(program, "light_ambient"),1, light_ambient);
  glUniform4fv( glGetUniformLocation(program, "light_diffuse"),1, light_diffuse);
  glUniform4fv( glGetUniformLocation(program, "light_specular"),1, light_specular);	
  glUniform4fv( glGetUniformLocation(program, "lightPosition"),1, light_position_distant );
  // Retrieve transformation uniform variable locations
  ModelViewCam = glGetUniformLocation( program, "modelView" );
  ModelViewObj = glGetUniformLocation(program, "ModelViewObj");
  Projection = glGetUniformLocation( program, "projection" );
  NormalTransformation = glGetUniformLocation( program, "normalTransformation" );

  glClearDepth( 1.0 ); 
  glEnable( GL_DEPTH_TEST );
  glDepthFunc(GL_LESS);
  glClearColor( 0.0, 0.0, 0.0, 1.0 ); 
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Setup camera
  mat4 modelViewCamera = LookAt(eye, eye-n, v);
  glUniformMatrix4fv(ModelViewCam, 1, GL_TRUE, modelViewCamera);

 
  float scale_x, scale_y, scale_z;
  mat4 scaleTransformation;
  mat4 invScaleTranformation;
  mat4 normalMatrix;
  mat4 modelViewObject;

  // _________________________________Load the first model
  scale_x = 1;
  scale_y = 1;
  scale_z = 1;
  // Scale Transformation Matrix
  scaleTransformation = Scale(scale_x, scale_y, scale_z);
  // Inverse Scale Transformation Matrix 
  invScaleTranformation = Scale(1/scale_x, 1/scale_y, 1/scale_z);
  normalMatrix =  RotateX( xrot ) * RotateY( yrot ) * invScaleTranformation;
  modelViewObject = Translate(0.0, 0.0, 2.0)*RotateX( xrot ) * RotateY( yrot )*scaleTransformation;
  glUniformMatrix4fv( ModelViewObj, 1, GL_TRUE, modelViewObject );
  glUniformMatrix4fv(NormalTransformation , 1, GL_TRUE,  normalMatrix);
  glmDrawVBO(models[0], program);

  // _________________________________Load the second model
  scale_x = 1;
  scale_y = 1;
  scale_z = 1;
  // Scale Transformation Matrix
  scaleTransformation = Scale(scale_x, scale_y, scale_z);
  // Inverse Scale Transformation Matrix 
  invScaleTranformation = Scale(1/scale_x, 1/scale_y, 1/scale_z);
  normalMatrix = invScaleTranformation;
  modelViewObject = Translate(2, 2, 0)*scaleTransformation;
  glUniformMatrix4fv( ModelViewObj, 1, GL_TRUE, modelViewObject );
  glUniformMatrix4fv(NormalTransformation , 1, GL_TRUE,  normalMatrix);
  glmDrawVBO(models[1], program);

  // _________________________________Load the third model multiple times
  scale_x = 1;
  scale_y = 1;
  scale_z =1;
  // Scale Transformation Matrix
  scaleTransformation = Scale(scale_x, scale_y, scale_z);
  // Inverse Scale Transformation Matrix 
  invScaleTranformation = Scale(1/scale_x, 1/scale_y, 1/scale_z);
  normalMatrix = invScaleTranformation;
  glUniformMatrix4fv(NormalTransformation , 1, GL_TRUE,  normalMatrix);
  modelViewObject = Translate(-5, 0, -3)*scaleTransformation; 
  glUniformMatrix4fv( ModelViewObj, 1, GL_TRUE, modelViewObject );
  glmDrawVBO(models[0], program);

  modelViewObject = Translate(-3, 0, -3)*scaleTransformation;
  glUniformMatrix4fv( ModelViewObj, 1, GL_TRUE, modelViewObject );
  glmDrawVBO(models[0], program);

  modelViewObject = Translate(-2, 0, -5)*scaleTransformation;
  glUniformMatrix4fv( ModelViewObj, 1, GL_TRUE, modelViewObject );
  glmDrawVBO(models[0], program);


  glutSwapBuffers();
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
			eye -= ROTATE * n;
			break;
		case GLUT_KEY_DOWN:
			eye += ROTATE * n;
			break;
		case GLUT_KEY_LEFT:
			eye -= ROTATE * u;
			break;
		case GLUT_KEY_RIGHT:
			eye += ROTATE * u;
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