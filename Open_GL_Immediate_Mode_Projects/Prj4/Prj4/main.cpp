#include "vertices.h"
#include "my_io.h"
#include "transformations.h"
#include "projection.h"


/*
Levi Thieme Project 4
CS 321 Computer Graphics
Dr. Kim
*/


int DISPLAY_WIDTH = 600;
int DISPLAY_HEIGHT = 400;
int VIEWPORT_WIDTH = 300;
int VIEWPORT_HEIGHT = 200;
vec2 VIEWPORT_ORIGIN(0, 0);
float ASPECT_RATIO = DISPLAY_WIDTH / DISPLAY_HEIGHT;
int multiViewports = 0;

//Vertices
vec4 *points;
//total number of vertices
int point_count;
//Indices of jumps
vector<int> jumps;
//Starting center point of objects
vec4 start_center(1.0);
//Origin offset vector for centering purposes
vec3 origin_offset(0.0);
//Vector for storing maximum x, y, z coordinates
vec3 vmax;
//Vector for storing minimum x, y, z coordinates
vec3 vmin;
//projection mode
enum pojectionType { ORTHO_T = 't', ORTHO_S = 's', ORTHO_F = 'f', PERSPECTIVE = 'p'};
//Stores current projection mode
pojectionType projectionMode = ORTHO_F;
//Camera location
vec3 cam_location(0);
//Camera look
vec3 look(0);
//Camera up
vec3 up(0);
//Square viewing-volume dimension
GLfloat view_volume_dimension = .8;

/*
Global MODEL_VIEW matrix
*/
mat4 model_view(1.0);

/*
Rotation matrix is global because my code doesn't work without it being global for some
super weird and annoying reason. 
Making the rotation matrix local causes nothing, but 
the major axis to be drawn.
*/
mat4 rotation_m(1.0);

void cleanup();
void draw_major_axis(mat4 modelView);
void parallel_ortho_front();
void parallel_ortho_top();
void parallel_ortho_side(int leftOrRight);
void perspective_projection();
void draw_viewport(pojectionType projectionMode);
void draw_image();
void draw_vertices(GLenum e, vec4 *vertices, mat4 modelView, int start, int end);
void calculate_width_and_height(vec3 *size);
void get_center(vec4 *center, vec3 size);
void set_transformation_factors(vec3 size, GLfloat *translate_x, GLfloat *translate_y, GLfloat *scale, GLfloat *rotate);
int set_viewport_location(GLint x, GLint y);
void center_viewport();
int set_viewport_size(GLfloat w, GLfloat h);

/*
Free allocated memory.
*/
void cleanup() {
	free(points);
}

/*
Initializes the window and its attributes.
*/
void init_window(int argc, char** argv) {
	glutInit(&argc, argv); //Initialize GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH); //Specify display mode
	glutInitWindowSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	glutInitWindowPosition(500, 400);
	glutCreateWindow("Project 4 Levi Thieme");
}

/*
Sets up the clear color, viewing volume, and view port.
*/
void other_init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glViewport(VIEWPORT_ORIGIN.x, VIEWPORT_ORIGIN.y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	parallel_ortho_front();
}


/*
Draws the major axis
*/
void draw_major_axis(mat4 modelView) {
	glLineWidth(3.0);
	glColor3f(1, 0, 0);
	GLfloat length = .60;

	glBegin(GL_LINES);
	//object center to pos-x
	glVertex4fv(modelView * start_center);
	glVertex4fv(modelView * vec4(start_center.x + length, start_center.y, start_center.z, 1));
	//object center to pos-y
	glVertex4fv(modelView * start_center);
	glVertex4fv(modelView * vec4(start_center.x, start_center.y + length, start_center.z, 1));
	//object center to pos-z
	glVertex4fv(modelView * start_center);
	glVertex4fv(modelView * vec4(start_center.x, start_center.y, start_center.z + length, 1));
	glEnd();
}

/*
Draw Vertices from start to end in the vertices array.

e - the drawing mode to use (OpenGL constant)
vertices - array of vec4's
start - the starting index
end - the ending index
*/
void draw_vertices(GLenum e, vec4 *vertices, mat4 modelView, int start, int end) {
	glLineWidth(1.0);
	glColor3f(0, .7, .7);
	glBegin(e);
	for (int i = start; i < (end - 1); i++) {
		glVertex4fv(modelView * points[i]);
		glVertex4fv(modelView * points[i + 1]);
	}
	glEnd();
}


/*
Draw the viewport
*/
void draw_viewport(pojectionType projectionMode) {
	glColor3f(1.0, 0, 0);
	glLineWidth(1.0);
	GLfloat w = view_volume_dimension - .01;
	GLfloat h = view_volume_dimension - .01;
	GLfloat z = view_volume_dimension - .01;
	switch (projectionMode) {
	case ORTHO_F:
		glBegin(GL_LINES);
		//Left side
		glVertex4f(-w, -h, 0, 1);
		glVertex4f(-w, h, 0, 1);
		//Top side
		glVertex4f(-w, h, 0, 1);
		glVertex4f(w, h, 0, 1);
		//Right side
		glVertex4f(w, h, 0, 1);
		glVertex4f(w, -h, 0, 1);
		//Bottom side
		glVertex4f(w, -h, 0, 1);
		glVertex4f(-w, -h, 0, 1);
		glEnd();
		break;
	case ORTHO_T:
		glBegin(GL_LINES);
		//bottom side
		glVertex4f(-w, h, z, 1);
		glVertex4f(w, h, z, 1);
		//left side
		glVertex4f(-w, h, -z, 1);
		glVertex4f(-w, h, z, 1);
		//top side
		glVertex4f(-w, h, -z, 1);
		glVertex4f(w, h, -z, 1);
		//right side
		glVertex4f(w, h, -z, 1);
		glVertex4f(w, h, z, 1);
		glEnd();
		break;
	case ORTHO_S:
		glBegin(GL_LINES);
		//top
		glVertex4f(w, h, -z, 1);
		glVertex4f(w, h, z, 1);
		//right
		glVertex4f(w, h, z, 1);
		glVertex4f(w, -h, z, 1);
		//bottom
		glVertex4f(w, -h, z, 1);
		glVertex4f(w, -h, -z, 1);
		//left
		glVertex4f(w, -h, -z, 1);
		glVertex4f(w, h, -z, 1);
		glEnd();
		break;
	case PERSPECTIVE:
		
		z = -1;
		w = 1.99;
		h = 1.99;
		glBegin(GL_LINES);
		//Left side
		glVertex4f(-w, -h, z, 1);
		glVertex4f(-w, h, z, 1);
		//Top side
		glVertex4f(-w, h, z, 1);
		glVertex4f(w, h, z, 1);
		//Right side
		glVertex4f(w, h, z, 1);
		glVertex4f(w, -h, z, 1);
		//Bottom side
		glVertex4f(w, -h, z, 1);
		glVertex4f(-w, -h, z, 1);
		glEnd();
		break;
	default:
		glBegin(GL_LINES);
		//Left side
		glVertex4f(-w, -h, 0, 1);
		glVertex4f(-w, h, 0, 1);
		//Top side
		glVertex4f(-w, h, 0, 1);
		glVertex4f(w, h, 0, 1);
		//Right side
		glVertex4f(w, h, 0, 1);
		glVertex4f(w, -h, 0, 1);
		//Bottom side
		glVertex4f(w, -h, 0, 1);
		glVertex4f(-w, -h, 0, 1);
		glEnd();
		break;
	}
}

/*
Draws the image.
*/
void draw_image() {
	int last_jump = 0, current_jump = 0;
	//Draw all vertices up to the last jump.
	int vertices_drawn = 0;

	for (int i = 0; i < jumps.size(); i++) {
		current_jump = jumps.at(i);
		//draw
		draw_vertices(GL_LINES, points, model_view, last_jump, current_jump); //draw src_points
																			  //update
		vertices_drawn += (last_jump - current_jump);
		last_jump = (current_jump);
	}

	/*
	Because the previous loop is based on jumps, it will not draw points after the last jump.
	This is only an issue for files that are not terminated with a jump indicator.
	This if statement will determine if there are any remaining points to draw.
	*/
	if (vertices_drawn < point_count) {
		draw_vertices(GL_LINES, points, model_view, last_jump, point_count); //draw remaining source points
	}

	draw_major_axis(model_view);
}

/*
Sets the projection to parallel orthographic with front elevation
*/
void parallel_ortho_front() {
	setParallelOrtho(-view_volume_dimension, view_volume_dimension, -view_volume_dimension, view_volume_dimension, 0, 8);
	up = vec3(0, 1, 0);
	cam_location = vec3(start_center.x, start_center.y, -3);
	lookAt(cam_location, look, up);
}

/*
Sets the projection to parallel orthographic with top elevation
*/
void parallel_ortho_top() {
	setParallelOrtho(-view_volume_dimension, view_volume_dimension, -view_volume_dimension, view_volume_dimension, 0, 8);
	cam_location = vec3(0, 3, 0);
	look.z = 0;
	up = vec3(0, 0, 1);
	lookAt(cam_location, look, up);
}

/*
Sets the projection to parallel orthographic with side elevation
*/
void parallel_ortho_side(int leftOrRight) {
	setParallelOrtho(-view_volume_dimension, view_volume_dimension, -view_volume_dimension, view_volume_dimension, 0, 8);
	if (leftOrRight) { //true left side, else right side
		cam_location.x = -3;
	}
	else {
		cam_location.x = 3;
	}
	cam_location.y = start_center.y;
	cam_location.z = 0;
	up = vec3(0, 1, 0);
	lookAt(cam_location, look, up);
}

/*
Sets the projection to perspective projection
*/
void perspective_projection() {
	//Calculate znear and zfar for viewing volume
	GLdouble zNear = 0.01;
	GLdouble zFar = 100;
	setPerspective(45, VIEWPORT_WIDTH / VIEWPORT_HEIGHT, zNear, zFar);
	cam_location.z = 3;
	cam_location = vec3(0, start_center.y, cam_location.z);
	up = vec3(0, 1, 0);
	look = vec3(start_center.x, start_center.y, start_center.z);
	lookAt(cam_location, look, up);
}


/*
Display function to be used in glutMainLoop().
*/
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (multiViewports) {
		//draw four viewports
		GLint width = DISPLAY_WIDTH / 2;
		GLint height = DISPLAY_HEIGHT / 2;

		//Top elevation
		glViewport(0, 0, width, height);
		parallel_ortho_top();
		//draw_viewport(ORTHO_T);
		draw_image();

		//Perspective
		glViewport(width, 0, width, height);
		perspective_projection();
		//draw_viewport(PERSPECTIVE);
		draw_image();

		//Front elevation
		glViewport(width, height, width, height);
		parallel_ortho_front();
		//draw_viewport(ORTHO_F);
		draw_image();

		//Side(left) elevation
		glViewport(0, height, width, height);
		parallel_ortho_side(1);
		//draw_viewport(ORTHO_S);
		draw_image();
	}
	else {
		draw_viewport(projectionMode);
		draw_image();
	}

	glFlush();
}

/*
Sets the width and height in size vector.
*/
void calculate_width_and_height(vec3 *size) {
	//Find absolute largest values 
	vec4 initialMax = model_view * points[0];
	vec3 vmax(initialMax.x, initialMax.y, initialMax.z);
	find_largest_x_y_z(points, point_count, model_view, &vmax);

	//Find minimum values
	vec3 vmin(vmax);
	find_minimum_x_y_z(points, point_count, model_view, &vmin);

	//Calculate the width and height
	calculate_width_height(&vmin, &vmax, size);
}

/*
Sets the center point in center vector.
*/
void get_center(vec4 *center, vec3 size) {
	(*center).x = size.x / 2.0;
	(*center).y = size.y / 2.0;
	(*center).z = size.z / 2.0;
}

/*
Set the values for translations, scaling, and rotating
*/
void set_transformation_factors(vec3 size, GLfloat *translate_x, GLfloat *translate_y, GLfloat *scale, GLfloat *rotate) {
	//left-right translation amount
	*translate_x = 0.10 * size.x;
	//up-down translation amount
	*translate_y = 0.10 * size.y;
	//scaling factor
	*scale = 0.10 * sqrt((size.x * size.x) + (size.y * size.y));
	//rotation (degrees)
	*rotate = 15.0;
}

/*
Sets the viewport location and handles boundaries.

The moved variable is used for creating the panning effect when moving the viewport.
If the viewport moves, then the objects in the world should be inversely translated 
relative to the viewport's translation. However, if the viewport is at a window boundary and 
the viewport is not moved, the world objects should not be inversely translated.

Returns nonzero if the viewport was moved to x or to y or both
*/
int set_viewport_location(GLint x, GLint y) {
	int moved = 0;
	//Left Translation
	if (x < VIEWPORT_ORIGIN.x) {
		if (x < 0) { VIEWPORT_ORIGIN.x = 0; }
		else { VIEWPORT_ORIGIN.x = x;  moved = 1; }
	}
	else if(x > VIEWPORT_ORIGIN.x) { //Right Translation
		if ((x + VIEWPORT_WIDTH) > DISPLAY_WIDTH) { VIEWPORT_ORIGIN.x = DISPLAY_WIDTH - VIEWPORT_WIDTH; }
		else { VIEWPORT_ORIGIN.x = x; moved = 1; }
	}

	//Down translation
	if (y < VIEWPORT_ORIGIN.y) {
		if (y < 0) { VIEWPORT_ORIGIN.y = 0; } else { VIEWPORT_ORIGIN.y = y; moved = 1;}
	}
	else if(y > VIEWPORT_ORIGIN.y){ //Up translation
		if ((y + VIEWPORT_HEIGHT > DISPLAY_HEIGHT)) { VIEWPORT_ORIGIN.y = DISPLAY_HEIGHT - VIEWPORT_HEIGHT; }
		else { VIEWPORT_ORIGIN.y = y; moved = 1; }
	}
	return moved;
}

/*
Centers the viewport in the window.
*/
void center_viewport() {
	//calculate x
	GLfloat x = (DISPLAY_WIDTH - VIEWPORT_WIDTH) / 2.0;
	GLfloat y = (DISPLAY_HEIGHT - VIEWPORT_HEIGHT) / 2.0;
	set_viewport_location(x, y);
}

/*
Sets the viewports size and handles boundaries
Returns -1 if viewport has decreased in size, 0 if no change has occurred,
or 1 if viewport size has increased.
*/
int set_viewport_size(GLfloat w, GLfloat h) {
	int change = 0;
	//Width increase
	if (w > VIEWPORT_WIDTH) {
		//Handle maximum boundaries
		if (VIEWPORT_ORIGIN.x + w > DISPLAY_WIDTH) {
			VIEWPORT_WIDTH = DISPLAY_WIDTH - VIEWPORT_ORIGIN.x;
		}
		else {
			VIEWPORT_WIDTH = w;
			change = 1;
		}
	}//Width decrease
	else {
		//Handle mininum boundaries
		if (w < 10) {
			VIEWPORT_WIDTH = 10;
		}
		else {
			VIEWPORT_WIDTH = w;
			change = -1;
		}
	}

	//Height increase
	if (h > VIEWPORT_HEIGHT) {
		if (VIEWPORT_ORIGIN.y + h > DISPLAY_HEIGHT) {
			VIEWPORT_HEIGHT = DISPLAY_HEIGHT - VIEWPORT_ORIGIN.y;
		}
		else {
			VIEWPORT_HEIGHT = h;
			change = 1;
		}
	}//Height decrease
	else {
		//Handle minimum boundaries
		if (h < 10) {
			VIEWPORT_HEIGHT = 10;
		}
		else {
			VIEWPORT_HEIGHT = h;
			change = -1;
		}
	}
	return change;
}

/*
Handles keyboard input
*/
void keyboard(unsigned char key, int x, int y) {
	rotation_m = mat4(1.0);
	mat4 scaling_m = mat4(1.0);
	mat4 translation_m = mat4(1.0);

	//Get width and height
	vec3 size(0.0);
	calculate_width_and_height(&size);

	//Set the amounts for transformation factors
	GLfloat translate_x, translate_y, scale, rotate;
	set_transformation_factors(size, &translate_x, &translate_y, &scale, &rotate);
	//Set viewport scaling factor
	GLfloat scale_percentage = 0.10;
	//Number of window coordinates for translating viewport
	GLfloat viewport_scale = VIEWPORT_HEIGHT * scale_percentage;
	/*
	Multiplier = 2 for inverse translation of objects when viewport is moved to create panning effect
	*/
	GLint translation_multiplier = 2;

	key = tolower(key);
	int modifier = glutGetModifiers();

	GLfloat v_width = VIEWPORT_WIDTH;
	GLfloat v_height = VIEWPORT_HEIGHT;
	GLfloat curr_ar;
	GLfloat new_ar;
	int width = DISPLAY_WIDTH / 4;
	int height = DISPLAY_HEIGHT / 4;

	//Calculate size of image
	calculate_width_and_height(&size);

	switch (key) {
	case 'u':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			//translate up
			if (projectionMode == ORTHO_T) {
				translate_m(0, 0, translate_y, &translation_m);
			}
			else {
				translate_m(0, translate_y, 0, &translation_m);
			}
		}
		else {
			//Move viewport up
			if (set_viewport_location(VIEWPORT_ORIGIN.x, VIEWPORT_ORIGIN.y + viewport_scale)) {
				//Translate object(s) down for panning effect
				if (projectionMode == ORTHO_T) {
					translate_m(0, 0, -scale_percentage * translation_multiplier, &translation_m);
				}
				else {
					translate_m(0, -scale_percentage * translation_multiplier, 0, &translation_m);
				}
			}
		}
		break;
	case 'd':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			//down
			if (projectionMode == ORTHO_T) {
				translate_m(0, 0, -translate_y, &translation_m);
			}
			else {
				translate_m(0, -translate_y, 0, &translation_m);
			}
		}
		else { 
			//Move viewport down
			if (set_viewport_location(VIEWPORT_ORIGIN.x, VIEWPORT_ORIGIN.y - viewport_scale)) {
				//Translate object(s) up for panning effect
				if (projectionMode == ORTHO_T) {
					translate_m(0, 0, scale_percentage * translation_multiplier, &translation_m);
				}
				else {
					translate_m(0, scale_percentage * translation_multiplier, 0, &translation_m);
				}
			}
		}
		break;
	case 'l':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			//left
			if (projectionMode == ORTHO_F) { translate_m(scale_percentage * translation_multiplier, 0, 0, &translation_m); }
			else if (projectionMode == ORTHO_T) { translate_m(scale_percentage * translation_multiplier, 0, 0, &translation_m); }
			else if (projectionMode == ORTHO_S) { translate_m(0, 0, -scale_percentage * translation_multiplier, &translation_m); }
			else if (projectionMode == PERSPECTIVE) { translate_m(-scale_percentage * translation_multiplier, 0, 0, &translation_m); }
		}
		else { 
			//Move viewport left
			if (set_viewport_location(VIEWPORT_ORIGIN.x - viewport_scale, VIEWPORT_ORIGIN.y)) {
				//move object right
				if (projectionMode == ORTHO_F) { translate_m(-scale_percentage * translation_multiplier, 0, 0, &translation_m); }
				else if (projectionMode == ORTHO_T) { translate_m(-scale_percentage * translation_multiplier, 0, 0, &translation_m); }
				else if (projectionMode == ORTHO_S) { translate_m(0, 0, scale_percentage * translation_multiplier, &translation_m); }
				else if (projectionMode == PERSPECTIVE) { translate_m(scale_percentage * translation_multiplier, 0, 0, &translation_m); }
			}
		}
		break;
	case 'r':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			//right
			if (projectionMode == ORTHO_F) { translate_m(-scale_percentage * translation_multiplier, 0, 0, &translation_m); }
			else if (projectionMode == ORTHO_T) { translate_m(-scale_percentage * translation_multiplier, 0, 0, &translation_m); }
			else if (projectionMode == ORTHO_S) { translate_m(0, 0, scale_percentage * translation_multiplier, &translation_m); }
			else if (projectionMode == PERSPECTIVE) { translate_m(scale_percentage * translation_multiplier, 0, 0, &translation_m); }
		}
		else {
			//Move viewport right
			if (set_viewport_location(VIEWPORT_ORIGIN.x + viewport_scale, VIEWPORT_ORIGIN.y)) {
				//move object left
				if (projectionMode == ORTHO_F) { translate_m(scale_percentage * translation_multiplier, 0, 0, &translation_m); }
				else if (projectionMode == ORTHO_T) { translate_m(scale_percentage * translation_multiplier, 0, 0, &translation_m); }
				else if (projectionMode == ORTHO_S) { translate_m(0, 0, -scale_percentage * translation_multiplier, &translation_m); }
				else if (projectionMode == PERSPECTIVE) { translate_m(-scale_percentage * translation_multiplier, 0, 0, &translation_m); }
			}
		}
		break;
	case '+':
		//scale up 
		scale_3d(new vec3(scale, scale, scale), &scaling_m);
		break;
	case '-':
		//scale down
		scale_3d(new vec3(-scale, -scale, -scale), &scaling_m);
		break;
	case 'x':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			//positive x-axis rotation applied to rotation matrix
			rotate_x(rotate, &rotation_m);
		}
		else {
			//negative x-axis rotation
			rotate_x(-rotate, &rotation_m);
		}
		break;
	case 'y':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			//positiive y-axis rotation
			rotate_y(rotate, &rotation_m);
		}
		else {
			//negative y-axis rotation
			rotate_y(-rotate, &rotation_m);
		}
		break;
	case 'z':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			//positive z-axis rotation
			rotate_z(rotate, &rotation_m);
		}
		else {
			//negative z-axis rotation
			rotate_z(-rotate, &rotation_m);
		}
		break;
	case 'e':
		if (!multiViewports) {
			if (modifier == GLUT_ACTIVE_SHIFT) {
				//Scale up viewport
				v_width = VIEWPORT_WIDTH + viewport_scale;
				v_height = VIEWPORT_HEIGHT + viewport_scale;
				if (set_viewport_size(v_width, v_height) == 1) {
					//scale down object if viewport size increased
					scale_3d(new vec3(-scale_percentage, -scale_percentage, -scale_percentage), &scaling_m);
				}
			}
			else {
				//Scale down viewport
				v_width = VIEWPORT_WIDTH - viewport_scale;
				v_height = VIEWPORT_HEIGHT - viewport_scale;
				if (set_viewport_size(v_width, v_height) == -1) {
					//scale up object if viewport size decreased
					scale_3d(new vec3(scale_percentage, scale_percentage, scale_percentage), &scaling_m);
				}
			}
			curr_ar = VIEWPORT_WIDTH / VIEWPORT_HEIGHT;
			new_ar = v_width / v_height;
		}
		break;
	case 'i':
		//reset transformation matrices
		rotation_m = mat4(1.0);
		scaling_m = mat4(1.0);
		translation_m = mat4(1.0);
		model_view = mat4(1.0);
		origin_offset = vec3(start_center.x, start_center.y, start_center.z);
		break;
	case 'p':
		//Perspective projection
		perspective_projection();
		projectionMode = PERSPECTIVE;
		break;
	case 'o':
		//Parallel orthographic front-elevation projection
		parallel_ortho_front();
		projectionMode = ORTHO_F;
		break;
	case 't':
		//Parallel orthographic top-elevation projection
		parallel_ortho_top();
		projectionMode = ORTHO_T;
		break;
	case 's' :
		//Parallel orthographic side(left)-elevation projection
		parallel_ortho_side(1);
		projectionMode = ORTHO_S;
		break;
	case 'v':
		if (multiViewports) {
			multiViewports = 0;
		}
		else {
			multiViewports = 1;
		}
		break;
	case 'w':
		//Center the viewport
		center_viewport();
		draw_viewport(projectionMode);
		break;
	case 'f':
		//move cop away by 10%
		cam_location.z *= 1.1;
		lookAt(cam_location, look, up);
		break;
	case 'n':
		//move cop closer by 10%, COP cannot pass projection plane
		//if ((cam_location.z * 0.9) > 0.1) {
			cam_location.z *= 0.9;
			lookAt(cam_location, look, up);
		//}
		break;
	case 'q':
		cleanup();
		exit(0);
		break;
	}

	//Get values for setting to/from origin translation matrices
	origin_offset.x += translation_m[0][3];
	origin_offset.y += translation_m[1][3];
	origin_offset.z += translation_m[2][3];

	//setup translation to origin
	mat4 to_origin_translation(1.0);
	translate_m(-origin_offset.x, -origin_offset.y, -origin_offset.z, &to_origin_translation);

	//setup translation back to original location
	mat4 from_origin_translation(1.0);
	translate_m(origin_offset.x, origin_offset.y, origin_offset.z, &from_origin_translation);

	/*
	Set model_view to contain all transformations

	Transformations: translate to origin --> scale --> rotate --> translate back to original location --> translate
	*/
	model_view = translation_m * from_origin_translation * rotation_m * scaling_m * to_origin_translation * model_view;

	if (!multiViewports) {
		//Set viewport
		glViewport(VIEWPORT_ORIGIN.x, VIEWPORT_ORIGIN.y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	}

	//Mark for redisplay
	glutPostRedisplay();
}

void resize(int width, int height) {
	//Set values for viewport
	glViewport(VIEWPORT_ORIGIN.x, VIEWPORT_ORIGIN.y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

	DISPLAY_WIDTH = width;
	DISPLAY_HEIGHT = height;
}

int main(int argc, char **argv) {
	ifstream inp = solicit_file("Please enter a filename.", "File not found.");
	string format = "%f %f %f";
	string jump_indicator = "J";

	vector<vec4 *> vec4_vector;

	point_count = read_vecs4(&vec4_vector, &jumps, &inp, &format, &jump_indicator); //Load the vertices and jumps from data file
	if (point_count < 1) {
		cout << "Unable to read any points from data file. Quitting...\n";
		system("pause");
		return -1;
	}

	//allocate points array to store modifiable vertices.
	points = (vec4*)malloc(sizeof(vec4) * vec4_vector.size());

	if (points == NULL) {
		cout << "Unable to allocate memory for points. Quitting...\n";
		system("pause");
		return -2;
	}

	for (int i = 0; i < vec4_vector.size(); i++) {
		//Transfer vertices from the vector to vec4 array.
		points[i] = *(vec4_vector.at(i));
	}

	//Find absolute largest values for normalization from -1.0 to 1.0
	vmax = 0;
	find_abs_largest_x_y_z(points, point_count, model_view, &vmax);

	//Find minimum values
	vmin = vec3(vmax);
	find_minimum_x_y_z(points, point_count, model_view, &vmin);

	//Normalize the vertices to -1.0 to 1.0
	normalize_vecs4(points, point_count, &vmax);

	//Normalize the min and max vectors to be used in calculating normalized width/height/center
	normalize_vec3(&vmin, &vmax);
	normalize_vec3(&vmax, &vmax);

	//Calculate center point
	start_center.w = 1.0;
	calculate_center(&vmin, &vmax, &start_center);

	origin_offset = vec3(start_center.x, start_center.y, start_center.z);

	VIEWPORT_ORIGIN = (0, 0);
	center_viewport();

	//Initialize windows and display image
	init_window(argc, argv);
	other_init();
	glewInit();
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutMainLoop();
	system("pause");
	cleanup();
	return 0;
}
