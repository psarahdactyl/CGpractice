GCC_OPTIONS=-Wall -pedantic -I include
GL_OPTIONS=-lGLEW -lGL -lglut
OPTIONS=$(GCC_OPTIONS) $(GL_OPTIONS)


.cpp: 
	g++ $@.cpp Common/InitShader.cpp $(OPTIONS) -o $@  

run: hw7
	./hw7 patchPoints.txt
