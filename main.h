/*
 * main.h
 *
 *  Created on: November 22, 2024
 *      Author: Connor Gramlinnng
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "myLib.h"
#include <stdio.h>
#include <stdlib.h>

extern int num_vertices;
extern vec4 *positions;
extern vec2 *tex_coords;

extern float sun_x;
extern float sun_y; // Positioned 20 units above the center of the island
extern float sun_z;
extern float rotation_angle; // Current rotation angle in degrees

#endif /* MAIN_H_ */