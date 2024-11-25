LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

main: main.c initShader.o myLib.o initMaze.o sun.o perspFunc.o flashlight.o lighting.o
	gcc -o main main.c initShader.o myLib.o initMaze.o sun.o perspFunc.o flashlight.o lighting.o $(LIBS)

test: test.c initShader.o myLib.o  perspFunc.o
	gcc -o test test.c initShader.o myLib.o  perspFunc.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

myLib.o: myLib.h myLib.c
	gcc -c myLib.c

initMaze.o: initMaze.c initMaze.h
	gcc -c initMaze.c

sun.o: sun.c sun.h
	gcc -c sun.c
	
presp_func.o: perspFunc.c perspFunc.h
	gcc -c perspFunc.c

flashlight.o: flashlight.c flashlight.h
	gcc -c flashlight.c

lighting.o: lighting.c lighting.h
	gcc -c lighting.c
