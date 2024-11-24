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

#define PLATFORM_SIZE 32

#define MAZE_SIZE 4
#define NUM_MAZE_POLES (MAZE_SIZE + 1) * (MAZE_SIZE + 1)
#define WALL_LENGTH 3
#define MAZE_WALL_HEIGHT 4
#define FLOOR_SIZE (WALL_LENGTH * MAZE_SIZE) + MAZE_SIZE + 1

void initMaze();

#endif /* INITMAZE_H_ */