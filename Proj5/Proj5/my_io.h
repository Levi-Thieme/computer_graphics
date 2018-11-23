#ifndef MY_IO_H
#define MY_IO_H
#endif

#include <string>
#include <fstream>
#include <vector>
#include "Angel.h"



using namespace std;

/*
* Prints the points contained in the points vector.
* Points are delimited by a space.
* output_file - the file to write to.
* points - a vector containing the coordinates for a point.
*/
int print_vertex(ofstream *output_file, vec3 *v);

/*
Solicits a file name from the user and attempts to instantiate
an output file stream.

solicitation - the message to be used in asking the user for a filename.
error_mesg - the message to be displayed if an invalid name is entered.
Returns an ifstream for the given file.
*/
std::ifstream solicit_file(string path, string solicitation, string error_mesg, string &name);

/*
* This method parses a line to retrieve vertex coordinates
* and store them in the corresponding ints.
* Returns 0 if a vertex was successfully read from line. Otherwise, returns 1.
* line - a string to be scanned for coordinate points.
* format - a string specifying the format of the string.
* x, y, z - stores corresponding coordinates.
*/
int get_vec3(const string line, const string *format, GLfloat *x, GLfloat *y, GLfloat *z);

/*
Reads all available vec3's from the input stream.

vecs - Array of vec3.
input - stream to read vertex data from.
jump_indicator - A string representing a jump
Returns the number of vec3's read from input and stored in vecs.
*/
int read_vecs3(vector<vec3 *> *vecs, vector<int> *jumps, ifstream *input, string *format, string *jump_indicator);

/*
Reads all available vec4's from the input stream.

vecs - Array of vec4.
input - stream to read vertex data from.
jump_indicator - A string representing a jump
Returns the number of vec4's read from input and stored in vecs.
*/
int read_vecs4(vector<vec4 *> *vecs, vector<int> *jumps, ifstream *input, string *format, string *jump_indicator);
