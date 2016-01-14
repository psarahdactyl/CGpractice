build:
	g++ -std=c++11 Shape.cpp Line.cpp Polygon.cpp Vector.cpp Matrix.cpp Transformation.cpp CG_hw3.cpp -o CG_hw3

run: build
	./CG_hw3 -f hw3_split.ps -a 0 -b 0 -c 500 -d 500 -j 0 -k 0 -o 500 -p 500 -s 1.0 -m 0 -n 0 -r 0 > out.xpm 
#./CG_hw3 -f hw2_a.ps -s 1.5 -o 499 -p 499 > out.xpm 

clean:
	rm -f out*
	rm -f CG_hw3
