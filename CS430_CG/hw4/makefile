build:
	g++ -std=c++11 Shape.cpp Line.cpp Polygon.cpp Vector3D.cpp Matrix3D.cpp Vector4D.cpp Matrix4D.cpp Transformation.cpp CG_hw4.cpp -o CG_hw4


run: build 
	./CG_hw4 -f bound-lo-sphere.smf -j 0 -k 0 -o 500 -p 500 -x 0.0 -y 0.0 -z 1.0 -X 0.0 -Y 0.0 -Z 0.0 -q 0.0 -r 0.0 -w -1.0 -Q 0.0 -R 1.0 -W 0.0 -u -0.7 -U 0.7 -V 0.7 >test1.xpm
	./CG_hw4 -f bound-lo-sphere.smf -x -1.0 -z 0.5 -q 1.0 -w -0.5 >test2.xpm
	./CG_hw4 -f bound-lo-sphere.smf -q 1.0 -w -1.0 >test3.xpm
	./CG_hw4 -f bound-lo-sphere.smf -x -4.0 -z 5.0 -q 1.0 -w -0.5 >test4.xpm
	./CG_hw4 -f bound-lo-sphere.smf -k 125 -p 375 -q 1.0 -u -1.4 -U 1.4 >test5.xpm
	./CG_hw4 -f bound-bunny_200.smf -j 100 -k 50 -o 400 -p 450 -x 0.5 -y 0.2 -z 1.0 -X 0.2 -Y -0.2 -Z 0.3 -q -3.0 -r -2.0 -w 1.0 -Q 3.0 -R -2.0 -W -4.0 -u -0.5 -U 1.2 -V 0.8 -P >test6.xpm
	./CG_hw4 -f bound-cow.smf >test7.xpm
	./CG_hw4 -f bound-cow.smf -P >test8.xpm
	./CG_hw4 -f bound-cow.smf -j 0 -k 30 -o 275 -p 305 -P >test9.xpm
	./CG_hw4 -f bound-cow.smf -x 1.5 >test10.xpm
	./CG_hw4 -f bound-cow.smf -x 4.75 -y -3.25 -z 3.3 -P >test11.xpm
	./CG_hw4 -f bound-cow.smf -X 0.25 -Y -0.15 -Z 0.3 >test12.xpm
	./CG_hw4 -f bound-cow.smf -X 0.35 -Y -0.3 -Z 0.3 -u -0.35 -v -0.35 -P >test13.xpm
	./CG_hw4 -f bound-cow.smf -X 0.25 -Y -0.15 -Z 0.3 -j 103 -k 143 -o 421 -p 379 >test14.xpm
	./CG_hw4 -f bound-cow.smf -X 0.35 -Y -0.3 -Z 0.3 -u -0.35 -v -0.35 -j 43 -k 71 -o 201 -p 402 -P >test15.xpm
	./CG_hw4 -f bound-cow.smf -q -1 -r 1.5 -w -2.0 >test16.xpm
	./CG_hw4 -f bound-cow.smf -Q 1.5 -R 1 -W 0.4 >test17.xpm
	./CG_hw4 -f bound-cow.smf -u -1.5 -v -0.9 -U 1.2 -V 0.7 >test18.xpm

clean:
	rm -f *xpm
	rm -f CG_hw4
