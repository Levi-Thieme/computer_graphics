
#include "obj_loader.h"
using namespace std;

void set(GLfloat *p, GLfloat r, GLfloat g, GLfloat b) {
	p[0] = r;
	p[1] = g;
	p[2] = b;
}

/*
Reads a .obj file.
Only handles vertices, normals, faces, ambient, diffuse, specular, and emissive lighting.
*/
void read_model_object(ifstream &file, vector<vec4*> &vertices, vector<vec4*> &normals, vector<face*> &faces) {
	string line;
	char t[3];
	float x = 0, y = 0, z = 0;
	float r = 0, g = 0, b = 0, a = 1;
	GLfloat diffuse[3] = { r, g, b };
	GLfloat ambient[3] = { r, g, b };
	GLfloat specular[3] = { r, g, b };
	GLfloat emissive[3] = { r, g, b };

	while (!getline(file, line).eof()) {
		if (line.length() < 2) { continue; }

		if (sscanf_s(line.c_str(), "%s %f %f %f", &t, sizeof(t), &x, &y, &z) == 4) {
			
			if (strcmp(t, "v") == 0) {
				vertices.push_back(new vec4(x, y, z, 1));
			}
			else if (strcmp(t, "vn") == 0) {
				normals.push_back(new vec4(x, y, z, 1));
			}
			else if (t[0] == 'k') {
				r = x;
				g = y;
				b = z;

				if (strcmp(t, "kd") == 0) {
					set(diffuse, r, g, b);
				}
				else if (strcmp(t, "ka") == 0) {
					set(ambient, r, g, b);
				}
				else if (strcmp(t, "ke") == 0) {
					set(emissive, r, g, b);
				}
				else if (strcmp(t, "ks") == 0) {
					set(specular, r, g, b);
				}
			}
		}
		else if (line[0] == 'f') {
			face *f = new face();
			f->setDiffuse(diffuse);
			f->setAmbient(ambient);
			f->setEmissive(emissive);
			f->setSpecular(specular);

			vector<vec3*> *verts = NULL;
			verts = new vector<vec3*>;
			f->vertices = verts;
			getFaceVertices(line, f->vertices);
			//Add the face to faces vector
			faces.push_back(f);
		}
	}
}

void getFaceVertices(string &line, vector<vec3*> *vertices) {
	int a, b, c, d, e, f;
	char t[3];
	if (sscanf_s(line.c_str(), "%s  %d//%d  %d//%d  %d//%d", &t, sizeof(t), &a, &b, &c, &d, &e, &f) == 7) {
		vec3 *v1 = new vec3(a, -1, b);
		vertices->push_back(v1);
		vec3 *v2 = new vec3(c, -1, d);
		vertices->push_back(v2);
		vec3 *v3 = new vec3(e, -1, f);
		vertices->push_back(v3);
	}
}

void findMaxMin(vector<vec4*> vertices, vec4 &min, vec4 &max) {
	min = vec4(*vertices[0]);
	max = vec4(min);
	for (int i = 1; i < vertices.size(); i++) {
		vec4 v = *(vertices[i]);
		if (v.x < min.x) {
			min.x = v.x;
		}
		if (v.y < min.y) {
			min.y = v.y;
		}
		if (v.z < min.z) {
			min.z = v.z;
		}
		if (v.x > max.x) {
			max.x = v.x;
		}
		if (v.y > max.y) {
			max.y = v.y;
		}
		if (v.z > max.z) {
			max.z = v.z;
		}
	}
}