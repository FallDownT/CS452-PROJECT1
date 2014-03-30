// fragment shading of sphere model

#include "Angel.h"
#include <iostream>

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// Model and view matrices uniform location
GLuint  mvMatrix, pMatrix;

// Create camera view variables
point4 at( 0.0, 0.0, 0.0, 1.0 );
point4 eye( 1.0, 1.0, 1.0, 1.0 );
vec4   up( 0.0, 10.0, 0.0, 0.0 );

GLfloat size=1;

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

GLfloat colorarray[]={
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f
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
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexarray) + sizeof(colorarray),NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertexarray),vertexarray);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(vertexarray),sizeof(colorarray),colorarray);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );
	
    // Set up vertex arrays
	GLuint in_position = glGetAttribLocation(program, "in_position" );
	glVertexAttribPointer(in_position,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
    glEnableVertexAttribArray( in_position );

	GLuint in_color = glGetAttribLocation(program, "in_color" );
	glVertexAttribPointer(in_color,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(sizeof(vertexarray)));
    glEnableVertexAttribArray( in_color );

	// Create Element Array Buffer
	glGenBuffers(1,&ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elems),elems,GL_STATIC_DRAW);
		 
    // Retrieve transformation uniform variable locations
    mvMatrix = glGetUniformLocation( program, "modelViewMatrix" );
    pMatrix = glGetUniformLocation( program, "projectionMatrix" );
    
    glEnable( GL_DEPTH_TEST );
    
    glClearColor( 0.2, 0.6, 0.8, 1.0 ); /* sky blue background */
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Create new eye vector from sphericaleye vector

    mat4 modelView = LookAt( eye, at, up );
    glUniformMatrix4fv( mvMatrix, 1, GL_TRUE, modelView );

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
    glUniformMatrix4fv( pMatrix, 1, GL_TRUE, projection );
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
