/*
 * main.c
 *
 *  Created on: November 11, 2024
 *      Author: Connor Gramling
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
//#include "initMaze.h"
#include "perspFunc.h"

#include "myLib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define GRID_SIZE 1
int num_vertices = 36;

mat4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint ctm_location;
mat4 previous_ctm= {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

float previous_x;
float previous_y;
bool left_press = true;

GLuint model_view_location;
mat4 model_view = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLuint projection_location;
mat4 projection = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

void init(void) {

    
    vec4 positions[36] = {
        // Front face
        {-0.5, -0.5,  0.5, 1.0}, { 0.5,  0.5,  0.5, 1.0}, {-0.5,  0.5,  0.5, 1.0},
        {-0.5, -0.5,  0.5, 1.0}, { 0.5, -0.5,  0.5, 1.0}, { 0.5,  0.5,  0.5, 1.0},
        
        // Back face
        { 0.5, -0.5, -0.5, 1.0}, {-0.5,  0.5, -0.5, 1.0}, { 0.5,  0.5, -0.5, 1.0},
        { 0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {-0.5,  0.5, -0.5, 1.0},
        
        // Left face
        {-0.5, -0.5, -0.5, 1.0}, {-0.5,  0.5,  0.5, 1.0}, {-0.5,  0.5, -0.5, 1.0},
        {-0.5, -0.5, -0.5, 1.0}, {-0.5, -0.5,  0.5, 1.0}, {-0.5,  0.5,  0.5, 1.0},
        
        // Right face
        { 0.5, -0.5,  0.5, 1.0}, { 0.5,  0.5, -0.5, 1.0}, { 0.5,  0.5,  0.5, 1.0},
        { 0.5, -0.5,  0.5, 1.0}, { 0.5, -0.5, -0.5, 1.0}, { 0.5,  0.5, -0.5, 1.0},
        
        // Top face
        {-0.5,  0.5,  0.5, 1.0}, { 0.5,  0.5, -0.5, 1.0}, {-0.5,  0.5, -0.5, 1.0},
        {-0.5,  0.5,  0.5, 1.0}, { 0.5,  0.5,  0.5, 1.0}, { 0.5,  0.5, -0.5, 1.0},
        
        // Bottom face
        {-0.5, -0.5, -0.5, 1.0}, { 0.5, -0.5,  0.5, 1.0}, {-0.5, -0.5,  0.5, 1.0},
        {-0.5, -0.5, -0.5, 1.0}, { 0.5, -0.5, -0.5, 1.0}, { 0.5, -0.5,  0.5, 1.0}
    };

    vec2 tex_coords[36] = {
        // Front face
        {0.0, 0.5}, {0.25, 0.25}, {0.0, 0.25},
        {0.0, 0.5}, {0.25, 0.5}, {0.25, 0.25},
        
        // Back face
        {0.0, 0.5}, {0.25, 0.25}, {0.0, 0.25},
        {0.0, 0.5}, {0.25, 0.5}, {0.25, 0.25},
        
        // Left face
        {0.0, 0.5}, {0.25, 0.25}, {0.0, 0.25},
        {0.0, 0.5}, {0.25, 0.5}, {0.25, 0.25},
        
        // Right face
        {0.0, 0.5}, {0.25, 0.25}, {0.0, 0.25},
        {0.0, 0.5}, {0.25, 0.5}, {0.25, 0.25},
        
        // Top face
        {0.0, 0.5}, {0.25, 0.25}, {0.0, 0.25},
        {0.0, 0.5}, {0.25, 0.5}, {0.25, 0.25},
        
        // Bottom face
        {0.0, 0.5}, {0.25, 0.25}, {0.0, 0.25},
        {0.0, 0.5}, {0.25, 0.5}, {0.25, 0.25}
    };
    model_view = lookAt((vec4){0,0,0, 1},(vec4){0,0,-1,1}, (vec4){0,1,0,0});
    projection = frustrum(-1, 1, -1, 1, 1, -1);

    int tex_width = 64;
    int tex_height = 64;
    GLubyte my_texels[tex_width][tex_height][3];

    FILE *fp = fopen("textures02.raw", "r");
    if (fp != NULL)
        printf("[textureTemplate] Successfully open a texture file.\n");
    fread(my_texels, tex_width * tex_height * 3, 1, fp);
    fclose(fp);

    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    GLuint vao;
#ifdef __APPLE__
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);
#else
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
#endif

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(tex_coords), tex_coords);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(positions)));

    ctm_location = glGetUniformLocation(program, "ctm");
    model_view_location = glGetUniformLocation(program, "model_view");
    projection_location = glGetUniformLocation(program, "projection");

    GLuint texture_location = glGetUniformLocation(program, "texture");
    glUniform1i(texture_location, 0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1, 0);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm);
    glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *)&model_view);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)&projection);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey) {
    if (key == 'q') {
        glutLeaveMainLoop();
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        left_press = true;
        previous_x=  (x * 2.0 / 511.0) - 1;;
        previous_y= -((y * 2.0 / 511.0) -1);
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        left_press = false;
        previous_ctm= ctm;
    }
    if (button == 3){
        ctm = mat_mult( scale(1.02, 1.02, 1.02), previous_ctm);
        previous_ctm= ctm;
    }
    if (button == 4){
        ctm = mat_mult(scale(.98, .98, .98), previous_ctm);
        previous_ctm= ctm;
    }
    
    glutPostRedisplay();
    
}
void motion(int x, int y) {
    if (left_press == true){
        float x_coord = (x * 2.0 / 511.0) - 1;
        float y_coord = -((y * 2.0 / 511.0) -1);
        if(sqrt((x_coord*x_coord)+ (y_coord*y_coord))<= 1 && sqrt((previous_x*previous_x)+ (previous_y*previous_y))<= 1){
            float z_coord = (float) (1-(x_coord*x_coord)-(y_coord*y_coord));
            vec4 axis2 = ((vec4){x_coord, y_coord,z_coord,0});
            vec4 axis1 = ((vec4){previous_x, previous_y,(1-(previous_x*previous_x)-(previous_y*previous_y)),0});
            vec4 axis = norm_v(cross_prod(axis1, axis2));
            float d = (float) sqrt((axis.y*axis.y)+(axis.z*axis.z));
            mat4 x_rotate_matrix = {{1, 0,0,0},{0,(axis.z/d),(axis.y/d),0},{0,(-axis.y/d), (axis.z/d),0},{0, 0, 0, 1}};
            mat4 neg_y_rotate_matrix = {{d, 0,axis.x,0},{0,1,0,0},{-axis.x,0,d,0},{0, 0, 0, 1}};
            float degree = (rad_to_deg(acos(dot_prod(axis1, axis2)/(mag_v(axis1) * mag_v(axis2)))));
            
            mat4 ctm2 =mat_mult(trans_mat(x_rotate_matrix), mat_mult(trans_mat(neg_y_rotate_matrix),mat_mult(rotate_z(degree), mat_mult(neg_y_rotate_matrix, x_rotate_matrix))));
            ctm= mat_mult( ctm2, previous_ctm);
            
        }
   }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Maze");
#ifndef __APPLE__
    glewInit();
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}