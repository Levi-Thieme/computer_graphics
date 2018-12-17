#ifndef VERTICES_H
#define VERTICES_H
#endif

#include "Angel.h"
#include <string>
#include <algorithm>

/*
Finds the absolute largest x,y, and z values.
vecs - a vector to search.
max - vector to store maximum points.
*/
void find_abs_largest_x_y_z(vec3 *vecs, int count, vec3 *max);

/*
Finds the absolute largest x,y, and z values.
vecs - a vector to search.
max - vector to store maximum points.
*/
void find_abs_largest_x_y_z(vec4 *vecs, int count, mat4 modelView, vec3 *max);

/*
Finds the largest x,y, and z values.
vecs - a vector to search.
max - vector to store maximum points.
*/
void find_largest_x_y_z(vec4 *vecs, int count, mat4 modelView, vec3 *max);

/*
Finds the minimum x,y, and z values.
polys - a vector of polys to search.
min - vector to store minimum points.
*/
void find_minimum_x_y_z(vec3 *vec, int count, vec3 *min);

/*
Finds the minimum x,y, and z values.
polys - a vector of polys to search.
min - vector to store minimum points.
*/
void find_minimum_x_y_z(vec4 *vec, int count, mat4 modelView, vec3 *min);

/*
Normalizes coordinates so that values are within a 0.0 - 1.0 range.
vec - the vector to normalize
max - vector containing maximum values
min - vector containing minimum values
*/
void normalize_vec3(vec3 *vec, const vec3 *max);

/*
Normalizes coordinates so that values are within a 0.0 - 1.0 range.
vec - the vector to normalize
max - vector containing maximum values
min - vector containing minimum values
*/
void normalize_vec4(vec4 *vec, const vec3 *max);

/*
Normalizes all vec3's in vecs.
count - the number of vec3's contained in vecs.
max - vector containing maximum values
min - vector containing minimum values
*/
void normalize_vecs3(vec3 *vecs, const int count, const vec3 *max);

/*
Normalizes all vec4's in vecs.
count - the number of vec4's contained in vecs.
max - vector containing maximum values
min - vector containing minimum values
*/
void normalize_vecs4(vec4 *vecs, const int count, const vec3 *max);

/*
Changes 0's to 1's for normalization
*/
void clean_normalization_vector(vec3 *v);

void generate_random_normalized_vertices(vec3 *vecs, int count);

void normalize_random(GLfloat *p, const GLfloat max);

/*
Calculates the largest difference in x-coordinates for points.
points - An array of vectors
point_count - The number of vectors contained in points array
*/
void calculate_width_height(vec3 *min, vec3 *max, vec3 *differences);

/*
Calculates the center based on min and max vertices

min - vector containing the minimum values for x,y,z
max - vector containing the maximum values for x,y,z
center - vector to store the center vertex in
*/
void calculate_center(vec3 *min, vec3 *max, vec4 *center);
