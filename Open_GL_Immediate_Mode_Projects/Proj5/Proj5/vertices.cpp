#include "vertices.h"
#include <iostream>

/*
Finds the largest x,y, and z values.
vecs - a vector to search.
max - vector to store maximum points.
*/
void find_abs_largest_x_y_z(vec3 *vecs, int count, vec3 *max) {
	for (int i = 0; i < count; i++) {
		max->x = std::max(max->x, std::abs(vecs[i].x));
		max->y = std::max(max->y, std::abs(vecs[i].y));
		max->z = std::max(max->z, std::abs(vecs[i].z));
	}
}

/*
Finds the largest x,y, and z values.
vecs - a vector to search.
max - vector to store maximum points.
*/
void find_abs_largest_x_y_z(vec4 *vecs, int count, mat4 modelView, vec3 *max) {

	for (int i = 0; i < count; i++) {
		vec4 temp = modelView * vecs[i];
		max->x = std::max(max->x, std::abs(temp.x));
		max->y = std::max(max->y, std::abs(temp.y));
		max->z = std::max(max->z, std::abs(temp.z));
	}
}

/*
Finds the largest x,y, and z values.
vecs - a vector to search.
max - vector to store maximum points.
*/
void find_largest_x_y_z(vec4 *vecs, int count, mat4 modelView, vec3 *max) {
	for (int i = 0; i < count; i++) {
		vec4 temp = modelView * vecs[i];
		max->x = std::max(max->x, temp.x);
		max->y = std::max(max->y, temp.y);
		max->z = std::max(max->z, temp.z);
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
Finds the minimum x,y, and z values.
polys - a vector of polys to search.
min - vector to store minimum points.
*/
void find_minimum_x_y_z(vec4 *vecs, int count, mat4 modelView, vec3 *min) {
	for (int i = 0; i < count; i++) {
		vec4 temp = modelView * vecs[i];
		min->x = std::min(min->x, temp.x);
		min->y = std::min(min->y, temp.y);
		min->z = std::min(min->z, temp.z);
	}
}

/*
Normalizes coordinates so that values are within a 0.0 - 1.0 range.
vec - the vector to normalize
max - vector containing maximum values
min - vector containing minimum values
*/
void normalize_vec3(vec3 *vec, const vec3 *max) {
	for (int i = 0; i < 3; i++) {
		if ((*max)[i] != 0) {
			(*vec)[i] = ((*vec)[i] / (*max)[i]);
		}
	}
}

/*
Normalizes coordinates so that values are within a 0.0 - 1.0 range.
vec - the vector to normalize
max - vector containing maximum values
min - vector containing minimum values
*/
void normalize_vec4(vec4 *vec, const vec3 *max) {
	for (int i = 0; i < 4; i++) {
		if ((*max)[i] != 0) {
			(*vec)[i] = ((*vec)[i] / (*max)[i]);
		}
	}
}

/*
Normalizes all vec3's in vecs.
count - the number of vec3's contained in vecs.
max - vector containing maximum values
min - vector containing minimum values
*/
void normalize_vecs3(vec3 *vecs, const int count, const vec3 *max) {
	for (int i = 0; i < count; i++) {
		normalize_vec3(&(vecs[i]), max);
	}
}

/*
Normalizes all vec4's in vecs.
count - the number of vec4's contained in vecs.
max - vector containing maximum values
min - vector containing minimum values
*/
void normalize_vecs4(vec4 *vecs, const int count, const vec3 *max) {
	for (int i = 0; i < count; i++) {
		normalize_vec4(&(vecs[i]), max);
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

/*
Calculates the largest difference in x-coordinates for points.
points - An array of vectors
point_count - The number of vectors contained in points array
*/
void calculate_width_height(vec3 *min, vec3 *max, vec3 *differences) {
	differences->x = max->x - min->x;
	differences->y = max->y - min->y;
	differences->z = max->z - min->z;
}

/*
Calculates the center based on min and max vertices

min - vector containing the minimum values for x,y,z
max - vector containing the maximum values for x,y,z
center - vector to store the center vertex in
*/
void calculate_center(vec3 *min, vec3 *max, vec4 *center) {
	center->x = (min->x + max->x) / 2;
	center->y = (min->y + max->y) / 2;
	center->z = (min->z + max->z) / 2;
}