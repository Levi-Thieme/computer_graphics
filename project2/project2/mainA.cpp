#include "stdafx.h"
#include "vertices.h"
#include "my_io.h"
#include "transformations.h"


int DISPLAY_WIDTH = 500;
int DISPLAY_HEIGHT = 400;
int VIEWPORT_WIDTH = DISPLAY_WIDTH;
int VIEWPORT_HEIGHT = DISPLAY_HEIGHT;
float ASPECT_RATIO = DISPLAY_WIDTH / DISPLAY_HEIGHT;

vec3 *points;
int point_count;
vector<int> jumps;

/*
Initializes the window and its attributes.
*/
void init_window(int argc, char** argv) {
	glutInit(&argc, argv); //Initialize GLUT
	glutInitDisplayMode(GLUT_RGB); //Specify display mode
	glutInitWindowSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	glutInitWindowPosition(500, 400);
	glutCreateWindow("Project 2 Levi Thieme");
}

/*
Sets up the clear color, viewing volume, and view port.
*/
void other_init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.2, 1.2, -1.2, 1.2, -1.0, 1.0);
	glViewport(0, 0, VIEWPORT_HEIGHT, VIEWPORT_WIDTH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void draw_vertices(GLenum e, vec3 *vertices, int start, int end) {
	glBegin(e);
	for (int i = start; i < end; i++) {
		//glColor3fv(vertices[i]);
		glColor3f(0, .7, .7);
		vec3 *v = &(vertices[i]);
		glVertex3f(v->x, v->y, v->z);
	}
	glEnd();
}

/*
Display function to be used in glutMainLoop().
*/
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(1.0);
	int last_jump = 0, current_jump = 0;
	//Draw all vertices up to the last jump.
	int vertices_drawn = 0;


	for (int i = 0; i < jumps.size(); i++) {
		current_jump = jumps.at(i);
		//draw
		draw_vertices(GL_LINE_STRIP, points, last_jump, current_jump); //draw src_points
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
		draw_vertices(GL_LINE_STRIP, points, last_jump, point_count); //draw remaining source points
	}

	glFlush();
}

/*
Free allocated memory.
*/
void cleanup() {
	free(points);
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
	vector<vec3 *> vec3_vector;

	point_count = read_vecs3(&vec3_vector, &jumps, &inp, &format, &jump_indicator); //Load the vertices and jumps from data file
	if (point_count < 1) {
		cout << "Unable to read any points from data file. Quitting...\n";
		system("pause");
		return -1;
	}

	//allocate points array to store modifiable vertices.
	points = (vec3*)malloc(sizeof(vec3) * vec3_vector.size());

	if (points == NULL) {
		cout << "Unable to allocate memory for points. Quitting...\n";
		system("pause");
		return -2;
	}

	for (int i = 0; i < vec3_vector.size(); i++) {
		//Transfer vertices from the vector to vec3 array.
		points[i] = *(vec3_vector.at(i));
	}

	//Find absolute largest values for normalization from -1.0 to 1.0
	vec3 max(0, 0, 0);
	find_largest_x_y_z(points, point_count, &max);


	//Normalize the vertices to -1.0 to 1.0
	normalize_vecs3(points, point_count, &max);

	//Initialize windows and display image
	init_window(argc, argv);
	other_init();
	glewInit();
	glutDisplayFunc(display);
	glutMainLoop();
	system("pause");
	cleanup();
	return 0;
}
