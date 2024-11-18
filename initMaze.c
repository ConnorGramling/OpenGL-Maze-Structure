/*
 * initMaze.c
 *
 *  Created on: November 18, 2024
 *      Author: Connor Gramlinnng
 */

#include "initMaze.h"

void initMaze(int GRID_SIZE)
{
  // Create a 12x12 grid of squares, each square being 1x1 unit
  int index = 0;
  for (int row = 0; row < GRID_SIZE; ++row)
  {
    for (int col = 0; col < GRID_SIZE; ++col)
    {
      float x = (float)col;
      float y = (float)row;

      // Define positions for two triangles that form the square
      positions[index] = (vec4){x, y, 0.0, 1.0};
      tex_coords[index++] = (vec2){0.0, 0.0};

      positions[index] = (vec4){x + 1.0, y + 1.0, 0.0, 1.0};
      tex_coords[index++] = (vec2){1.0, 1.0};

      positions[index] = (vec4){x, y + 1.0, 0.0, 1.0};
      tex_coords[index++] = (vec2){0.0, 1.0};

      positions[index] = (vec4){x, y, 0.0, 1.0};
      tex_coords[index++] = (vec2){0.0, 0.0};

      positions[index] = (vec4){x + 1.0, y, 0.0, 1.0};
      tex_coords[index++] = (vec2){1.0, 0.0};

      positions[index] = (vec4){x + 1.0, y + 1.0, 0.0, 1.0};
      tex_coords[index++] = (vec2){1.0, 1.0};
    }
  }
}