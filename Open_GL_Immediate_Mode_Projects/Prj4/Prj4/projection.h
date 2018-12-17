#pragma once

#include "Angel.h"

/*
Sets the projection to perspective projection

fovy - field of view angle
aspect - aspect ratio
zNear - near distance for view volume
zFar - far distance for view volume
*/
void setPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);


/*
Sets the projection to parallel orthographic

viewVolum - the dimensions for viewing volume
cameraLocation - the location of camera
look - location to look at
up - specifies up vector for camera
*/
void setParallelOrtho(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);


/*
Loads modelview with identity and sets the camera location
*/
void lookAt(vec3 cameraLocation, vec3 look, vec3 up);
