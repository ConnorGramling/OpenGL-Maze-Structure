#include <stdlib.h>
#include <time.h>

/*
 * initMaze.c
 *
 *  Created on: November 18, 2024
 *      Author: Connor Gramling
 */

#include "initMaze.h"
#include "sun.h"
#include "myLib.h"

int num_vertices;
vec4 *positions;
vec2 *tex_coords;

float sun_x = 0.0f;
float sun_y = 20.0f; // Positioned 20 units above the center of the island
float sun_z = 0.0f;
float rotation_angle = 0.0f; // Current rotation angle in degrees

void initMaze()
{
  srand(time(NULL)); // Seed for randomness

  // Postion for block shape
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

  // Textures
  vec2 grass_top_tex_coords[6] = {
      {0.5, 1.0},
      {0.75, 0.75},
      {0.5, 0.75},
      {0.5, 1.0},
      {0.75, 1.0},
      {0.75, 0.75}};

  vec2 dirt_tex_coords[6] = {
      {0.75, 1.0},
      {1.0, 0.75},
      {0.75, 0.75},
      {0.75, 1.0},
      {1.0, 1.0},
      {1.0, 0.75}};

  vec2 grass_tex_coords[6] = {
      {0.0, 0.25},
      {0.25, 0.0},
      {0.0, 0.0},
      {0.0, 0.25},
      {0.25, 0.25},
      {0.25, 0.0}};

  vec2 sun_tex_coords[6] = {
      {0.0, 0.75},
      {0.25, 0.5},
      {0.0, 0.5},
      {0.0, 0.75},
      {0.25, 0.75},
      {0.25, 0.5}};

  int pyramid_base = PLATFORM_SIZE;
  int pyramid_height = PLATFORM_SIZE / 2;

  int pyramid_vertices = 0;
  for (int i = 0; i < pyramid_height; ++i)
  {
    pyramid_vertices += 36 * (pyramid_base - 2 * i) * (pyramid_base - 2 * i);
  }
  num_vertices = pyramid_vertices;

  positions = (vec4 *)malloc(num_vertices * sizeof(vec4));
  tex_coords = (vec2 *)malloc(num_vertices * sizeof(vec2));

  if (positions == NULL || tex_coords == NULL)
  {
    fprintf(stderr, "Memory allocation failed!\n");
    exit(1);
  }

  int index = 0;

  for (int i = 0; i < pyramid_height; ++i)
  {
    int layer_size = pyramid_base - 2 * i;

    for (int j = 0; j < layer_size; ++j)
    {
      for (int k = 0; k < layer_size; ++k)
      {
        // Check if this is an exterior block
        if (j == 0 || j == layer_size - 1 || k == 0 || k == layer_size - 1)
        {
          // Randomly decide whether to skip this block
          if (rand() % 2 == 0)
          {
            continue; // Skip this block
          }
        }

        float x_offset = j - (layer_size - 1) / 2.0f;
        float z_offset = -(k - (layer_size - 1) / 2.0f);
        float y_offset = -i - 0.5;

        for (int v = 0; v < 36; v++)
        {
          positions[index] = (vec4){
              base_positions[v].x + x_offset,
              base_positions[v].y + y_offset,
              base_positions[v].z + z_offset,
              base_positions[v].w};

          if (i == 0)
          {
            if (v >= 30) // Bottom face
              tex_coords[index] = dirt_tex_coords[v % 6];
            else if (v >= 24) // Top face
              tex_coords[index] = grass_tex_coords[v % 6];
            else // Front, Back, Left, Right faces
              tex_coords[index] = grass_top_tex_coords[v % 6];
          }
          else
          {
            tex_coords[index] = dirt_tex_coords[v % 6];
          }
          index++;
        }
      }
    }
  }

  // Adding the sun block
  for (int v = 0; v < 36; v++)
  {
    positions[index] = (vec4){
        base_positions[v].x + sun_x,
        base_positions[v].y + sun_y,
        base_positions[v].z + sun_z,
        base_positions[v].w};

    tex_coords[index] = sun_tex_coords[v % 6];
    index++;
  }

  num_vertices = index;
}
