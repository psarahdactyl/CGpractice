build:
	g++ -std=c++11 Shape.cpp Line.cpp Polygon.cpp Vector3D.cpp Matrix3D.cpp Vector4D.cpp Matrix4D.cpp Transformation.cpp CG_hw5.cpp -o CG_hw5


run: build 
	./CG_hw5 -f bound-sprellpsd.smf -j 0 -k 0 -o 500 -p 500 -x 0.0 -y 0.0 -z 1.0 -X 0.0 -Y 0.0 -Z 0.0 -q 0.0 -r 0.0 -w -1.0 -Q 0.0 -R 1.0 -W 0.0 -u -0.7 -U 0.7 -V 0.7 -F 0.6 -B -0.6 >test1.xpm
	./CG_hw5 -f bound-cow.smf -F 0.1 >test2.xpm
	./CG_hw5 -f bound-cow.smf -F 0.1 -B -0.18 >test3.xpm
	./CG_hw5 -f bound-bunny_1k.smf -g bound-cow.smf -i bound-sprtrd.smf -u -0.8 -U 0.8 -v -0.8 -V 0.8 >test4.xpm
	./CG_hw5 -f bound-bunny_1k.smf -g bound-cow.smf -i bound-sprtrd.smf -q 0.4 -r 0.2 -w 1 -u -0.6 -v -0.6 -U 0.6 -V 0.6 -P >test5.xpm
	./CG_hw5 -f bound-sprellpsd.smf -j 334 -k 24 -o 449 -p 402 >test6.xpm
	./CG_hw5 -u -1.0 -U 0.8 -v -0.9 -V 1.0 -g bound-bunny_1k.smf -f bound-sprtrd.smf -j 34 -k 104 -0 149 -p 472 >test7.xpm
	./CG_hw5 -u -1.0 -U 0.8 -v -0.9 -V 1.0 -g bound-bunny_1k.smf -f bound-sprtrd.smf -j 34 -k 104 -o 149 -p 472 -Q -1.1 -R 0.9 -W 0.6 >test8.xpm

clean:
	rm -f *xpm
	rm -f CG_hw5
