#include "stdafx.h"
#include "polygon.h"
#include <algorithm>
#include <fstream>
#include "vec.h"

/*
Initializes a polygon's vertices

poly - the polygon to initialize.
input_stream - the input stream to read points from.
format - the format to use when reading from input_stream.
Returns the number of vertices read.
*/
int init_polygon(Poly *poly, ifstream *input_stream, string *format) {
	poly->vertices = new vector<vec3 *>(); //initalize vertices vector for the polygon
	string line;
	int i = 0;
	//Get lines and read their vertices until a jump is encountered
	while (getline(*input_stream, line)) {
		if (i == 0 && (line == "j" || line == "J")) { //skip first leading jump in file
			continue;
		}
		else if (line != "j" && line != "J") {
			//get_vec3(line, format, poly->vertices);
			i += 1;
		}
		else {
			break;
		}
	}
	return i;
}

/*
Initializes the maximum number of polygons based on the input_stream.

polys - a vector of polygons to initialize.
input_stream - the input stream to read points from.
format - the format to use when reading from input_stream.
Returns 0 if successful.
*/
int init_polygons(vector<Poly *> *polys, ifstream *input_stream, string *format) {
	while (!input_stream->eof()) {
		Poly *current_poly = (Poly*)malloc(sizeof(Poly));
		if (current_poly == NULL) {
			cout << "Failure to allocate memory for polygon in function init_polygons from polygon.cpp";
			return 1;
		}
		int read = 0;
		do {
			read = init_polygon(current_poly, input_stream, format);
		} while (read == 0 && !input_stream->eof());
		if (read > 0) {
			polys->push_back(current_poly);
		}
	}
}