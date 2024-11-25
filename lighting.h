/*
 * lighting.h
 *
 *  Created on: November 24, 2024
 *      Author: Connor Gramling
 */

#ifndef LIGHTING_H_
#define LIGHTING_H_

#include "main.h"
#include "myLib.h"
#include <stdio.h>
#include <stdlib.h>

void updateLight(); 

extern vec4 *ambient_colors;
extern vec4 *diffuse_colors;
extern vec4 *specular_colors;

#endif /* LIGHTING_H_ */
