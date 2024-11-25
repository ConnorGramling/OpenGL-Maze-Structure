/*
 * lighting.c
 *
 *  Created on: November 24, 2024
 *      Author: Connor Gramling
 */

#include "main.h"
#include "myLib.h"
#include "lighting.h"

// START SHADING

void updateLight()
{

  // Ambient
  for (int i = 0; i < num_vertices; i++)
  {
    ambient_colors[i] = scal_v_mult(0.3, colors[i]);
  }

  // Diffuse
  for (int i = 0; i < num_vertices; i++)
  {
    vec4 l = norm_v(sub_vv(light_position, positions[i]));
    vec4 n = normals[i];
    float l_dot_n = dot_prod(l, n);
    if (l_dot_n < 0)
      l_dot_n = 0;
    diffuse_colors[i] = scal_v_mult(l_dot_n, colors[i]);
  }

  // Specular
  for (int i = 0; i < num_vertices; i++)
  {
    vec4 l = norm_v(sub_vv(light_position, positions[i]));
    vec4 v = norm_v(sub_vv(eye, positions[i]));
    vec4 h = norm_v(add_vv(l, v));
    vec4 n = normals[i];

    float h_dot_n = dot_prod(h, n);
    if (h_dot_n < 0)
      h_dot_n = 0;

    specular_colors[i] = scal_v_mult(pow(h_dot_n, 50), (vec4){1, 1, 1, 1});
  }

  for (int i = 0; i < num_vertices; i++)
  {
    colors[i] = add_vv(specular_colors[i], add_vv(diffuse_colors[i], ambient_colors[i]));
  }

  print_v4(specular_colors[1]);
  print_v4(diffuse_colors[1]);
  print_v4(ambient_colors[1]);
}
// END SHADING