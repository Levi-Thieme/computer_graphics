#pragma once
#include "Angel.h"
#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H
#endif

void flip_y(vec3 *vertices, int v_count);

void flip_x(vec3 *vertices, int v_count);

void translate_vec3(vec3 *vec, const GLfloat dx, const GLfloat dy, const GLfloat dz);

void translate_image(vec3 *vecs, int count, const GLfloat dx, const GLfloat dy, const GLfloat dz);

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