GCC_OPTIONS=-Wall -pedantic -I include
GL_OPTIONS=-lGLEW -lGL -lglut
OPTIONS=$(GCC_OPTIONS) $(GL_OPTIONS)


.cpp: 
	g++ $@.cpp Common/InitShader.o $(OPTIONS) -o $@  

run: hw6
	./hw6 bound-bunny_5k.smf
