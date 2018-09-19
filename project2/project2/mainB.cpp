#include "stdafx.h"
#include "vertices.h"
#include "my_io.h"
#include "transformations.h"


int DISPLAY_WIDTH = 500;
int DISPLAY_HEIGHT = 400;
int VIEWPORT_WIDTH = DISPLAY_WIDTH;
int VIEWPORT_HEIGHT = DISPLAY_HEIGHT;
float ASPECT_RATIO = DISPLAY_WIDTH / DISPLAY_HEIGHT;

vec3 *destination_points;
vec3 *src_points;
vec3 *delta_slices;
GLint point_count;
vector<GLint> jumps;
const GLint slices = 2000;
GLint translations_performed = 0;

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
	glOrtho(-1.2, 1.2, 1.2, -1.2, -1.0, 1.0);
	glViewport(0, 0, VIEWPORT_HEIGHT, VIEWPORT_WIDTH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void draw_vertices(GLenum e, vec3 *vertices, int start, int end) {
	glBegin(e);
	for (int i = start; i < end; i++) {
		vec3 *v = &(vertices[i]);
		glColor3fv(src_points[i]);
		glVertex3f(v->x, v->y, v->z);
	}
	glEnd();
}

/*
Display function to be used in glutMainLoop().
*/
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	int last_jump = 0, current_jump = 0;
	glLineWidth(2.0);
	//Draw all vertices up to the last jump.
	int vertices_drawn = 0;
	for (int i = 0; i < jumps.size(); i++) {
		current_jump = jumps.at(i);
		//draw_vertices(GL_LINE_STRIP, destination_points, last_jump, current_jump); //draw destination points
		draw_vertices(GL_LINE_STRIP, src_points, last_jump, current_jump); //Draw source points
		vertices_drawn += (last_jump - current_jump);
		last_jump = (current_jump);
	}

	/*
	Because the previous loop is based on jumps, it will not draw points after the last jump.
	This is only an issue for files that are not terminated with a jump indicator.
	This if statement will determine if there are any remaining points to draw.
	*/
	if (vertices_drawn < point_count) {
		//draw_vertices(GL_LINE_STRIP, src_points, last_jump, point_count); //draw remaining source points
		draw_vertices(GL_LINE_STRIP, destination_points, last_jump, point_count); //draw remaining destination points
	}
	
	if (translations_performed < slices) {
		//Translate src points to destination points
		for (int i = 0; i < point_count; i++) {
			translate_to(&(src_points[i]), &(destination_points[i]), &(delta_slices[i]));
		}
		translations_performed += 1;
	}
	glFlush();
	glutPostRedisplay();
}

void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(60, (float)width/height, .1, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -4);
}

/*
Free allocated memory.
*/
void cleanup() {
	free(destination_points);
	free(delta_slices);
	free(src_points);
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
	destination_points = (vec3*)malloc(sizeof(vec3) * point_count);
	src_points = (vec3*)malloc(sizeof(vec3) * point_count);
	vec3 *deltas = (vec3*)malloc(sizeof(vec3) * point_count);
	delta_slices = (vec3*)malloc(sizeof(vec3) * point_count);

	if (destination_points == NULL || src_points == NULL || deltas == NULL || delta_slices == NULL) {
		cout << "Unable to allocate memory for points. Quitting...\n";
		return -2;
	}

	for (int i = 0; i < vec3_vector.size(); i++) {
		//Transfer vertices from the vector to vec3 array.
		destination_points[i] = *(vec3_vector.at(i));
	}

	//create and initialize min & max vectors
	vec3 min(0, 0, 0);
	find_minimum_x_y_z(destination_points, point_count, &min);
	vec3 max(0, 0, 0);
	find_largest_x_y_z(destination_points, point_count, &max);
	//Normalize the vertices to -1.0 to 1.0
	normalize_vecs3(destination_points, point_count, &max, &min);
	//Flip along x-axis
	flip_y(src_points, point_count);

	//Get random src points
	generate_random_normalized_vertices(src_points, point_count);
	//Calculate difference from src to destination points
	calculate_deltas(deltas, src_points, destination_points, point_count, slices);
	//Slice deltas
	for (int i = 0; i < point_count; i++) {
		slice_delta(&deltas[i], slices, &delta_slices[i]);
	}
	//Initialize translations performed
	translations_performed = 0;

	//Initialize windows and display image
	init_window(argc, argv);
	other_init();
	glewInit();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
	system("pause");
	cleanup();
	free(deltas);
	return 0;
}



