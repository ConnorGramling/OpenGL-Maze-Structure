/*
 * initMaze.c
 *
 *  Created on: November 18, 2024
 *      Author: Connor Gramling
 */

#include "initMaze.h"
#include "myLib.h"

int num_vertices = 36 * PLATFORM_SIZE * PLATFORM_SIZE;
vec4 positions[36 * PLATFORM_SIZE * PLATFORM_SIZE];
vec2 tex_coords[36 * PLATFORM_SIZE * PLATFORM_SIZE];

void initMaze()
{
  // Base cube positions and texture coordinates (unchanged from before)
  vec4 base_positions[36] = {
      // Front face
      {-0.5, -0.5, 0.5, 1.0},
      {0.5, 0.5, 0.5, 1.0},
      {-0.5, 0.5, 0.5, 1.0},
      {-0.5, -0.5, 0.5, 1.0},
      {0.5, -0.5, 0.5, 1.0},
      {0.5, 0.5, 0.5, 1.0},

      // Back face
      {0.5, -0.5, -0.5, 1.0},
      {-0.5, 0.5, -0.5, 1.0},
      {0.5, 0.5, -0.5, 1.0},
      {0.5, -0.5, -0.5, 1.0},
      {-0.5, -0.5, -0.5, 1.0},
      {-0.5, 0.5, -0.5, 1.0},

      // Left face
      {-0.5, -0.5, -0.5, 1.0},
      {-0.5, 0.5, 0.5, 1.0},
      {-0.5, 0.5, -0.5, 1.0},
      {-0.5, -0.5, -0.5, 1.0},
      {-0.5, -0.5, 0.5, 1.0},
      {-0.5, 0.5, 0.5, 1.0},

      // Right face
      {0.5, -0.5, 0.5, 1.0},
      {0.5, 0.5, -0.5, 1.0},
      {0.5, 0.5, 0.5, 1.0},
      {0.5, -0.5, 0.5, 1.0},
      {0.5, -0.5, -0.5, 1.0},
      {0.5, 0.5, -0.5, 1.0},

      // Top face
      {-0.5, 0.5, 0.5, 1.0},
      {0.5, 0.5, -0.5, 1.0},
      {-0.5, 0.5, -0.5, 1.0},
      {-0.5, 0.5, 0.5, 1.0},
      {0.5, 0.5, 0.5, 1.0},
      {0.5, 0.5, -0.5, 1.0},

      // Bottom face
      {-0.5, -0.5, -0.5, 1.0},
      {0.5, -0.5, 0.5, 1.0},
      {-0.5, -0.5, 0.5, 1.0},
      {-0.5, -0.5, -0.5, 1.0},
      {0.5, -0.5, -0.5, 1.0},
      {0.5, -0.5, 0.5, 1.0}};

  vec2 base_tex_coords[36] = {
      // Front face
      {0.5, 1.0},   // Bottom-left
      {0.75, 0.75}, // Top-right
      {0.5, 0.75},  // Top-left
      {0.5, 1.0},   // Bottom-left
      {0.75, 1.0},  // Bottom-right
      {0.75, 0.75}, // Top-right

      // Back face
      {0.5, 1.0},
      {0.75, 0.75},
      {0.5, 0.75},
      {0.5, 1.0},
      {0.75, 1.0},
      {0.75, 0.75},

      // Left face - Grass Top
      {0.5, 1.0},
      {0.75, 0.75},
      {0.5, 0.75},
      {0.5, 1.0},
      {0.75, 1.0},
      {0.75, 0.75},

      // Right face - Grass Top
      {0.5, 1.0},
      {0.75, 0.75},
      {0.5, 0.75},
      {0.5, 1.0},
      {0.75, 1.0},
      {0.75, 0.75},

      // Top face - Grass
      {0.0, 0.25},
      {0.25, 0.0},
      {0.0, 0.0},
      {0.0, 0.25},
      {0.25, 0.25},
      {0.25, 0.0},

      // Bottom face - Dirt
      {0.75, 1.0},
      {1.0, 0.75},
      {0.75, 0.75},
      {0.75, 1.0},
      {1.0, 1.0},
      {1.0, 0.75}};

  int index = 0;
  float half_size = PLATFORM_SIZE / 2.0f; // Centering adjustment

  // Generate 14x14 platform starting from the opposite side
  for (int row = 0; row < PLATFORM_SIZE; row++)
  {
    for (int col = 0; col < PLATFORM_SIZE; col++)
    {
      float x_offset = col - half_size;
      float z_offset = -(row - half_size); // Adjust to shift starting perspective

      // Copy the base cube positions and adjust for current row/col
      for (int i = 0; i < 36; i++)
      {
        positions[index] = (vec4){
            base_positions[i].x + x_offset,
            base_positions[i].y,
            base_positions[i].z + z_offset,
            base_positions[i].w};
        tex_coords[index] = base_tex_coords[i];
        index++;
      }
    }
  }
}
