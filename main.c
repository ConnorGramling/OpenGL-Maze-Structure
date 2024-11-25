/*
 * main.c
 *
 *  Created on: November 11, 2024
 *      Author: Connor Gramling
        Author: Alli Batell
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
#include "initMaze.h"
#include "perspFunc.h"
#include "sun.h"
#include "flashlight.h"
#include "lighting.h"

#include "main.h"
#include "myLib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

GLuint buffer;
GLuint light_position_location;
GLuint use_texture_location;
int use_texture = 0;

mat4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint ctm_location;
mat4 previous_ctm= {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

float previous_x;
float previous_y;
bool left_press = true;

vec4 light_position = {0, 20, 0, 1};

int quadrant= 1;

bool in_maze= false;

typedef enum{
    NONE = 0,
    WALK_FORWARDS,
    WALK_BACKWARDS,
    WALK_LEFT,
    WALK_RIGHT,
    TURN_LEFT,
    TURN_RIGHT,
    RESET
 } state;

state currentState = NONE;

int current_step = 0;
int max_steps;
int isAnimating = -1;

mat4 base_projection;

vec4 eye, at, up;
float zoom_left = -1, zoom_right = 1, zoom_top =1, zoom_bottom = -1, zoom_near = 1, zoom_far = -1;

GLuint model_view_location;
mat4 model_view = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLuint projection_location;
mat4 projection = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

vec4 *ambient_colors;
vec4 *diffuse_colors;
vec4 *specular_colors;

void init(void) {
    
    initMaze();

    ambient_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    diffuse_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    specular_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);

    updateLight();

    eye = (vec4) {0,0, PLATFORM_SIZE/2,1}, at = (vec4) {0,0,PLATFORM_SIZE/2-1,1}, up =(vec4) {0,1,0,0};
    model_view = lookAt(eye, at, up);

    zoom_left = -PLATFORM_SIZE/2, zoom_right = PLATFORM_SIZE/2, zoom_top =PLATFORM_SIZE/2, zoom_bottom =-PLATFORM_SIZE/2, zoom_near = PLATFORM_SIZE, zoom_far = -PLATFORM_SIZE;
    projection = ortho(zoom_left, zoom_right, zoom_bottom, zoom_top, zoom_near, zoom_far);
    base_projection= ortho( -PLATFORM_SIZE/2, PLATFORM_SIZE/2, -PLATFORM_SIZE/2, PLATFORM_SIZE/2, PLATFORM_SIZE, -PLATFORM_SIZE);

    // model_view = lookAt((vec4){0,0, PLATFORM_SIZE/2,1}, (vec4){0,0,PLATFORM_SIZE/2 -1,1},(vec4) {0,1,0,0});
    // zoom_left = -PLATFORM_SIZE/2, zoom_right = PLATFORM_SIZE/2, zoom_top =PLATFORM_SIZE/2, zoom_bottom =-PLATFORM_SIZE/2, zoom_near = -PLATFORM_SIZE/4, zoom_far = -PLATFORM_SIZE *1.5;
    // projection = frustrum(zoom_left, zoom_right, zoom_bottom, zoom_top, zoom_near, zoom_far);

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

    //GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * 3 * num_vertices + sizeof(vec2) * num_vertices, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices, positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices, colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * 2 * num_vertices, sizeof(vec4) * num_vertices, normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * 3 * num_vertices, sizeof(vec2) * num_vertices, tex_coords);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * num_vertices));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * 2 * num_vertices));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * 3 * num_vertices));

    ctm_location = glGetUniformLocation(program, "ctm");
    model_view_location = glGetUniformLocation(program, "model_view");
    projection_location = glGetUniformLocation(program, "projection");
    
    GLuint texture_location = glGetUniformLocation(program, "texture");
    glUniform1i(texture_location, 0);

    use_texture_location = glGetUniformLocation(program, "use_texture");
    glUniform1i(use_texture_location, use_texture);

    light_position_location = glGetUniformLocation(program, "light_position");
    glUniform4fv(light_position_location, 1, (GLvoid *) &light_position);

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
    if (key == 'l') {
        glutLeaveMainLoop();
    } else if (key == ',') { // Rotate sun counterclockwise around X-axis
        rotation_angle -= 45.0f;
        rotateSunX(-45.0f);
    } else if (key == '.') { // Rotate sun clockwise around X-axis
        rotation_angle += 45.0f;
        rotateSunX(45.0f);
    } else if (key == '\'') { // Rotate sun counterclockwise around Y-axis
        rotateSunY(-45.0f);
    } else if (key == ';') { // Rotate sun clockwise around Y-axis
        rotateSunY(45.0f);
    }
    if (key == 'w'){
        if (in_maze){
            eye.z = eye.z +.25;
            at.z = at.z +.25;
            model_view = lookAt(eye, at, up);
        }
    }
    if (key == 'a'){
        if (in_maze){
            eye.x = eye.x +.25;
            at.x = at.x +.25;
            model_view = lookAt(eye, at, up);
        }
    }
    if (key == 's'){
        if (in_maze){
            eye.z = eye.z -.25;
            at.z = at.z -.25;
            model_view = lookAt(eye, at, up);
        }
    }
    if (key == 'd'){
        if (in_maze){
            eye.x = eye.x -.25;
            at.x = at.x -.25;
            model_view = lookAt(eye, at, up);
        }
    }
    if (key == 'q'){
        if (in_maze){
            mat4 rotate_mat = rotate_y(-1);
            model_view = mat_mult(rotate_mat,model_view);
        }
    }
    if (key == 'e'){
       if (in_maze){
            mat4 rotate_mat = rotate_y(1);
            model_view = mat_mult(rotate_mat, model_view);
       }
    }
    if (key == 'r'){
        isAnimating =1;
        currentState = RESET;
        current_step =0;
        max_steps = 300;
    } 
    if(key == 't') {
	use_texture ^= 0x1;
	glUniform1i(use_texture_location, use_texture);
	glutPostRedisplay();
    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        left_press = true;
        previous_x=  (x * 2.0 / 511.0) - 1;;
        previous_y= -((y * 2.0 / 511.0) -1);
        if (!in_maze)
            previous_ctm= ctm;
        if (in_maze)
            new_direction(previous_x, previous_y);
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        left_press = false;
        previous_ctm= ctm;
    }

    // proper zoom function, not yet working 

    if (button == 3){
        zoom_left = 1.02 *zoom_left;
        zoom_right = 1.02 *zoom_right;
        zoom_top = 1.02 *zoom_top;
        zoom_bottom = 1.02 *zoom_bottom;
        zoom_near = 1.02 *zoom_near;
        zoom_far = 1.02 *zoom_far;
        projection = ortho(zoom_left,zoom_right, zoom_bottom, zoom_top, zoom_near, zoom_far);
    }
    if (button == 4){
        zoom_left = 0.98 *zoom_left;
        zoom_right = 0.98 *zoom_right;
        zoom_top = 0.98 *zoom_top;
        zoom_bottom = 0.98 *zoom_bottom;
        zoom_near = 0.98 *zoom_near;
        zoom_far = 0.98 *zoom_far;
        projection = ortho(zoom_left,zoom_right, zoom_bottom, zoom_top, zoom_near, zoom_far);
    } 
    
    glutPostRedisplay();
    
}
void motion(int x, int y) {
    if (left_press == true){
        float x_coord = (x * 2.0 / 511.0) - 1;
        float y_coord = -((y * 2.0 / 511.0) -1);
        if (!in_maze)
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
        if (in_maze){
            new_direction(x_coord, y_coord);
        }
   }
    glutPostRedisplay();
}

void idle(void){
    if(isAnimating){
        if (currentState== NONE);
            
        else if(currentState == WALK_FORWARDS) {
            if(current_step == max_steps){
                
            }
            else{
            
            }
        }
        else if(currentState == WALK_BACKWARDS) {
            if(current_step == max_steps){
                
            }
            else{
            
            }
        }
        else if(currentState == TURN_LEFT) {
            if(current_step == max_steps){
                
            }
            else{
            
            }
        }
        else if(currentState == TURN_RIGHT) {
            if(current_step == max_steps){
                
            }
            else{
            
            }
        }
        else if(currentState == WALK_LEFT) {
            if(current_step == max_steps){
                
            }
            else{
            
            }
        }
        else if(currentState == WALK_RIGHT) {
            if(current_step == max_steps){
                
            }
            else{
            
            }
        }
        else if(currentState == RESET) {
            float alpha;
            if(current_step == max_steps){
                isAnimating = -1;
                alpha = 1.0;
                ctm = (mat4){{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
                previous_ctm=(mat4) {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
                projection = base_projection;
                currentState= NONE;
            }
            else{
                alpha = (float) current_step/ max_steps;
                current_step++;
                mat4 newctm;
                vec4 movex, movey, movez, movew;


                //animate projection

                mat4 new_model;
                movex = sub_vv (base_projection.x ,projection.x);
                movex =scal_v_mult(alpha,movex);
                new_model.x = add_vv(projection.x, movex);
                movey = sub_vv (base_projection.y,projection.y);
                movey =scal_v_mult(alpha,movey);
                new_model.y = add_vv(projection.y, movey);
                movez = sub_vv (base_projection.z,projection.z);
                movez =scal_v_mult(alpha,movez);
                new_model.z = add_vv(projection.z, movez);
                movew = sub_vv (base_projection.w,projection.w);
                movew =scal_v_mult(alpha,movew);
                new_model.w = add_vv(projection.w, movew);
                projection=new_model;
                
                

                //animate ctm
                movex = sub_vv ((vec4){1,0,0,0},ctm.x);
                movex =scal_v_mult(alpha,movex);
                newctm.x = add_vv(ctm.x, movex);
                movey = sub_vv ((vec4){0,1,0,0},ctm.y);
                movey =scal_v_mult(alpha,movey);
                newctm.y = add_vv(ctm.y, movey);
                movez = sub_vv ((vec4){0,0,1,0},ctm.z);
                movez =scal_v_mult(alpha,movez);
                newctm.z = add_vv(ctm.z, movez);
                movew = sub_vv ((vec4){0,0,0,1},ctm.w);
                movew =scal_v_mult(alpha,movew);
                newctm.w = add_vv(ctm.w, movew);
                ctm=newctm;



            }
        }
    glutPostRedisplay();
    }
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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
    glutIdleFunc(idle);
    glutMainLoop();

    free(positions);
    free(tex_coords);

    return 0;
}
