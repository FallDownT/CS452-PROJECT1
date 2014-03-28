run: project1.cpp
	g++ project1.cpp InitShader.cpp -lglut -lGL -lGLU -lGLEW -lm -lSDL2 -g

clean:
	rm -f *.out *~
