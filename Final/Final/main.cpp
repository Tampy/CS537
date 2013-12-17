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
	skyboxInit();

	//set up camera
  eye.x += 0.15;
  eye.y -= 1.75;
  eye.z -= 5.0;
  yaw(-ROTATE * 32);

  glUseProgram(program[0]);
  // Load the OBJ models from file
  models[0] = glmReadOBJ("car/car.obj");
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
  program[0] = InitShader( "./shaders/shader_vert.glsl", "./shaders/shader_frag.glsl" ); 
  glUseProgram( program[0] );
  // set up vertex arrays
  glBindVertexArray( models[0]->vao );

  glUseProgram( program[0] );

  glUniform4fv( glGetUniformLocation(program[0], "light_ambient"),1, light_ambient);
  glUniform4fv( glGetUniformLocation(program[0], "light_diffuse"),1, light_diffuse);
  glUniform4fv( glGetUniformLocation(program[0], "light_specular"),1, light_specular);	
  glUniform4fv( glGetUniformLocation(program[0], "lightPosition"),1, light_position_distant );
  // Retrieve transformation uniform variable locations
  ModelViewCam = glGetUniformLocation( program[0], "modelView" );
  ModelViewObj = glGetUniformLocation(program[0], "ModelViewObj");
  Projection = glGetUniformLocation( program[0], "projection" );
  NormalTransformation = glGetUniformLocation( program[0], "normalTransformation" );

  glClearDepth( 1.0 ); 
  glEnable( GL_DEPTH_TEST );
  glDepthFunc(GL_LESS);
  glClearColor( 0.0, 0.0, 0.0, 1.0 );
}

void skyboxInit()
{
	glGenBuffers( 4, buffers );
	//Vertex buffer for the vertex coordinates

	glBindBuffer( GL_ARRAY_BUFFER, buffers[cube_vertices_explicit] ); 
	glBufferData( GL_ARRAY_BUFFER, sizeof(cube_vertices_positions), cube_vertices_positions, GL_STATIC_DRAW );

	std::cout<< "sizeof(cube_vertices_positions)" << sizeof(cube_vertices_positions) << std::endl;

	//Elements buffer for the pointers
	 glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[cube_indeces] ); 
	 glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

	 glGenVertexArrays(2, VAO);

// Load shaders and use the resulting shader programs
    
	program[1] = InitShader( "./shaders/skyboxvertex.glsl", "./shaders/skyboxfragment.glsl" ); 

	//VAO[1] the skybox
	glUseProgram( program[1] );
	glBindVertexArray(VAO[1]);
	glBindBuffer( GL_ARRAY_BUFFER, buffers[cube_vertices_explicit] ); 
	vPosition = glGetAttribLocation( program[1], "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
   	//done with this packet
    glBindVertexArray(0); 
	glUseProgram(0);

	vPosition = glGetAttribLocation( program[0], "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
   	//done with this packet
    glBindVertexArray(0); 

	glUseProgram(0);
	
	//Load Skybox Images. 6 images to represent the 6 angles of view. Inside it's own structured Cubemap
    skybox.top = glmReadPPM("skybox\\sky-top.ppm", &skybox.topWidth, &skybox.topHeight);
	skybox.bottom = glmReadPPM("skybox\\grass-bottom.ppm", &skybox.bottomWidth, &skybox.bottomHeight);
    skybox.right = glmReadPPM("skybox\\sky-right.ppm", &skybox.rightWidth, &skybox.rightHeight);
    skybox.left = glmReadPPM("skybox\\sky-left.ppm", &skybox.leftWidth, &skybox.leftHeight);
    skybox.front = glmReadPPM("skybox\\sky-front.ppm", &skybox.frontWidth, &skybox.frontHeight);
    skybox.back = glmReadPPM("skybox\\sky-back.ppm", &skybox.backWidth, &skybox.backHeight);
    
   	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &texture);

	int isEnabled=0; 
	
	if (glIsEnabled(GL_TEXTURE_CUBE_MAP) == GL_TRUE) {isEnabled = 1;} else {isEnabled = 0;};

	std::cout << isEnabled << std::endl;

	glEnable(GL_TEXTURE_CUBE_MAP);

	if (glIsEnabled(GL_TEXTURE_CUBE_MAP) == GL_TRUE) {isEnabled = 1;} else {isEnabled = 0;};

	std::cout << isEnabled << std::endl;

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, skybox.topWidth, skybox.topHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE,skybox.top);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, skybox.bottomWidth, skybox.bottomHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE,skybox.bottom);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, skybox.rightWidth, skybox.rightHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE,skybox.right);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, skybox.leftWidth, skybox.rightWidth, 0,
                 GL_RGB, GL_UNSIGNED_BYTE,skybox.left);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, skybox.frontWidth, skybox.frontHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE,skybox.front);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, skybox.backWidth, skybox.backHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE,skybox.back);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	displaySkybox();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

  // Setup camera
  mat4 modelViewCamera = LookAt(eye, eye-n, v)*model_view;
  glUniformMatrix4fv(ModelViewCam, 1, GL_TRUE, modelViewCamera);
  glUseProgram(program[0]);
  

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
  modelViewObject = Translate(0.0, 0.0, 5.0)*RotateX( xrot ) * RotateY( yrot )*scaleTransformation;
  glUniformMatrix4fv(NormalTransformation , 1, GL_TRUE,  normalMatrix);
  glUniformMatrix4fv( ModelViewObj, 1, GL_TRUE, modelViewObject );
  glmDrawVBO(models[0], program[0]);

  glutSwapBuffers();
}

void displaySkybox()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);

//Make sure you start with the Default Coordinate Systems
	projmat=mat4(1.0);
	model_view = mat4(1.0);
	
//Set up the camera optics
	projmat = projmat*Perspective(90,1.0,0.1,20.0); 

//Sky Box @  infinity
	vec4 skyboxEye = vec4(0.0, 0.0, 0.0, 1.0);
	model_view = LookAt(skyboxEye, skyboxEye-n, v);

	glUseProgram(program[1]);
	proj_loc       = glGetUniformLocation(program[1], "Projection");
	model_view_loc = glGetUniformLocation(program[1], "ModelView");
	glUniformMatrix4fv(proj_loc, 1, GL_TRUE, projmat);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_view);
	glUniform1i(glGetUniformLocation(program[1], "CubeMap"), 0);

	glBindVertexArray(VAO[1]);

	glCullFace(GL_BACK);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glDrawArrays(GL_QUADS, 0, 24);

	glUseProgram(0);

	model_view = Angel::mat4(1.0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'v':
			if(!heliCam)
			{
				pitch(1.0);
				eye += ROTATE * n * 100;
				heliCam = true;
			}
			break;
		case 'V':
			if(heliCam)
			{
				eye -= ROTATE * n * 100;
				pitch(-1.0);
				heliCam = false;
			}
			break;
		//debugging cases
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
		case 't':
			eye.x -= 0.15;
			break;
		case 'T':
			eye.x += 0.15;	//+.15, -1.75, -5
			break;
		case 'y':
			eye.y -= 1.75;
			break;
		case 'Y':
			eye.y += 1.75;
			break;
		case 'u':
			eye.z -= 5.0;
			break;
		case 'U':
			eye.z += 5.0;
			break;
	}
	glutPostRedisplay();
}

void arrow(int key, int x, int y)
{
	switch(key)
	{
		//camera debugging cases
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
	glutPostRedisplay();
}

void idle()
{
	glutPostRedisplay();
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
	glutIdleFunc(idle);

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