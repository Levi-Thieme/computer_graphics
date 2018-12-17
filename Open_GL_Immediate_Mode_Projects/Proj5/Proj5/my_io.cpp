#include <algorithm>
#include "my_io.h"
using namespace std;
/*
* Prints the points contained in the points vector.
* Points are delimited by a space.
* output_file - the file to write to.
* points - a vector containing the coordinates for a point.
*/
int print_vertex(ofstream *output_file, vec3 *v) {
	*output_file << v->x << " " << v->y << " " << v->z << "\n";
	output_file->flush();
	return 0;
}

/*
Solicits a file name from the user and attempts to instantiate
an output file stream.

solicitation - the message to be used in asking the user for a filename.
error_mesg - the message to be displayed if an invalid name is entered.
Returns an ifstream for the given file.
*/
ifstream solicit_file(string path, string solicitation, string error_msg, string &name) {
	cout << "Enter a filename.\n";
	string file_name;
	cin >> file_name;
	ifstream input_stream(path + file_name);
	if (!input_stream) {
		cout << "File not found.\n";
		system("pause");
		exit(1);
	}
	name = file_name;
	return input_stream;
}

/*
* This method parses a line to retrieve vertex coordinates
* and store them in the corresponding ints.

* line - a string to be scanned for coordinate points.
* format - a string specifying the format of the string.
* x, y, z - stores corresponding coordinates.
* Returns the number of tokens parsed.
*/
int get_vec3(const string line, const string *format, GLfloat *x, GLfloat *y, GLfloat *z) {
	int retval = sscanf_s(line.c_str(), (*format).c_str(), x, y, z);
	return retval;
}

/*
Checks to see if line equals jump_indicator. Ignores case.

line - the line to check
jump_indicator - the string that indicates a jump
Returns true if line == jump_indicator.
*/
bool is_jump_indicator(string line, string jump_indicator) {
	std::transform(line.begin(), line.end(), line.begin(), ::tolower);
	std::transform(jump_indicator.begin(), jump_indicator.end(), jump_indicator.begin(), tolower);
	return line == jump_indicator;
}

/*
Reads all available vec3's from the input stream.

vecs - Array of vec3.
input - stream to read vertex data from.
Returns the number of vec3's read from input and stored in vecs.
*/
int read_vecs3(vector<vec3 *> *vecs, vector<int> *jumps, ifstream *input, string *format, string *jump_indicator) {
	string line;
	int count = 0;
	while (getline(*input, line)) {	//while there are more lines
		GLfloat x = 0, y = 0, z = 0;
		int coordinates_parsed = get_vec3(line, format, &x, &y, &z);
		if (coordinates_parsed == 3) { //verify the line contains 3 points and retrieve them
			vec3 *vec = (vec3*)malloc(sizeof(vec3)); 
			if (vec == NULL) {
				cout << "Failure to allocate memory for vec3 in read_vecs3s function from my_io.cpp\n";
				return count;
			}
			vec->x = x;
			vec->y = y;
			vec->z = z;
			vecs->push_back(vec);
			count += 1;
		}
		else if (coordinates_parsed == 0) { //check for a jump and add the index to jumps
			if (is_jump_indicator(line, *jump_indicator) && count != 0) { //only add index to jumps if it's not a leading jump in the file
				jumps->push_back(count);
			}
		}
	}
	return count;
}

/*
Reads all available vec4's from the input stream.

vecs - Array of vec4.
input - stream to read vertex data from.
jump_indicator - A string representing a jump
Returns the number of vec4's read from input and stored in vecs.
*/
int read_vecs4(vector<vec4 *> *vecs, vector<int> *jumps, ifstream *input, string *format, string *jump_indicator) {
	string line;
	int count = 0;
	while (getline(*input, line)) {	//while there are more lines
		GLfloat x = 0, y = 0, z = 0;
		int coordinates_parsed = get_vec3(line, format, &x, &y, &z);
		if (coordinates_parsed == 3) { //verify the line contains 3 points and retrieve them
			vec4 *vec = (vec4*)malloc(sizeof(vec3));
			if (vec == NULL) {
				cout << "Failure to allocate memory for vec3 in read_vecs3s function from my_io.cpp\n";
				return count;
			}
			vec->x = x;
			vec->y = y;
			vec->z = z;
			vec->w = 1.0;
			vecs->push_back(vec);
			count += 1;
		}
		else if (coordinates_parsed == 0) { //check for a jump and add the index to jumps
			if (is_jump_indicator(line, *jump_indicator) && count != 0) { //only add index to jumps if it's not a leading jump in the file
				jumps->push_back(count);
			}
		}
	}
	return count;
}