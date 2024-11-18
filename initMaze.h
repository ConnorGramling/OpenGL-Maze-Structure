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

extern vec4 positions[24];
extern vec2 tex_coords[24];

void initMaze(int GRID_SIZE);

#endif /* INITMAZE_H_ */