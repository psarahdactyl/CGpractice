build:
	g++ -std=c++11 Shape.cpp Line.cpp Polygon.cpp Vector.cpp Matrix.cpp Transformation.cpp CG_hw2.cpp -o CG_hw2

run: build
	./CG_hw2 -f hw1.ps -a 0 -b 0 -c 499 -s 1.0 -m 0 -n 0 -r 0 > out.xpm 
	./CG_hw2 -f hw1.ps -a 0 -b 0 -c 499 -d 499 -s 0.8 -m 85 -n 25 -r 10 > out2.xpm 
	./CG_hw2 -f hw1.ps -s 0.5 > out3.xpm 
	./CG_hw2 -f hw1.ps -r -30 > out4.xpm 
	./CG_hw2 -f hw1.ps -m 100 -n 100 > out5.xpm 
	./CG_hw2 -f hw1.ps -a 25 -b 50 -c 399 -d 399 > out6.xpm 
	./CG_hw2 -f hw1.ps -a 25 -b 50 -c 399 -d 399 -r 30 -m 100 -n 100 -s 0.5 > out7.xpm 

clean:
	rm -f out*
	rm -f CG_hw2
