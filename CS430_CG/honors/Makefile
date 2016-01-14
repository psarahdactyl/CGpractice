GCC_OPTIONS=-Wall -pedantic -I include
GL_OPTIONS=-lGLEW -lGL -lGLU -lglut
OPTIONS=$(GCC_OPTIONS) $(GL_OPTIONS)


.cpp: 
	g++ $@.cpp $(OPTIONS) -o $@  

run: honors
	./honors

