#pragma once
#include "Angel.h"
#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H
#endif

void flip_y(vec3 *vertices, int v_count);

void flip_x(vec3 *vertices, int v_count);

void translate_vec3(vec3 *vec, const GLfloat dx, const GLfloat dy, const GLfloat dz);

void translate_vec4(vec4 *vec, const GLfloat dx, const GLfloat dy, const GLfloat dz);

void translate_image3D(vec3 *vecs, int count, const GLfloat dx, const GLfloat dy, const GLfloat dz);

void translate_image4D(vec4 *vecs, int count, const GLfloat dx, const GLfloat dy, const GLfloat dz);

void scale_vec3(vec3 *vec, const GLfloat dx, const GLfloat dy, const GLfloat dz);

void scale_image(vec3 *vecs, int count, const GLfloat dx, const GLfloat dy, const GLfloat dz);

/*
Calculates the difference between src and dest and stores in diff.

src - the source vertex
dest - the destination vertex
diff - vector containing difference
*/
void abs_difference(vec3 *src, vec3 *dest, vec3 *diff);

/*
Calculates amount / slices and stores in delta.

delta - a vector containing deltas for x, y, and z
slices - the number of slices
delta - the vec3 to store the delta slice for each delta.
*/
void slice_delta(vec3 *v, int slices, vec3 *delta);

/*
Translates v towards dest by delta amount.

v - the vector to translate.
dest - the destination to translate towards.
delta - the amount to translate by.
*/
void translate_to(vec3 *v, const vec3 *dest, const vec3 *delta);

/*
Fills the deltas array with deltas based on src, dest, and slices.

deltas - Array to store delta for src and dest vertices
src - Array storing source vertices.
dest - Array storing destination vertices.
v_count - the number of vertices in deltas, src, and dest.
slices - the number of slices applied to the difference of src and dest to calculate delta.
*/
void calculate_deltas(vec3 *deltas, vec3 *src, vec3 *dest, const GLint v_count, const GLint slices);

/*
Sets the values for a translation matrix.
dx - delta x
dy - delta y
dz - delta z
translation_mat - the matrix to hold the translation values
*/
void translate_m(GLfloat dx, GLfloat dy, GLfloat dz, mat4 *translation_mat);

/*
Applies scaling factors to scale_m's x and y coordinates

s_vec - vector containing scaling factors
scale_m - scaling matrix
*/
void scale_2d(vec3 *s_vec, mat4 *scale_m);

/*
Applies scaling to scale_m

s_vec - vector containing scaling factors
scale_m - scaling matrix
*/
void scale_3d(vec3 *s_vec, mat4 *scale_m);

/*
Rotates with respect to y-axis
theta - the amount of rotation(degrees)
*/
void rotate_y(GLfloat theta, mat4 *rotation_m);

/*
Rotates with respect to x-axis
theta - the amount of rotation(degrees)
*/
void rotate_x(GLfloat theta, mat4 *rotation_m);

/*
Rotates with respect to z-axis
theta - the amount of rotation(degrees)
*/
void rotate_z(GLfloat theta, mat4 *rotation_m);