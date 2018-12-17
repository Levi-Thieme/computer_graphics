#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#endif

#include "vec.h"
#include "draw.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

void read_model_object(ifstream &file, vector<vec4*> &vertices, vector<vec4*> &normals, vector<face*> &faces);

void getFaceVertices(string &line, vector<vec3*> *vertices);

void findMaxMin(vector<vec4*> vertices, vec4 &min, vec4 &max);
