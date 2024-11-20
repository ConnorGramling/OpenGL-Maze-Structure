/*
 * initMaze.c
 *
 *  Created on: November 18, 2024
 *      Author: Connor Gramling
 */

#include "initMaze.h"
#include "myLib.h"

int num_vertices;
vec4 *positions;
vec2 *tex_coords;

void initMaze()
{
  // Top platform positions and texture coordinates
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
      // Front face - Grass Top
      {0.5, 1.0},   // Bottom-left
      {0.75, 0.75}, // Top-right
      {0.5, 0.75},  // Top-left
      {0.5, 1.0},   // Bottom-left
      {0.75, 1.0},  // Bottom-right
      {0.75, 0.75}, // Top-right

      // Back face - Grass Top
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

  // Calculate the total number of vertices needed for both platform and pyramid
  int platform_vertices = 36 * PLATFORM_SIZE * PLATFORM_SIZE;
  int pyramid_base = PLATFORM_SIZE;
  int pyramid_height = PLATFORM_SIZE / 2;
  int pyramid_vertices = 0;
  for (int i = 0; i < pyramid_height; ++i)
  {
    pyramid_vertices += 36 * (pyramid_base - 2 * i) * (pyramid_base - 2 * i);
  }
  num_vertices = platform_vertices + pyramid_vertices;

  // Dynamically allocate memory for positions and tex_coords
  positions = (vec4 *)malloc(num_vertices * sizeof(vec4));
  tex_coords = (vec2 *)malloc(num_vertices * sizeof(vec2));

  if (positions == NULL || tex_coords == NULL)
  {
    // Handle memory allocation error (e.g., print an error message and exit)
    fprintf(stderr, "Memory allocation failed!\n");
    exit(1);
  }
  else
  {
    printf("Memory allocated /n");
  }

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
  // Pyramid Structure

  int pyramid_index = index; // Start index for pyramid vertices

  for (int i = 0; i < pyramid_height; ++i)
  {
    int layer_size = pyramid_base - 2 * i;
    for (int j = 0; j < layer_size; ++j)
    {
      for (int k = 0; k < layer_size; ++k)
      {
        float x_offset = j - (layer_size - 1) / 2.0f;    // Centering on x-axis
        float z_offset = -(k - (layer_size - 1) / 2.0f); // Centering on z-axis
        float y_offset = -i - 0.5;                       // Adjust y-offset to stack the blocks

        // Copy the base cube positions and adjust for current row/col
        for (int v = 0; v < 36; v++)
        {
          positions[pyramid_index] = (vec4){
              base_positions[v].x + x_offset,
              base_positions[v].y + y_offset,
              base_positions[v].z + z_offset,
              base_positions[v].w};

          // All sides of the pyramid should have dirt texture,  use bottom face textures
          tex_coords[pyramid_index] = base_tex_coords[v + 30]; // Index 30 starts the bottom face textures

          pyramid_index++;
        }
      }
    }
  }
  num_vertices = pyramid_index; // Update vertex count
}
