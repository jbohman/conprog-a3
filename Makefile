options = -g -O2 -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodeminimodel -lgecodegist -lgecodedriver -lQtCore -lQtGui  -I/usr/include/qt4 

squarepacking: squarepacking.cpp
	g++ $(options) squarepacking.cpp -o squarepacking

squarepacking_custom: squarepacking_custom.cpp no-overlap.cpp
	g++ $(options) squarepacking_custom.cpp -o squarepacking_custom
