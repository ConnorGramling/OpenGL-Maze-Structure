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
#include "main.h"
#include <math.h>

void rotateSun(float angle) {
    // Convert angle to radians for trigonometric functions
    float radians = angle * M_PI / 180.0f;

    // Calculate new sun position using a rotation matrix around the X-axis
    float new_y = sun_y * cos(radians) - sun_z * sin(radians);
    float new_z = sun_y * sin(radians) + sun_z * cos(radians);

    // Update the sun's position
    sun_y = new_y;
    sun_z = new_z;

    printf
}