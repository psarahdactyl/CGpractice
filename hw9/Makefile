GCC_OPTIONS=-Wall -pedantic -I include
GL_OPTIONS=-lGLEW -lGL -lglut
OPTIONS=$(GCC_OPTIONS) $(GL_OPTIONS)


.cpp: 
	g++ $@.cpp Common/InitShader.o $(OPTIONS) -o $@  

run1: hw9-1 
	./hw9-1 bound-bunny_5k.smf

run2: hw9-2
	./hw9-2 patchPoints.txt
