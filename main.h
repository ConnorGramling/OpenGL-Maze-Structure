/*
 * main.h
 *
 *  Created on: November 22, 2024
 *      Author: Connor Gramlinnng
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include "initShader.h"
#include "initMaze.h"
#include "perspFunc.h"
#include "sun.h"
#include "flashlight.h"
#include "lighting.h"
#include "myLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

extern GLuint buffer;
extern GLuint light_position_location;

extern int num_vertices;
extern vec4 *positions;
extern vec2 *tex_coords;
extern vec4 *normals;
extern vec4 *colors;

extern vec4 light_position;

extern vec4 base_positions[36];
extern vec2 grass_top_tex_coords[6];
extern vec2 dirt_tex_coords[6];
extern vec2 grass_tex_coords[6];
extern vec2 sun_tex_coords[6];

extern float sun_x;
extern float sun_y;
extern float sun_z;
extern float rotation_angle; // Current rotation angle in degrees

extern vec4 eye, at, up;

#endif /* MAIN_H_ */