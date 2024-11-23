LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

main: main.c initShader.o myLib.o initMaze.o perspFunc.o
	gcc -o main main.c initShader.o myLib.o initMaze.o perspFunc.o $(LIBS)

test: test.c initShader.o myLib.o  perspFunc.o
	gcc -o test test.c initShader.o myLib.o  perspFunc.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

myLib.o: myLib.h myLib.c
	gcc -c myLib.c

initMaze.o: initMaze.c initMaze.h
	gcc -c initMaze.c
	
presp_func.o: perspFunc.c perspFunc.h
	gcc -c perspFunc.c