options = -g -O2 -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodeminimodel -lgecodegist -lgecodedriver -lQtCore -lQtGui  -I/usr/include/qt4 

sudoku: squarepacking.cpp
	g++ $(options) squarepacking.cpp -o squarepacking
