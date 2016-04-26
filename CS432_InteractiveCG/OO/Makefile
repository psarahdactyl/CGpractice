GCC_OPTIONS=-Wall -pedantic -I include
GL_OPTIONS=-lGLEW -lGL -lglut
OPTIONS=$(GCC_OPTIONS) $(GL_OPTIONS)


.cpp: 
	g++ Circle.cpp Shape.cpp $@.cpp Common/InitShader.cpp $(OPTIONS) -o $@  

