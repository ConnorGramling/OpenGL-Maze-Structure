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

#define MAZE_SIZE 8 //the length/width of the maze in "tiles"
#define WALL_LENGTH 3 //how many blocks make up the walls
#define NUM_MAZE_POLES (MAZE_SIZE + 1) * (MAZE_SIZE + 1)
#define WALL_HEIGHT 4 //how tall are the walls
#define FLOOR_SIZE (WALL_LENGTH * MAZE_SIZE) + MAZE_SIZE + 1
#define PLATFORM_SIZE (FLOOR_SIZE + 8)

void initMaze();

#endif /* INITMAZE_H_ */