#include "myLib.h"
/*
 * presp_func.c
 *
 *  Created on: November 18, 2024
 *      Author: Alli Batell
 */

mat4 lookAt(vec4 eye, vec4 at, vec4 up){
    vec4 vpn = sub_vv(eye, at);
    vec4 n = norm_v(vpn);
    vec4 u = cross_prod(up,n);
    u = norm_v(u);
    vec4 v= cross_prod(n, u);
    v = norm_v(v);
    mat4 R = (mat4){u, v, n, (vec4){0,0,0,1}};
    R = trans_mat(R);
    mat4 T = translate(eye.x, eye.y, eye.z);
    T= mat_mult(R,T);
    print_mat4(T);
    return T;
}

mat4 ortho(float left, float right, float bottom, float top, float near, float far){
    mat4 T = translate(-((right + left)/2), -((top + bottom)/2), -((near+far)/2));
    mat4 S = scale(2/(right-left), 2/(top-bottom), 2/(near-far));
    return mat_mult(S,T);
}

mat4 frustrum(float left, float right, float bottom, float top, float near, float far){
    mat4 H = (mat4){{1,0,0,0},{0,1,0,0}, {((left+right)/(-2*near)), ((bottom + top)/ (-2*near)), 1,0},{0,0,0,1}};
    mat4 S = (mat4) {{((-2*near)/(right-left)), 0,0,0}, {0, ((-2* near)/ (top - bottom)),0,0}, {0,0,1,0},{0,0,0,1}};
    mat4 N = (mat4) {{1,0,0,0},{0,1,0,0},{0,0,((near +far)/(far-near)),-1},{0,0,(-(2*near*far)/(far-near)),0}};
    mat4 SH = mat_mult(S,H);
    //return SH;
    return mat_mult(N, SH);
}