LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

template: template.c initShader.o myLib.o
	gcc -o template template.c initShader.o myLib.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

myLib.o: myLib.h myLib.c
	gcc -c myLib.c