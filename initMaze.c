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
#include "main.h"

int num_vertices;
vec4 *positions;
vec2 *tex_coords;
vec4 *normals;
vec4 *colors;

float sun_x = 0.0f;
float sun_y = 20.0f; // Positioned 20 units above the center of the island
float sun_z = 0.0f;
float rotation_angle = 0.0f; // Current rotation angle in degrees

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

vec2 wood_tex_coords[6] = {
    {0.75, 0.75},
    {1.0, 0.5},
    {0.75, 0.5},
    {0.75, 0.75},
    {1.0, 0.75},
    {1.0, 0.5}};

vec2 stone_tex_coords[6] = {
    {0.25, 0.5},
    {0.5, 0.25},
    {0.25, 0.25},
    {0.25, 0.5},
    {0.5, 0.5},
    {0.5, 0.25}};

vec2 brick_tex_coords[6] = {
    {0.75, 0.5},
    {1.0, 0.25},
    {0.75, 0.25},
    {0.75, 0.5},
    {1.0, 0.5},
    {1.0, 0.25}};

void initMaze()
{
  srand(time(NULL)); // Seed for randomness
  int tempNum = MAZE_SIZE * MAZE_SIZE * 3; //# of walls
  int maze_vertices = 36 * ((FLOOR_SIZE * FLOOR_SIZE) + (WALL_HEIGHT * (NUM_MAZE_POLES + (WALL_LENGTH * tempNum))));

  //printf("\nplatform size : %d\n", PLATFORM_SIZE);

  int pyramid_base = PLATFORM_SIZE;
  int pyramid_height = PLATFORM_SIZE / 2;

  int pyramid_vertices = 0;
  for (int i = 0; i < pyramid_height; ++i)
  {
    pyramid_vertices += 36 * (pyramid_base - 2 * i) * (pyramid_base - 2 * i);
  }
  num_vertices = pyramid_vertices + maze_vertices;

  positions = (vec4 *)malloc(num_vertices * sizeof(vec4));
  tex_coords = (vec2 *)malloc(num_vertices * sizeof(vec2));
  normals = (vec4 *)malloc(num_vertices * sizeof(vec4));
  colors = (vec4 *)malloc(num_vertices * sizeof(vec4));

  if (positions == NULL || tex_coords == NULL || normals == NULL || colors == NULL)
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
          // Set positions
          positions[index] = (vec4){
              base_positions[v].x + x_offset,
              base_positions[v].y + y_offset,
              base_positions[v].z + z_offset,
              base_positions[v].w};

          // Set texture coordinates
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

          // Set normals (based on face direction)
          vec4 normal = {0.0, 0.0, 0.0, 0.0};
          if (v < 6)      // Front face
            normal = (vec4){0.0, 0.0, 1.0, 0.0};
          else if (v < 12) // Back face
            normal = (vec4){0.0, 0.0, -1.0, 0.0};
          else if (v < 18) // Left face
            normal = (vec4){-1.0, 0.0, 0.0, 0.0};
          else if (v < 24) // Right face
            normal = (vec4){1.0, 0.0, 0.0, 0.0};
          else if (v < 30) // Top face
            normal = (vec4){0.0, 1.0, 0.0, 0.0};
          else             // Bottom face
            normal = (vec4){0.0, -1.0, 0.0, 0.0};
          normals[index] = normal;

          // Set colors (example gradient based on height)
          colors[index] = (vec4){
              (float)i / pyramid_height, // Red increases with height
              1.0f - (float)i / pyramid_height, // Green decreases with height
              (float)(rand() % 256) / 255.0f, // Random blue
              1.0f};                          // Alpha is fully opaque

          index++;
        }
      }
    }
  }

  ////////////////////////////////
  // START MAZE

  float yfloor = 0.5f; //floor height
  float yStart = 1.5f; //walls and pole height
  float xStart = -(((WALL_LENGTH + 1) * MAZE_SIZE)/2);
  float zStart = -(((WALL_LENGTH + 1) * MAZE_SIZE)/2);

  float yTemp = yStart;
  float xTemp = xStart;
  float zTemp = zStart;

  // make floor
for (int i = 0; i < FLOOR_SIZE; i++) {
    for (int j = 0; j < FLOOR_SIZE; j++) {
        for (int v = 0; v < 36; v++) {
            positions[index] = (vec4){
                base_positions[v].x + xTemp,
                base_positions[v].y + yfloor,
                base_positions[v].z + zTemp,
                base_positions[v].w};
            tex_coords[index] = wood_tex_coords[v % 6];

            // Set normals (assuming floor is flat, normal pointing up)
            normals[index] = (vec4){0.0, 1.0, 0.0, 0.0};

            // Set colors (alternate color pattern for the floor)
            colors[index] = (vec4){
                (float)i / FLOOR_SIZE,           // Red based on row index
                (float)j / FLOOR_SIZE,           // Green based on column index
                1.0f,                            // Blue is constant for the floor
                1.0f};                           // Alpha is fully opaque

            index++;
        }
        xTemp += 1.0f;
    }
    xTemp = xStart;
    zTemp += 1.0f;
}

// make poles
xTemp = xStart;
zTemp = zStart;
for (int i = 0; i < MAZE_SIZE + 1; i++) {
    for (int j = 0; j < MAZE_SIZE + 1; j++) {
        for (int k = 0; k < WALL_HEIGHT; k++) {
            for (int v = 0; v < 36; v++) {
                positions[index] = (vec4){
                    base_positions[v].x + xTemp,
                    base_positions[v].y + yTemp,
                    base_positions[v].z + zTemp,
                    base_positions[v].w};
                tex_coords[index] = stone_tex_coords[v % 6];

                // Set normals (poles are vertical, normal pointing up or down)
                normals[index] = (vec4){0.0, 1.0, 0.0, 0.0}; // Normal pointing upwards

                // Set colors (changing color pattern for each pole)
                colors[index] = (vec4){
                    (float)i / MAZE_SIZE,            // Red based on row index
                    (float)j / MAZE_SIZE,            // Green based on column index
                    (float)k / WALL_HEIGHT,          // Blue based on height of the pole
                    1.0f};                           // Alpha is fully opaque

                index++;
            }
            yTemp += 1.0f;
        }
        yTemp = yStart;
        xTemp += (WALL_LENGTH + 1.0f);
    }
    xTemp = xStart;
    zTemp += (WALL_LENGTH + 1.0f);
}

  // make xwalls
  yTemp = yStart;
  xTemp = xStart + 1.0f;
  zTemp = zStart;
  for(int i = 0; i < MAZE_SIZE + 1; i++) {
    for(int j = 0; j < MAZE_SIZE; j++) {
      for(int n = 0; n < WALL_LENGTH; n++) {
        for(int k = 0; k < WALL_HEIGHT; k++) {
          for (int v = 0; v < 36; v++){
            positions[index] = (vec4){
              base_positions[v].x + xTemp,
              base_positions[v].y + yTemp,
              base_positions[v].z + zTemp,
              base_positions[v].w};
            tex_coords[index] = brick_tex_coords[v % 6];
            index++;
          }
          yTemp += 1.0f;
        }
        yTemp = yStart;
        xTemp += 1.0f;
      }
      xTemp += 1.0f;
    }
    xTemp = xStart + 1.0f;
    zTemp += (WALL_LENGTH + 1.0f);
  }

  // make zwalls
  yTemp = yStart;
  xTemp = xStart;
  zTemp = zStart + 1.0f;
  for(int i = 0; i < MAZE_SIZE + 1; i++) {
    for(int j = 0; j < MAZE_SIZE; j++) {
      for(int n = 0; n < WALL_LENGTH; n++) {
        for(int k = 0; k < WALL_HEIGHT; k++) {
          for (int v = 0; v < 36; v++){
            positions[index] = (vec4){
              base_positions[v].x + xTemp,
              base_positions[v].y + yTemp,
              base_positions[v].z + zTemp,
              base_positions[v].w};
            tex_coords[index] = brick_tex_coords[v % 6];
            index++;
          }
          yTemp += 1.0f;
        }
        yTemp = yStart;
        zTemp += 1.0f;
      }
      zTemp += 1.0f;
    }
    zTemp = zStart + 1.0f;
    xTemp += (WALL_LENGTH + 1.0f);
  }

  ////////////////////////////////

  // Adding the sun block
  for (int v = 0; v < 36; v++)
  {
    positions[index] = (vec4){
        base_positions[v].x + sun_x,
        base_positions[v].y + sun_y,
        base_positions[v].z + sun_z,
        base_positions[v].w};

    tex_coords[index] = sun_tex_coords[v % 6];
    normals[index] = (vec4){0.0, 1.0, 0.0, 0.0}; // Normal pointing up for the sun
    colors[index] = (vec4){1.0, 1.0, 0.0, 1.0}; // Yellow sun
    index++;
  }

  num_vertices = index;
}