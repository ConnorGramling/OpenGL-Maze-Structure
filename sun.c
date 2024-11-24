// no light (standard), viewer light, and sun

/*
 * sun.c
 *
 *  Created on: November 22, 2024
 *      Author: Connor Gramling
 */

#include "sun.h"
#include "myLib.h"
#include "main.h"
#include <math.h>

void rotateSunX(float angle)
{
    // Convert angle to radians for trigonometric functions
    float radians = angle * M_PI / 180.0f;

    // Calculate new sun position using a rotation matrix around the X-axis
    float new_y = sun_y * cos(radians) - sun_z * sin(radians);
    float new_z = sun_y * sin(radians) + sun_z * cos(radians);

    // Update the sun's position
    sun_y = new_y;
    sun_z = new_z;

    // Update vertex positions for the sun
    int sun_start_index = num_vertices - 36; // Assuming the last 36 vertices are for the sun
    for (int v = 0; v < 36; v++)
    {
        positions[sun_start_index + v] = (vec4){
            base_positions[v].x + sun_x,
            base_positions[v].y + sun_y,
            base_positions[v].z + sun_z,
            base_positions[v].w};
    }

    // Re-upload the updated vertex positions to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertices * sizeof(vec4), positions);

    printf("Sun position updated: (%.2f, %.2f, %.2f)\n", sun_x, sun_y, sun_z);
}

void rotateSunY(float angle)
{
    // Convert angle to radians for trigonometric functions
    float radians = angle * M_PI / 180.0f;

    // Calculate new sun position using a rotation matrix around the Y-axis
    float new_x = sun_x * cos(radians) + sun_z * sin(radians);
    float new_z = -sun_x * sin(radians) + sun_z * cos(radians);

    // Update the sun's position
    sun_x = new_x;
    sun_z = new_z;

    // Update vertex positions for the sun
    int sun_start_index = num_vertices - 36; // Assuming the last 36 vertices are for the sun
    for (int v = 0; v < 36; v++)
    {
        positions[sun_start_index + v] = (vec4){
            base_positions[v].x + sun_x,
            base_positions[v].y + sun_y,
            base_positions[v].z + sun_z,
            base_positions[v].w};
    }

    // Re-upload the updated vertex positions to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertices * sizeof(vec4), positions);

    printf("Sun position updated: (%.2f, %.2f, %.2f)\n", sun_x, sun_y, sun_z);
}