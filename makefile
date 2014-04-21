run: project1.cpp
	g++ project1.cpp InitShader.cpp -std=c++11 -lGL -lGLU -lGLEW -lm -lSDL2 -g

clean:
	rm -f *.out *~
