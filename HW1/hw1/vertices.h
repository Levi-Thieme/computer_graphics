#include <string>
#include <vector>
#include <fstream>
using namespace std;

/*
* A struct containing x, y, and z coordinates.
*/
typedef struct {
	int x;
	int y;
	int z;
} Vertex;

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
int get_3D_vertex(const std::string line, const std::string *format, vector<Vertex *> *vertices);

/*
* Prints the points contained in the points vector.
* Points are delimited by a space.
* output_file - the file to write to.
* points - a vector containing the coordinates for a point.
*/
int print_vertex(ofstream *output_file, Vertex *v);
