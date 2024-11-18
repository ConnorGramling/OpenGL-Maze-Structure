LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

main: main.c initShader.o myLib.o
	gcc -o main main.c initShader.o myLib.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

myLib.o: myLib.h myLib.c
	gcc -c myLib.c