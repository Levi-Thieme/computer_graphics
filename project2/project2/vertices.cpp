#include "stdafx.h"
#include "vertices.h"
#include <iostream>

/*
Finds the largest x,y, and z values.
polys - a vector of polys to search.
max - vector to store maximum points.
*/
void find_largest_x_y_z(vec3 *vecs, int count, vec3 *max) {
	for (int i = 0; i < count; i++) {
		max->x = std::max(max->x, vecs[i].x);
		max->y = std::max(max->y, vecs[i].y);
		max->z = std::max(max->z, vecs[i].z);
	}
}

/*
Finds the minimum x,y, and z values.
polys - a vector of polys to search.
min - vector to store minimum points.
*/
void find_minimum_x_y_z(vec3 *vecs, int count, vec3 *min) {
	for (int i = 0; i < count; i++) {
		min->x = std::min(min->x, vecs[i].x);
		min->y = std::min(min->y, vecs[i].y);
		min->z = std::min(min->z, vecs[i].z);
	}
}

/*
Normalizes coordinates so that values are within a 0.0 - 1.0 range.
vec - the vector to normalize
max - vector containing maximum values
min - vector containing minimum values
*/
void normalize_vec3(vec3 *vec, const vec3 *max, const vec3 *min) {
	for (int i = 0; i < 3; i++) {
		if ((*max)[i] != 0 && (*vec)[i] >= 0) {
			(*vec)[i] = ((*vec)[i] / (*max)[i]);
		}
		else if ((*min)[i] != 0) {
			(*vec)[i] = -((*vec)[i] / (*min)[i]);
		}
	}
}

/*
Normalizes all vec3's in vecs.
count - the number of vec3's contained in vecs.
max - vector containing maximum values
min - vector containing minimum values
*/
void normalize_vecs3(vec3 *vecs, const int count, const vec3 *max, const vec3 *min) {
	for (int i = 0; i < count; i++) {
		normalize_vec3(&(vecs[i]), max, min);
	}
}

/*
Changes 0's to 1's for normalization
*/
void clean_normalization_vector(vec3 *v) {
	for (int i = 0; i < 3; i++) {
		if (*v[0] == 0) {
			*v[0] = 1;
		}
	}
}

void generate_random_normalized_vertices(vec3 *vecs, int count) {
	for (int i = 0; i < count; i++) {
		GLfloat x = rand();
		GLfloat y = rand();
		GLfloat z = rand();
		normalize_random(&x, INT_MAX);
		normalize_random(&y, INT_MAX);
		normalize_random(&z, INT_MAX);
		vecs[i].x = x;
		vecs[i].y = y;
		vecs[i].z = z;
	}
}

void normalize_random(GLfloat *p, const GLfloat max) {
	GLfloat normalizer = 1;
	int normalized = 0;
	while (!normalized) {
		if (*p < normalizer) {
			*p /= normalizer;
			normalized = 1;
		}
		else {
			normalizer *= 10;
		}
	}
}