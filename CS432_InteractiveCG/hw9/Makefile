GCC_OPTIONS=-Wall -pedantic -I include
GL_OPTIONS=-lGLEW -lGL -lglut
OPTIONS=$(GCC_OPTIONS) $(GL_OPTIONS)


.cpp: 
	g++ $@.cpp Common/InitShader.cpp $(OPTIONS) -o $@  

run1: hw9-1
	./hw9-1 patchPoints.txt

run2: hw9-2 
	./hw9-2 bound-bunny_5k.smf

