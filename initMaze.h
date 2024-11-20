/*
 * initMaze.h
 *
 *  Created on: November 18, 2024
 *      Author: Connor Gramlinnng
 */

#ifndef INITMAZE_H_
#define INITMAZE_H_

#include "myLib.h"
#include <stdio.h>
#include <stdlib.h>

#define PLATFORM_SIZE 14
extern int num_vertices;
extern vec4 *positions;
extern vec2 *tex_coords;

void initMaze();

#endif /* INITMAZE_H_ */