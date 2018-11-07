#include "transformations.h"

void flip_y(vec3 *vertices, int v_count) {
	for (int i = 0; i < v_count; i++) {
		if (vertices[i].y != 0) { //Multiplying by -1 for a floating point data type equal to 0 results in -0
			vertices[i].y *= -1;
		}
	}
}

void flip_x(vec3 *vertices, int v_count) {
	for (int i = 0; i < v_count; i++) {
		if (vertices[i].x != 0) { //Multiplying by -1 for a floating point data type equal to 0 results in -0
			vertices[i].x *= -1;
		}
	}
}

void translate_vec3(vec3 *vec, const GLfloat dx, const GLfloat dy, const GLfloat dz) {
	vec->x = vec->x + dx;
	vec->y = vec->y + dy;
	vec->z = vec->z + dz;
}

void translate_vec4(vec4 *vec, const GLfloat dx, const GLfloat dy, const GLfloat dz) {
	vec->x = vec->x + dx;
	vec->y = vec->y + dy;
	vec->z = vec->z + dz;
}


void translate_image3D(vec3 *vecs, int count, const GLfloat dx, const GLfloat dy, const GLfloat dz) {
	for (int i = 0; i < count; i++) {
		translate_vec3(&vecs[i], dx, dy, dz);
	}
}

void translate_image4D(vec4 *vecs, int count, const GLfloat dx, const GLfloat dy, const GLfloat dz) {
	for (int i = 0; i < count; i++) {
		translate_vec4(&vecs[i], dx, dy, dz);
	}
}


void scale_vec3(vec3 *vec, const GLfloat dx, const GLfloat dy, const GLfloat dz) {
	if (std::abs(vec->x) > .2) { vec->x = vec->x * dx; }
	else { while (std::abs(vec->x) < .8) { vec->x = vec->x * (1 + std::abs(dx - 1)); } }
	if (std::abs(vec->y) > .2) { vec->y = vec->y * dy; }
	else { while (std::abs(vec->y) < .8) { vec->y = vec->y * (1 + std::abs(dy - 1)); } }
	if (std::abs(vec->z) > .2) { vec->z = vec->z * dz; }
}

/*
Some messed up scaling function to produce interesting visual effects.
*/
void scale_image(vec3 *vecs, int count, const GLfloat dx, const GLfloat dy, const GLfloat dz) {
	for (int i = 0; i < count; i++) {
		scale_vec3(&vecs[i], dx, dy, dz);
	}
}

/*
Calculates the difference between src and dest and stores in diff.

src - the source vertex
dest - the destination vertex
diff - vector containing difference
*/
void abs_difference(vec3 *src, vec3 *dest, vec3 *diff) {
	diff->x = std::abs(src->x - dest->x);
	diff->y = std::abs(src->y - dest->y);
	diff->z = std::abs(src->z - dest->z);
}

/*
Calculates amount / slices and stores in delta.

delta - a vector containing deltas for x, y, and z
slices - the number of slices
delta - the vec3 to store the delta slice for each delta.
*/
void slice_delta(vec3 *deltas, int slices, vec3 *delta_slice) {
	delta_slice->x = (deltas->x / slices);
	delta_slice->y = (deltas->y / slices);
	delta_slice->z = (deltas->z / slices);
}

/*
Fills the deltas array with deltas based on src, dest, and slices.

deltas - Array to store delta for src and dest vertices
src - Array storing source vertices.
dest - Array storing destination vertices.
v_count - the number of vertices in deltas, src, and dest.
slices - the number of slices applied to the difference of src and dest to calculate delta.
*/
void calculate_deltas(vec3 *deltas, vec3 *src, vec3 *dest, const GLint v_count, const GLint slices) {
	for (int i = 0; i < v_count; i++) {
		abs_difference(&src[i], &dest[i], &(deltas[i]));
	}
}

/*
Translates v towards dest by delta amount.

v - the vector to translate.
dest - the destination to translate towards.
delta - the amount to translate by.
*/
void translate_to(vec3 *v, const vec3 *dest, const vec3 *delta) {
	for (int i = 0; i < 3; i++) {
		GLfloat *coordinate = &((*v)[i]);
		if ((*dest)[i] - *coordinate > 0) {
			*coordinate += (*delta)[i];
		}
		else if ((*dest)[i] - *coordinate < 0) {
			*coordinate -= (*delta)[i];
		}
	}
}

/*
Sets the values for a translation matrix.
dx - delta x
dy - delta y
dz - delta z
translation_mat - the matrix to hold the translation values
*/
void translate_m(GLfloat dx, GLfloat dy, GLfloat dz, mat4 *translation_mat) {
	/*GLfloat x = dx + *translation_mat[0][3];
	GLfloat y = dy + *translation_mat[1][3];
	GLfloat z = dz + *translation_mat[2][3];*/
	GLfloat x = dx ;
	GLfloat y = dy ;
	GLfloat z = dz ;

	*translation_mat =
		{	1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1	};
}

/*
Applies scaling to scale_m

s_vec - vector containing scaling factors
scale_m - scaling matrix
*/
void scale_m(vec3 *s_vec, mat4 *scale_m) {
	GLfloat sx = s_vec->x + *scale_m[0][0];
	GLfloat sy = s_vec->y + *scale_m[1][1];
	GLfloat sz = s_vec->z + *scale_m[2][2];
	GLfloat min = .001;
		*scale_m =
		{ sx, 0, 0, 0,
			0, sy, 0, 0,
			0, 0, sz, 0,
			0, 0, 0, 1 };
}

/*
Rotates with respect to y-axis
theta - the amount of rotation(degrees)
*/
void rotate_y(GLfloat theta, mat4 *rotation_m) {
	theta = theta * (M_PI / 180);
	//sin(theta)
	GLfloat s = std::sin(theta);
	//cos(theta)
	GLfloat c = std::cos(theta);
	*rotation_m = { c, 0, s, 0,
					0, 1, 0, 0,
					-s, 0, c, 0,
					0, 0, 0, 1	};
}

/*
Rotates with respect to x-axis
theta - the amount of rotation(degrees)
*/
void rotate_x(GLfloat theta, mat4 *rotation_m) {
	theta = theta * (M_PI / 180);
	//sin(theta)
	GLfloat s = std::sin(theta);
	//cos(theta)
	GLfloat c = std::cos(theta);
	*rotation_m = { 
		1, 0, 0, 0,
		0, c, s, 0,
		0, -s, c, 0,
		0, 0, 0, 1 };
}

/*
Rotates with respect to z-axis
theta - the amount of rotation(degrees)
*/
void rotate_z(GLfloat theta, mat4 *rotation_m) {
	theta = theta * (M_PI / 180);
	//sin(theta)
	GLfloat s = std::sin(theta);
	//cos(theta)
	GLfloat c = std::cos(theta);
	*rotation_m = { 
		c, s, 0, 0,
		-s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
}