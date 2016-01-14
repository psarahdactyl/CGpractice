build:
	g++ -std=c++11 Shape.cpp Line.cpp Polygon.cpp Vector.cpp Matrix.cpp Transformation.cpp CG_hw3.cpp -o CG_hw3

run: build
	./CG_hw3 -f hw3_split.ps -a 0 -b 0 -c 500 -d 500 -j 0 -k 0 -o 500 -p 500 -s 1.0 -m 0 -n 0 -r 0 > out1.xpm
	./CG_hw3 -f hw3_split.ps -a 50 -b 0 -c 325 -d 500 -j 0 -k 110 -o 480 -p 410 -s 1 -m 0 -n 0 -r 0 > out2.xpm
	./CG_hw3 -f hw3_split.ps -a 10 -b 10 -c 550 -d 400 -j 10 -k 10 -o 500 -p 400 -s 1.2 -m 6 -n 25 -r 8 > out3.xpm
	./CG_hw3 -f hw3_split.ps -b 62 -c 500 -d 479 -r 75 -j 139 -o 404 -p 461 -s .85 -m 300 >out4.xpm
	./CG_hw3 -f hw3_split.ps -a 275 -b 81 -c 550 -d 502 -r -37 -j 123 -k 217 -o 373 -p 467 > out5.xpm

clean:
	rm -f out*
	rm -f CG_hw3
