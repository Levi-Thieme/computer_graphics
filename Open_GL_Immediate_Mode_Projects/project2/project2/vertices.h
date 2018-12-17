#ifndef VERTICES_H
#define VERTICES_H
#endif

#include "Angel.h"
#include <string>
#include <algorithm>



using namespace std;

/*
Finds the largest x,y, and z values.
polys - a vector of polys to search.
max - vector to store maximum points.
*/
void find_largest_x_y_z(vec3 *vecs, int count, vec3 *max);

/*
Finds the minimum x,y, and z values.
polys - a vector of polys to search.
min - vector to store minimum points.
*/
void find_minimum_x_y_z(vec3 *vec, int count, vec3 *min);

/*
Normalizes coordinates so that values are within a 0.0 - 1.0 range.
vec - the vector to normalize
max - vector containing maximum values
min - vector containing minimum values
*/
void normalize_vec3(vec3 *vec, const vec3 *max);

/*
Normalizes all vec3's in vecs.
count - the number of vec3's contained in vecs.
max - vector containing maximum values
min - vector containing minimum values
*/
void normalize_vecs3(vec3 *vecs, const int count, const vec3 *max);

/*
Changes 0's to 1's for normalization
*/
void clean_normalization_vector(vec3 *v);

void generate_random_normalized_vertices(vec3 *vecs, int count);

void normalize_random(GLfloat *p, const GLfloat max);