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

#include "myLib.h" // replace this with your library

#include <math.h>
#include <stdbool.h>

mat4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint ctm_location;
mat4 previous_ctm= {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

float previous_x;
float previous_y;
bool left_press = true;

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

    ctm_location = glGetUniformLocation(program, "ctm");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    
    glUniformMatrix4fv(ctm_location, 1,GL_FALSE, (GLfloat *) &ctm);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	glutLeaveMainLoop();

    //glutPostRedisplay();
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
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}
