#include "vertices.h"
using namespace std;

/*
* This method parses a line to retrieve vertex coordinates
* and store them in the corresponding vectors.
* Returns 0 if a vertex was successfully read from line. Otherwise, returns 1.
* line - a string to be scanned for coordinate points.
* format - a string specifying the format of the string.
* x_list - a vector containing x-coordinates.
* y_list - a vector containing y-coordinates.
* z_list - a vector containing z-coordinates.
*/
int get_3D_vertex(const string line, const string *format, vector<Vertex *> *vertices) {
	int x, y, z;
	if (sscanf_s(line.c_str(), (*format).c_str(), &x, &y, &z)) {
		Vertex *v = new Vertex();
		v->x = x;
		v->y = y;
		v->z = z;
		vertices->push_back(v);
		return 0;
	}
	return 1;
}

/*
* Prints the points contained in the points vector.
* Points are delimited by a space.
* output_file - the file to write to.
* points - a vector containing the coordinates for a point.
*/
int print_vertex(ofstream *output_file, Vertex *v) {
	*output_file << v->x << " " << v->y << " " << v->z << "\n";
	output_file->flush();
	return 0;
}
