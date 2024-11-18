LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

template: template.c initShader.o tempLib.o
	gcc -o template template.c initShader.o tempLib.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

tempLib.o: tempLib.h tempLib.c
	gcc -c tempLib.c
