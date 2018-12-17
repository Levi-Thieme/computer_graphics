#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <numeric>
#include <algorithm>
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

int main(int argc, char** args) {

	cout << "Enter a filename.\n";
	string file_name;
	cin >> file_name;
	ifstream input_stream (file_name);
	if (!input_stream) {
		cout << "File not found.\n";
		exit(1);
	}
	
	string line;
	string format = "%d %d %d";
	std::vector<Vertex *> vertices;
	while (getline(input_stream, line)) {
		get_3D_vertex(line, &format, &vertices);
	}
	input_stream.close();

	string raw_file_name = file_name.substr(0, file_name.find_last_of("."));
	ofstream output_file(raw_file_name + ".out");
	double x_total = 0;
	double y_total = 0;
	double z_total = 0;

	int vertices_size = vertices.size();
	for (int i = vertices_size - 1; i >= 0; i--) {
		Vertex *v = vertices.at(i);
		print_vertex(&output_file, v);
		x_total += v->x;
		y_total += v->y;
		z_total += v->z;
		delete(v);
	}
	char avg[32];
	snprintf(avg, sizeof(avg), "%.1f", x_total / vertices_size);
	output_file << "Mean X-Coordinate =  " << avg << "\n";
	snprintf(avg, sizeof(avg), "%.1f", y_total / vertices_size);
	output_file << "Mean Y-Coordinate =  " << avg << "\n";
	snprintf(avg, sizeof(avg), "%.1f", z_total / vertices_size);
	output_file << "Mean Z-Coordinate =  " << avg << "\n";
	output_file.flush();
	output_file.close();
	std::cout << "Program Finished. Quitting...\n";
}