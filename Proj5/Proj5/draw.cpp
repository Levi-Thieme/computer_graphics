
#include "draw.h"

/*
Draws a triangle mesh of objects.

verticies - the vertex vectors
normals - the normal vectors
faces - polygon faces
*/
void drawObject(vector<vec4*> &vertices, vector<vec4*> &normals, vector<face*> &faces, GLfloat shininess) {

	for (int i = 0; i < faces.size(); i++) {

		glBegin(GL_POLYGON);

		face f = *(faces.at(i));

		glMaterialfv(GL_FRONT, GL_AMBIENT, f.ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, f.diffuse);
		glMaterialfv(GL_FRONT, GL_EMISSION, f.emissive);
		glMaterialfv(GL_FRONT, GL_SPECULAR, f.specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
		

		//Draw the face's normal and vertices
		vector<vec3*> faceVertices = *(f.vertices);
		for (int j = faceVertices.size() - 1; j >= 0; j--) {

			vec3 *faceVertex = faceVertices.at(j);

			//Draw normal
			int index = (*faceVertex)[2];
			if (index != -1) {
				vec4 *n = normals.at(index-1);
				glNormal3f(n->x, n->y, n->z);
			}

			//Draw vertex coordinate
			index = (*faceVertex)[0];
			if (index != -1) {
				vec4 *n = vertices.at(index - 1);
				glVertex3f(n->x, n->y, n->z);
			}
		}

		glEnd();
	}
}