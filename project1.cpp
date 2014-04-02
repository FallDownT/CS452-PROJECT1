// fragment shading of sphere model

#include "Angel.h"
#include <iostream>

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// Model and view matrices uniform location
GLuint mMatrix, vMatrix, pMatrix;

// Define global vaos and ebos
GLuint vao1, vao2, ebo1, ebo2;

// Create camera view variables
point4 at( 0.0, 0.0, 0.0, 1.0 );
point4 eye( 0.0, 0.0, 5.0, 1.0 );
vec4   up( 0.0, 10.0, 0.0, 0.0 );

GLfloat size=1;

GLfloat positionArray[]={
	// Cube
	size,size,-size,
	size,-size,-size,
	-size,-size,-size,
	-size,size,-size,
	size,size,size,
	size,-size,size,
	-size,-size,size,
	-size,size,size,

	// Triangle
	4,-1,0,
	6,-1,0,
	5,1,0
};

GLfloat colorArray[]={
	// Cube
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,

	// Triangle
	1.0f,1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,1.0f,
	0.0f,1.0f,1.0f,1.0f
};
											
GLubyte cubeElemsArray[]={
	7,3,4,0,1,3,2,7,6,4,5,1,6,2,1,
};
											
GLubyte triElemsArray[]={
	0,1,2
};

// Constants
GLuint NumCubeVerticies = 8;

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

	// Define data members
    GLuint vbo;
	size_t posDataOffset, colorDataOffset, cubeElemsSize, triElemsSize;

	// --------------------------------------------------------------
	// -------  V E R T E X    A R R A Y    O B J E C T    1  -------
	// --------------------------------------------------------------
	// Define offsets and sizes
	posDataOffset = 0;
	colorDataOffset = sizeof(positionArray);

    // Generate and bind new vertex array object
    glGenVertexArrays( 1,&vao1 );
    glBindVertexArray( vao1 );

	// Generate and bind new vertex buffer object and populate the buffer
	glGenBuffers( 1,&vbo );
	glBindBuffer( GL_ARRAY_BUFFER,vbo );
	glBufferData( GL_ARRAY_BUFFER,sizeof(positionArray) + sizeof(colorArray),NULL,GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER,posDataOffset,sizeof(positionArray),positionArray );
	glBufferSubData( GL_ARRAY_BUFFER,colorDataOffset,sizeof(colorArray),colorArray );

	// Bind position attribute of vbo
	GLuint in_position = glGetAttribLocation( program, "in_position" );
	glVertexAttribPointer( in_position,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(posDataOffset) );
    glEnableVertexAttribArray( in_position );

	// Bind color attribute of vbo
	GLuint in_color = glGetAttribLocation( program, "in_color" );
	glVertexAttribPointer( in_color,4,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(colorDataOffset) );
    glEnableVertexAttribArray( in_color );

	// Generate and bind element buffer object
	glGenBuffers( 1,&ebo1 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,ebo1 );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,sizeof(cubeElemsArray),cubeElemsArray,GL_STATIC_DRAW );

	// Release bind to vao1
	glBindVertexArray( 0 );
	// --------------------------------------------------------------

	// --------------------------------------------------------------
	// -------  V E R T E X    A R R A Y    O B J E C T    2  -------
	// --------------------------------------------------------------
	// Define new offsets
	posDataOffset += sizeof(GLfloat) * 3 * NumCubeVerticies;
	colorDataOffset += sizeof(GLfloat) * 4 * NumCubeVerticies;

    // Generate new vertex array object
    glGenVertexArrays( 1,&vao2 );
    glBindVertexArray( vao2 );

	// Bind vertex buffer object
	// --Use same vbo as vao1 (no new buffer has been bound)
	
    // Bind attributes to vertex array
	glVertexAttribPointer(in_position,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(posDataOffset));
    glEnableVertexAttribArray( in_position );

	glVertexAttribPointer(in_color,4,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(colorDataOffset));
    glEnableVertexAttribArray( in_color );

	// Generate and bind element buffer object
	glGenBuffers( 1,&ebo2 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,ebo2 );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,sizeof(triElemsArray),triElemsArray,GL_STATIC_DRAW );

	// Release bind to vao2
	glBindVertexArray( 0 );
	// --------------------------------------------------------------

    // Retrieve transformation uniform variable locations
    mMatrix = glGetUniformLocation( program, "modelMatrix" );	//TODO make model matrix not unique!
    vMatrix = glGetUniformLocation( program, "viewMatrix" );
    pMatrix = glGetUniformLocation( program, "projectionMatrix" );
    
    glEnable( GL_DEPTH_TEST );
    
    glClearColor( 0.2, 0.6, 0.8, 1.0 ); // sky blue background 
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Transform model, view, and/or projection matrices here
	mat4 model(1.0f);
    glUniformMatrix4fv( mMatrix, 1, GL_TRUE, model );

    mat4 view = LookAt( eye, at, up );
    glUniformMatrix4fv( vMatrix, 1, GL_TRUE, view );

	// Draw elements of vao1
	glBindVertexArray( vao1 );
    glDrawElements( GL_TRIANGLE_STRIP,sizeof(cubeElemsArray),GL_UNSIGNED_BYTE,0 );

	// Draw elements of vao2
	glBindVertexArray( vao2 );
    glDrawElements( GL_TRIANGLE_STRIP,sizeof(triElemsArray),GL_UNSIGNED_BYTE,0 );

	// Release vao binds and swap buffers
	glBindVertexArray( 0 );
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
    }
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape( int width, int height )
{
    glViewport( 0, 0, width, height );

    GLfloat left = -5.0, right = 15.0;
    GLfloat top = 15.0, bottom = -5.0;
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
    glUniformMatrix4fv( pMatrix, 1, GL_TRUE, projection );
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize( 512, 384 );
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
