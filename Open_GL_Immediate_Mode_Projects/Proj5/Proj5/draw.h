#pragma once
#include "vec.h"
#include <vector>
using namespace std;

typedef struct {
	vector<vec3*> *vertices; //Vertex indices
	GLfloat ambient[3];
	GLfloat diffuse[3];
	GLfloat emissive[3];
	GLfloat specular[3];


	void setAmbient(GLfloat light[3]) {
		ambient[0] = light[0];
		ambient[1] = light[1];
		ambient[2] = light[2];
	}

	void setDiffuse(GLfloat light[3]) {
		diffuse[0] = light[0];
		diffuse[1] = light[1];
		diffuse[2] = light[2];
	}

	void setEmissive(GLfloat light[3]) {
		emissive[0] = light[0];
		emissive[1] = light[1];
		emissive[2] = light[2];
	}

	void setSpecular(GLfloat light[3]) {
		specular[0] = light[0];
		specular[1] = light[1];
		specular[2] = light[2];
	}
} face;


/*
Draws a triangle mesh of objects.

verticies - the vertex vectors
normals - the normal vectors
faces - polygon faces
*/
void drawObject(vector<vec4*> &vertices, vector<vec4*> &normals, vector<face*> &faces, GLfloat shininess);