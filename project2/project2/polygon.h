#ifndef POLYGON_H
#define POLYGON_H
#endif

#include "vertices.h"
#include <vector>


typedef struct {
	vector<vec3 *> *vertices;
} Poly;

/*
Initializes a polygon's vertices

poly - the polygon to initialize.
input_stream - the input stream to read points from.
format - the format to use when reading from input_stream.
Returns 0 if successful.
*/
int init_polygon(Poly *poly, ifstream *input_stream, string *format);

/*
Initializes the maximum number of polygons based on the input_stream.

polys - a vector of polygons to initialize.
input_stream - the input stream to read points from.
format - the format to use when reading from input_stream.
Returns 0 if successful.
*/
int init_polygons(vector<Poly *> *polys, ifstream *input_stream, string *format);