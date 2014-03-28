// fragment shading of sphere model

#include "Angel.h"
#include <iostream>

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

// Create camera view variables
point4 at( 0.0, 0.0, 0.0, 1.0 );
point4 eye( 0.0, 0.0, 0.0, 1.0 );
vec4   up( 0.0, 10.0, 0.0, 0.0 );

// Sperical Coordinate vector (r, theta, phi)
vec3 sphericaleye( 4.0, M_PI/4.0, M_PI/4.0 ) ;

GLfloat size=1;
GLfloat norm=1/sqrt(3.0); // 1 / sqrt(1^2 + 1^2 + 1^2)

GLfloat vertexarray[]={
	size,size,-size,
	size,-size,-size,
	-size,-size,-size,
	-size,size,-size,
	size,size,size,
	size,-size,size,
	-size,-size,size,
	-size,size,size
};

GLfloat normalarray[]={
	norm,norm,-norm,
	norm,-norm,-norm,
	-norm,-norm,-norm,
	-norm,norm,-norm,
	norm,norm,norm,
	norm,-norm,norm,
	-norm,-norm,norm,
	-norm,norm,norm
};
											
GLubyte elems[]={
	7,3,4,0,1,3,2,
	7,6,4,5,1,6,2,1
};

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    // Create a vertex array object
    GLuint vao, vbo, ebo;
    glGenVertexArrays( 1,&vao);
    glBindVertexArray( vao );

	// Create Vertex and Normal buffer
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexarray) + sizeof(normalarray),NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertexarray),vertexarray);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(vertexarray),sizeof(normalarray),normalarray);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );
	
    // set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition" );
	glVertexAttribPointer(vPosition,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
    glEnableVertexAttribArray( vPosition );

	GLuint vNormal = glGetAttribLocation(program, "vNormal" );
	glVertexAttribPointer(vNormal,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(sizeof(vertexarray)));
    glEnableVertexAttribArray( vNormal );

	// Create Element Array Buffer
	glGenBuffers(1,&ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elems),elems,GL_STATIC_DRAW);

    // Initialize shader lighting parameters for light 1
    point4 light_position1( 0.5, 1.0, -2.0, 0.0 );
    color4 light_ambient1( 0.3, 0.2, 0.2, 1.0 );
    color4 light_diffuse1( 0.9, 0.6, 0.6, 1.0 );
    color4 light_specular1( 1.0, 0.6, 0.6, 1.0 );

    // Initialize shader lighting parameters for light 2
    point4 light_position2( -0.5, -1.0, 2.0, 0.0 );
    color4 light_ambient2( 0.3, 0.2, 0.2, 1.0 );
    color4 light_diffuse2( 0.9, 0.6, 0.6, 1.0 );
    color4 light_specular2( 1.0, 0.6, 0.6, 1.0 );

    // Initialize shader material parameters for cube
    color4 material_ambient( 0.4, 0.25, 0.1, 1.0 );
    color4 material_diffuse( 0.8, 0.5, 0.2, 1.0 );
    color4 material_specular( 0.8, 0.8, 0.8, 1.0 );
    float  material_shininess = 16.0;

    color4 ambient_product1 = (light_ambient1) * material_ambient;
    color4 diffuse_product1 = (light_diffuse1) * material_diffuse;
    color4 specular_product1 = (light_specular1) * material_specular;

    color4 ambient_product2 = (light_ambient2) * material_ambient;
    color4 diffuse_product2 = (light_diffuse2) * material_diffuse;
    color4 specular_product2 = (light_specular2) * material_specular;

    glUniform4fv( glGetUniformLocation(program, "AmbientProduct1"),
		  1, ambient_product1 );
    glUniform4fv( glGetUniformLocation(program, "AmbientProduct2"),
		  1, ambient_product2 );

    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct1"),
		  1, diffuse_product1 );
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct2"),
		  1, diffuse_product2 );

    glUniform4fv( glGetUniformLocation(program, "SpecularProduct1"),
		  1, specular_product1 );
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct2"),
		  1, specular_product2 );
	
    glUniform4fv( glGetUniformLocation(program, "LightPosition1"),
		  1, light_position1 );
    glUniform4fv( glGetUniformLocation(program, "LightPosition2"),
		  1, light_position2 );

    glUniform1f( glGetUniformLocation(program, "Shininess"),
		 material_shininess );
		 
    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation( program, "ModelView" );
    Projection = glGetUniformLocation( program, "Projection" );
    
    glEnable( GL_DEPTH_TEST );
    
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); /* white background */
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Create new eye vector from sphericaleye vector
	eye.z = sphericaleye.x * cos(sphericaleye.y) * sin(sphericaleye.z);
	eye.x = sphericaleye.x * sin(sphericaleye.y) * sin(sphericaleye.z);
	eye.y = sphericaleye.x * cos(sphericaleye.z);

	std::cout << "Eye: (" << eye.x << "," << eye.y << "," << eye.z << ")  \t";
	std::cout << "Spherical Eye: (" << sphericaleye.x << "," << sphericaleye.y << "," << sphericaleye.z << ")" << std::endl;

    mat4 model_view = LookAt( eye, at, up );
    glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view );

    glDrawElements( GL_TRIANGLE_STRIP,sizeof(elems),GL_UNSIGNED_BYTE,NULL);
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
		case 033: // Escape Key
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
			break;
		case 'w': case 'W':
			sphericaleye.z+=M_PI/64.0;
			break;
		case 'a': case 'A':
			sphericaleye.y-=M_PI/64.0;
			break;
		case 's': case 'S':
			sphericaleye.z-=M_PI/64.0;
			break;
		case 'd': case 'D':
			sphericaleye.y+=M_PI/64.0;
			break;
    }
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape( int width, int height )
{
    glViewport( 0, 0, width, height );

    GLfloat left = -2.0, right = 2.0;
    GLfloat top = 2.0, bottom = -2.0;
    GLfloat zNear = -20.0, zFar = 20.0;

    GLfloat aspect = GLfloat(width)/height;

    if ( aspect > 1.0 ) {
		left *= aspect;
		right *= aspect;
    } else {
		top /= aspect;
		bottom /= aspect;
    }

    mat4 projection = Ortho( left, right, bottom, top, zNear, zFar );
    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 2, 1 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Beamer's Crew - Project 1" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}
