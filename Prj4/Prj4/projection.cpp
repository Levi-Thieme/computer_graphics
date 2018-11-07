#include "projection.h"

/*
Sets the projection to perspective projection

fovy - field of view angle
aspect - aspect ratio
zNear - near distance for view volume
zFar - far distance for view volume
*/
void setPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, zNear, zFar);
}


/*
Sets the projection to parallel orthographic

viewVolume - the dimensions for viewing volume

*/
void setParallelOrtho(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(l, r, b, t, n, f);
}


/*
Loads modelview with identity and sets the camera location

cameraLocation - the location of camera
look - location to look at
up - specifies up vector for camera
*/
void lookAt(vec3 cameraLocation, vec3 look, vec3 up) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraLocation.x, cameraLocation.y, cameraLocation.z, look.x, look.y, look.z, up.x, up.y, up.z);
}