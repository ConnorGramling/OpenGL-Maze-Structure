LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

<<<<<<< HEAD
main: main.c initShader.o lib.o
	gcc -o main main.c initShader.o lib.o $(LIBS)
=======
template: template.c initShader.o myLib.o
	gcc -o template template.c initShader.o myLib.o $(LIBS)
>>>>>>> e1b8cb101ce7bf47ccf739ad71c456243286a9d4

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

<<<<<<< HEAD
lib.o: lib.h lib.c
	gcc -c lib.c

=======
myLib.o: myLib.h myLib.c
	gcc -c myLib.c
>>>>>>> e1b8cb101ce7bf47ccf739ad71c456243286a9d4