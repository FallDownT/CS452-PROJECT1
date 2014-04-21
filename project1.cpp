// fragment shading of sphere model

#include "Angel.h"
#include <iostream>
#include <cstdlib>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include <chrono>
#include <thread>

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;


const int ms_per_frame = 50; //20fps, my vm runs really slow
int SpeedFactor = 1;
int score[2] = {0,0}; //element 0 is player 1's score, element 1 is player 2's score

typedef struct{
	bool isColliding; //true if collision, false if no collision
	int player; //0 if first player, 1 if second player
	int location; //location of the collision on the paddle. determines return path
}collisionInfo;


// Model and view matrices uniform location
GLuint  mMatrix, vMatrix, pMatrix;

// Create camera view variables
point4 at( 0.0, 0.0, 0.0, 1.0 );
point4 eye( 0.0, 0.0, 5.0, 1.0 );
vec4   up( 0.0, 10.0, 0.0, 0.0 );

GLfloat size=1;

GLfloat vertexarray[]={size,size,-size,
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
void init()
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
    mMatrix = glGetUniformLocation( program, "modelMatrix" );
    vMatrix = glGetUniformLocation( program, "viewMatrix" );
    pMatrix = glGetUniformLocation( program, "projectionMatrix" );
    
    glEnable( GL_DEPTH_TEST );
    
    glClearColor( 0.2, 0.6, 0.8, 1.0 ); // sky blue background 
}

//----------------------------------------------------------------------------

void display( SDL_Window* screen ){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Transform model, view and/or projection matrices here
	mat4 model(1.0f);
    glUniformMatrix4fv( mMatrix, 1, GL_TRUE, model );

    mat4 view = LookAt( eye, at, up );
    glUniformMatrix4fv( vMatrix, 1, GL_TRUE, view );

    glDrawElements( GL_TRIANGLE_STRIP,sizeof(elems),GL_UNSIGNED_BYTE,NULL);
    glFlush();
	SDL_GL_SwapWindow(screen);
}

//----------------------------------------------------------------------------

void input(SDL_Window* screen ){

  SDL_Event event;

  while (SDL_PollEvent(&event)){//Handling the keyboard
    switch (event.type){
    case SDL_QUIT:exit(0);break;
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym){
      case SDLK_ESCAPE:exit(0);
      case SDLK_w://paddle 1 up;
		  break;
      case SDLK_s://paddle 1 down;
		  break;
      case SDLK_i://paddle 2 up
		  break;
      case SDLK_k://paddle 2 down
		  break;
	  case SDLK_r://new game
		  score[0]=0;
    	  score[1]=0;
          break;
      }
    }
  }
}

//----------------------------------------------------------------------------
int detectCollision(){

	/*get position of ball:
	vec3 ballPosition[4];
	ballPosition = getBallPosition()*/
	/*
	    1-----0
		|     |
		|     |
		2-----3
	*/

	/*
	  if point 1 equals some point on paddle 1
	  and point 2 equals some point on paddle 1
	  -> collision with paddle 1
	       output location of collision

	  if point 0 = some point on paddle 2
	  and point 3 = some point on paddle 2
	  -> collision with paddle 2
	       output locaiton of collision
	 
	*/	
}

int updateScore(int collision){
	if (collision == 1){
		score[0]++;
		std::cout<<"Player 1 scored!\n\n";
	}
	if (collision == 2){
		score[1]++;
		std::cout<<"Player 2 scored!\n\n";
	}

	std::cout<<"Score is "<<score[0]<<" : "<<score[1]<<'\n';	
}

void updatePositions(struct collision){
	
}

int main( int argc, char **argv )
{
		
	//SDL window and context management
	SDL_Window *window;
	//used in main loop
	int sleepTime = 0;
	int ticks_0, ticks_1;


	
	if(SDL_Init(SDL_INIT_VIDEO)<0){//initilizes the SDL video subsystem
		fprintf(stderr,"Unable to create window: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);//die on error
	}

	//create window
	window = SDL_CreateWindow(
		"Beamer's Crew - Project 1", //Window title
		SDL_WINDOWPOS_UNDEFINED, //initial x position
		SDL_WINDOWPOS_UNDEFINED, //initial y position
		500,							//width, in pixels
		500,							//height, in pixels
		SDL_WINDOW_OPENGL	//flags to be had
		);
	
	//check window creation
	if(window==NULL){
		fprintf(stderr,"Unable to create window: %s\n",SDL_GetError());
	}
	

	//creates opengl context associated with the window
	SDL_GLContext glcontext=SDL_GL_CreateContext(window);
	
	//initializes glew
	glewExperimental=GL_TRUE;
	if(glewInit()){
		fprintf(stderr, "Unable to initalize GLEW");
		exit(EXIT_FAILURE);
	}
  
	init();

	while (true) {
		ticks_0 = SDL_GetTicks();
		collisionInfo collision;

		input(window);
		collision = detectCollision();
		updateScore(collision);
		updatePositions(collision);
		display(window);

		ticks_1 = SDL_GetTicks();
		sleepTime =  ms_per_frame - (ticks_1 - ticks_0);
		std::chrono::milliseconds dura(sleepTime);

		if (sleepTime > 0){
			std::this_thread::sleep_for(dura);
			std::cout<<"slept for "<<sleepTime<<" ms.\n";
		}
		else{
			std::cout<<"frame dropped.\n";
			std::this_thread::sleep_for(dura);
		}
	}

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
    return 0;
}
