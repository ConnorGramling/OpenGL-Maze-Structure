/*
 * template.c
 *
 *  Created on: September 10, 2024
 *      Author: Thumrongsak Kosiyatrakul
 */


#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "initShader.h"

#include "tempLib.h" // replace this with your library

int num_vertices = 6;

void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    vec4 *positions = (vec4 *) malloc(sizeof(vec4) * 6);

    positions[0] = (vec4) { 0.0,  0.5,  0.0, 1.0};	// top
    positions[1] = (vec4) {-0.5, -0.5,  0.0, 1.0};	// bottom left
    positions[2] = (vec4) { 0.5, -0.5,  0.0, 1.0};	// bottom right
    positions[3] = (vec4) { 0.5,  0.8, -0.5, 1.0};	// top
    positions[4] = (vec4) { 0.9,  0.0, -0.5, 1.0};	// bottom right
    positions[5] = (vec4) { 0.1,  0.0, -0.5, 1.0};	// bottom left

    vec4 *colors = (vec4 *) malloc(sizeof(vec4) * 6);

    colors[0] = (vec4) {1.0, 0.0, 0.0, 1.0};	// red   (for top)
    colors[1] = (vec4) {0.0, 1.0, 0.0, 1.0};	// green (for bottom left)
    colors[2] = (vec4) {0.0, 0.0, 1.0, 1.0};	// blue  (for bottom right)
    colors[3] = (vec4) {0.0, 1.0, 0.0, 1.0};	// green (for bottom right)
    colors[4] = (vec4) {0.0, 1.0, 0.0, 1.0};	// green (for bottom right)
    colors[5] = (vec4) {0.0, 1.0, 0.0, 1.0};	// green (for bottom right)

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * 6 + sizeof(vec4) * 6, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * 6, positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * 6, sizeof(vec4) * 6, colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * 6));

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	glutLeaveMainLoop();

    //glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Triangle");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
