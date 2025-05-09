/*
 * flashlight.h
 *
 *  Created on: November 24, 2024
 *      Author: Alli Batell
 */
#ifndef FLASHLIGHT_H_
#define FLASHLIGHT_H_


#include "myLib.h"

extern vec4 flashlight_pos;

void new_direction(int,int);
void initialize_flashlight(vec4);
void update_flashlight();

#endif 