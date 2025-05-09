/*
 * flashlight.c
 *
 *  Created on: November 24, 2024
 *      Author: Alli Batell
 */

#include "myLib.h"
#include "main.h"
#include <math.h>


vec4 light_direction;


float angle = 30;
float rad_angle;
float e =5;

float intensity;



vec4 light_i_ambient= (vec4){0,0,0,0};
vec4 light_i_diffuse=(vec4){1,1,1,0};
vec4 light_i_specular= (vec4){0,0,0,0};

vec4 reflect_i_ambient= (vec4){0,0,0,0}, reflect_i_diffuse = (vec4){1,1,1,1}, reflect_i_specular= (vec4){0,0,0,0};

void new_direction(int x,int y){
    light_direction.x = x;
    light_direction.y =y;
    //light_direction.z = eye.z;

}

void initialize_flashlight(vec4 direction){
    light_direction = direction;
    light_direction.w = 0;
    float rad_angle = deg_to_rad(angle);
    float intensity =pow(cos(rad_angle),e);
}
void update_flashlight(){
    glUniform4fv(flashlight_position_location, 1, (GLvoid *)&flashlight_position_location);  
}

