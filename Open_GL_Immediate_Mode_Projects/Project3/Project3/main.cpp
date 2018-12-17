#include "vertices.h"
#include "my_io.h"
#include "transformations.h"


int DISPLAY_WIDTH = 500;
int DISPLAY_HEIGHT = 500;
int VIEWPORT_WIDTH = DISPLAY_WIDTH;
int VIEWPORT_HEIGHT = DISPLAY_HEIGHT;
vec2 VIEWPORT_ORIGIN;
float ASPECT_RATIO = DISPLAY_WIDTH / DISPLAY_HEIGHT;

vec4 *points;
vec4 center;
vec3 vmax;
vec3 vmin;
int point_count;
vector<int> jumps;
mat4 model_view(1.0);
mat4 translation_m(1.0);
mat4 rotation_m(1.0);
mat4 scaling_m(1.0);

/*
Free allocated memory.
*/
void cleanup() {
	free(points);
}

/*
Draws the major axis
*/
void draw_major_axis() {
	glLineWidth(3.0);
	glColor3f(1, 0, 0);
	GLfloat length = .60;

	glBegin(GL_LINES);
	//object center to pos-x
	glVertex4fv(model_view * vec4(center.x, center.y, center.z, 1));
	glVertex4fv(model_view * vec4(center.x + length, center.y, center.z, 1));
	//object center to pos-y
	glVertex4fv(model_view * vec4(center.x, center.y, center.z, 1));
	glVertex4fv(model_view * vec4(center.x, center.y + length, center.z, 1));
	//object center to pos-z
	glVertex4fv(model_view * vec4(center.x, center.y, center.z, 1));
	glVertex4fv(model_view * vec4(center.x, center.y, center.z + length, 1));
	glEnd();
}

/*
Initializes the window and its attributes.
*/
void init_window(int argc, char** argv) {
	glutInit(&argc, argv); //Initialize GLUT
	glutInitDisplayMode(GLUT_RGB); //Specify display mode
	glutInitWindowSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	glutInitWindowPosition(500, 400);
	glutCreateWindow("Project 3 Levi Thieme");
}

/*
Sets up the clear color, viewing volume, and view port.
*/
void other_init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.5, 1.5, -1.5, 1.5, -2.0, 2.0);
	glViewport(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
Draw Vertices from start to end in the vertices array.

e - the drawing mode to use (OpenGL constant)
vertices - array of vec4's
start - the starting index
end - the ending index
*/
void draw_vertices(GLenum e, vec4 *vertices, int start, int end) {
	glLineWidth(1.0);
	glColor3f(0, .7, .7);
	glBegin(e);
	for (int i = start; i < (end - 1); i++) {
		glVertex4fv(model_view * points[i]);
		glVertex4fv(model_view * points[i + 1]);
	}
	glVertex4fv(vec4(0));
	glVertex4fv(vec4(.8, .8, 0, 1));
	glEnd();
}

/*
Display function to be used in glutMainLoop().
*/
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	int last_jump = 0, current_jump = 0;
	//Draw all vertices up to the last jump.
	int vertices_drawn = 0;

	for (int i = 0; i < jumps.size(); i++) {
		current_jump = jumps.at(i);
		//draw
		draw_vertices(GL_LINES, points, last_jump, current_jump); //draw src_points
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
		draw_vertices(GL_LINES, points, last_jump, point_count); //draw remaining source points
	}

	draw_major_axis();

	glFlush();
}

/*
Handles keyboard input
*/
void keyboard(unsigned char key, int x, int y) {
	/*
	Find the width and height
	*/
	//Find absolute largest values 
	find_largest_x_y_z(points, point_count, &vmax);

	//Find minimum values
	find_minimum_x_y_z(points, point_count, &vmin);

	//Calculate the width and height
	vec3 differences(0, 0, 0);
	calculate_width_height(&vmin, &vmax, &differences);

	vec4 homogeneous_differences;
	homogeneous_differences.x = differences.x;
	homogeneous_differences.y = differences.y;
	homogeneous_differences.z = differences.z;
	homogeneous_differences.w = 1;
	//apply model_view

	//left-right translation amount
	GLfloat width_t_value = 0.10 * homogeneous_differences.x;
	//up-down translation amount
	GLfloat height_t_value = 0.10 * homogeneous_differences.y;
	//scaling factor
	GLfloat s_value = 0.10 * sqrt((homogeneous_differences.x * homogeneous_differences.x) + (homogeneous_differences.y * homogeneous_differences.y));
	//rotation (degrees)
	GLfloat r_value = 15.0;	

	key = tolower(key);
	int modifier = glutGetModifiers();

	switch (key) {
	case 'u':
		//translate up
		translate_m(0, height_t_value, 0, &translation_m);
		center.y += height_t_value;
		VIEWPORT_ORIGIN.y += height_t_value * DISPLAY_WIDTH;
		glViewport(VIEWPORT_ORIGIN.x, VIEWPORT_ORIGIN.y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
		break;
	case 'd':
		//down
		translate_m(0, -height_t_value, 0, &translation_m);
		center.y -= height_t_value;
		VIEWPORT_ORIGIN.y -= height_t_value * DISPLAY_WIDTH;
		glViewport(VIEWPORT_ORIGIN.x, VIEWPORT_ORIGIN.y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
		break;
	case 'l':
		//left
		translate_m(-width_t_value, 0, 0, &translation_m);
		center.x -= width_t_value;
		VIEWPORT_ORIGIN.x -= width_t_value * DISPLAY_WIDTH;
		glViewport(VIEWPORT_ORIGIN.x, VIEWPORT_ORIGIN.y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
		break;
	case 'r':
		//right
		translate_m(width_t_value, 0, 0, &translation_m);
		center.x += width_t_value;
		VIEWPORT_ORIGIN.x += width_t_value * DISPLAY_WIDTH;
		glViewport(VIEWPORT_ORIGIN.x, VIEWPORT_ORIGIN.y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
		break;
	case '+':
		//scale up 
		scale_m(new vec3(s_value, s_value, s_value), &scaling_m);
		break;
	case '-':
		//scale down
		scale_m(new vec3(-s_value, -s_value, -s_value), &scaling_m);
		break;
	case 'x':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			//positive x-axis rotation applied to rotation matrix
			rotate_x(r_value, &rotation_m);
		}
		else {
			//negative x-axis rotation
			rotate_x(-r_value, &rotation_m);
		}
		break;
	case 'y':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			//positiive y-axis rotation
			rotate_y(r_value, &rotation_m);
		}
		else {
			//negative y-axis rotation
			rotate_y(-r_value, &rotation_m);
		}
		break;
	case 'z':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			//positive z-axis rotation
			rotate_z(r_value, &rotation_m);
		}
		else {
			//negative z-axis rotation
			rotate_z(-r_value, &rotation_m);
		}
		break;
	case 'i':
		//center image at origin
		VIEWPORT_ORIGIN.x = 0;
		VIEWPORT_ORIGIN.y = 0;
		center.x -= translation_m[0][3];
		center.y -= translation_m[1][3];
		center.z -= translation_m[2][3];
		glViewport(VIEWPORT_ORIGIN.x, VIEWPORT_ORIGIN.y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
		rotation_m = mat4(1.0);
		scaling_m = mat4(1.0);
		translation_m = mat4(1.0);
		model_view = mat4(1.0);
		break;
	case 'q':
		cleanup();
		exit(0);
		break;
	}
	
	//setup translation to origin
	mat4 to_origin_translation(1.0);
	translate_m(-center.x, -center.y, -center.z, &to_origin_translation);

	//setup translation back to original location
	mat4 from_origin_translation(1.0);
	translate_m(center.x, center.y, center.z, &from_origin_translation);


	//set model_view to contain all transformations
	model_view = translation_m * from_origin_translation * rotation_m * scaling_m * to_origin_translation * model_view;
	rotation_m = mat4(1.0);
	scaling_m = mat4(1.0);
	translation_m = mat4(1.0);
	glutPostRedisplay();
}

void resize(int width, int height) {
	if (width > DISPLAY_WIDTH) {
		glOrtho(-1, (width / height), -1, 1, -2, 2);
	}
	else if (height > DISPLAY_HEIGHT) {
		glOrtho(-1, 1, -1, (height / width), -2, 2);
	}
	DISPLAY_WIDTH = width;
	DISPLAY_HEIGHT = height;
}

int main(int argc, char **argv) {
	ifstream inp = solicit_file("Please enter a filename.", "File not found.");
	string format = "%f %f %f";
	string jump_indicator = "J";

	/*
	Note: This vector should not be modified because it stores the original points.
	When doing morphing animation, this vector will store the destination points, so values
	should be copied to another container where they can be modified.
	*/
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
	find_largest_x_y_z(points, point_count, &vmax);

	//Find minimum values
	vec3 min(0, 0, 0);
	find_minimum_x_y_z(points, point_count, &vmin);

	//Normalize the vertices to -1.0 to 1.0
	normalize_vecs4(points, point_count, &vmax);

	//Normalize the min and max vectors to be used in calculating normalized width/height/center
	normalize_vec3(&vmin, &vmax);
	normalize_vec3(&vmax, &vmax);

	//Calculate the width and height
	vec3 differences(0, 0, 0);
	calculate_width_height(&vmin, &vmax, &differences);

	//Calculate center point
	center.w = 1.0;
	calculate_center(&vmin, &vmax, &center);

	VIEWPORT_ORIGIN = (0, 0);

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
