#include "my_io.h"
#include "obj_loader.h"
#include "draw.h"
#include <time.h>


/*
Levi Thieme Project 5
CS 321 Computer Graphics
Dr. Kim
*/

string filename;
int DISPLAY_WIDTH = 800;
int DISPLAY_HEIGHT = 800;
int VIEWPORT_WIDTH = 800;
int VIEWPORT_HEIGHT = 800;
float ASPECT_RATIO = DISPLAY_WIDTH / DISPLAY_HEIGHT;
//Square viewing-volume dimension
GLfloat view_volume_dimension = 15;
//Theta for rotation
static GLint theta = 0;

vector<vec4*> vertices;
vector<vec4*> normals;
vector<face*> faces;

//Light locations
GLfloat light_positional[] = { 2.0, 1.0, 2.0, 1.0 };
GLfloat light_positional2[] = { -2.0, 1.0, 2.0, 1.0 };
GLfloat light_directional[] = { -4, 2, 0, 0};

GLfloat directional_cutoff = 35;
GLfloat specular_component = 10;
GLfloat shininess = 0;

//Light colors
GLfloat r[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat g[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat b[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat w[] = { 1.0, 1.0, 1.0, 1.0 };

//Light colors
GLfloat ambient[] = { .5, .5, .5, 1 };
GLfloat diffuse[] = { .9, 0, .4, 1 };
GLfloat diffuse2[] = { 0, .7, .7, 1 };
GLfloat specular[] = { .9, .9, .9, 1 };
GLfloat emissive[] = { 0, 0, .8, 1 };

void cleanup();
void setViewVolumeAndCamera();
void updateLight(GLfloat *light, GLfloat modifier);

/*
Free allocated memory.
*/
void cleanup() {
	for (int i = 0; i < vertices.size(); i++) {
		delete(vertices.at(i));
	}
	vertices.clear();
	for (int i = 0; i < normals.size(); i++) {
		delete(normals.at(i));
	}
	normals.clear();
	for (int i = 0; i < faces.size(); i++) {
		face *face = faces[i];
		delete(face);
	}
	faces.clear();
}


/*
Initializes the window and its attributes.
*/
void init_window(int argc, char** argv) {
	glutInit(&argc, argv); //Initialize GLUT
	glutInitDisplayMode(GL_DOUBLE | GLUT_RGB | GLUT_DEPTH); //Specify display mode
	glutInitWindowSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Project 5 Levi Thieme");
}




/*
Sets up the clear color, viewing volume, and view port.
*/
void other_init() {
	glClearColor(1, 1, 1, 1);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light_directional);
	glPopMatrix();
	glPushMatrix();
	glLightfv(GL_LIGHT1, GL_POSITION, light_positional);
	glPopMatrix();
	glPushMatrix();
	glLightfv(GL_LIGHT2, GL_POSITION, light_positional2);
	glPopMatrix();
	
	//Directional Light setup
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	//Positional Light 1 setup
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, &directional_cutoff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, &specular_component);
	glLightfv(GL_LIGHT1, GL_EMISSION, emissive);

	//Positional Light 2 setup
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
	glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, &directional_cutoff);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT2, GL_SPOT_EXPONENT, &specular_component);


	setViewVolumeAndCamera();
}

/*
Sets the viewing volume and camera location based on input file.
*/
void setViewVolumeAndCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (filename == "cube.obj") {
		glFrustum(-2, 2, -2, 2, 2, 4);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 4, 0, 0, 0, 0, 1, 0);
	}
	else if (filename == "teapot.obj") {
		glFrustum(-8, 8, -8, 8, 10, 50);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 40, 0, 0, 0, 0, 1, 0);
	}
	else if (filename == "lamp.obj") {
		glFrustum(-7, 7, -7, 7, 7, 15);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
	}
	else if (filename == "gangster.obj") {
		glFrustum(-1.3, 1.3, -1.3, 1.3, 1, 10);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 4, 0, 0, 0, 0, 1, 0);
	}
	else if (filename == "alfaRomeo147.obj") {
		glFrustum(-25, 25, -25, 25, 20, 100);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 115, 0, 0, 0, 0, 1, 0);
	}
}

/*
Display callback function for glutMainLoop().
*/
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawObject(vertices, normals, faces, shininess);
	glFlush();
	glutSwapBuffers();
}




/*
Keyboard callback function.
*/
void keyboard(unsigned char key, int x, int y) {
	
	key = tolower(key);
	int modifier = glutGetModifiers();
	GLfloat r = 15;
	GLfloat t = .2;
	GLfloat s = .2;

	switch (key) {

	case '0':
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		break;
	case '1':
		glEnable(GL_LIGHT0);
		break;
	case '2':
		glEnable(GL_LIGHT1);
		break;
	case '3':
		glEnable(GL_LIGHT2);
		break;
	case '+':
		directional_cutoff += 5;
		if (directional_cutoff > 90) {
			directional_cutoff = 180;
		}
		glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, &directional_cutoff);
		break;
	case '-':
		if (directional_cutoff == 180) {
			directional_cutoff = 90;
		}
		else {
			directional_cutoff -= 5;
			if (directional_cutoff < 0) {
				directional_cutoff = 0;
			}
		}
		glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, &directional_cutoff);
		glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, &directional_cutoff);
		break;
	case 's':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			specular_component += 1;
			if (specular_component > 128) {
				specular_component = 128;
			}
		}
		else {
			specular_component -= 1;
			if (specular_component < 0) {
				specular_component = 0;
			}
		}
		glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, &specular_component);
		glLightfv(GL_LIGHT2, GL_SPOT_EXPONENT, &specular_component);
		break;
	case 'x':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			glRotatef(-r, 1.0, 0, 0);
		}
		else {
			glRotatef(r, 1.0, 0, 0);
		}
		break;
	case 'o':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			glRotatef(-r, 0, 1, 0);
		}
		else {
			glRotatef(r, 0, 1, 0);
		}
		break;
	case 'z':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			glRotatef(-r, 0, 0, 1);
		}
		else {
			glRotatef(r, 0, 0, 1);
		}
		break;
	case 'p':
		glPushMatrix();
		//translate to origin
		glTranslatef(-light_positional[0], -light_positional[1], -light_positional[2]);
		theta += 10;
		glRotated(theta, 0, 1, 0);
		if (theta >= 360) {
			theta = 0;
		}
		glTranslatef(light_positional[0], light_positional[1], light_positional[2]);
		glLightfv(GL_LIGHT1, GL_POSITION, light_positional);
		glPopMatrix();
		
		glPushMatrix();
		//translate to origin
		glTranslatef(-light_positional2[0], -light_positional2[1], -light_positional2[2]);
		theta += 10;
		glRotated(theta, 0, 1, 0);
		if (theta >= 360) {
			theta = 0;
		}
		glTranslatef(light_positional2[0], light_positional2[1], light_positional2[2]);
		glLightfv(GL_LIGHT2, GL_POSITION, light_positional);
		glPopMatrix();
		break;
	case 'a':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			updateLight(ambient, 0.1);
		}
		else {
			updateLight(ambient, -0.1);
		}
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		break;
	case 'd':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			updateLight(diffuse, 0.1);
			updateLight(diffuse2, 0.1);
		}
		else {
			updateLight(diffuse, -0.1);
			updateLight(diffuse2, -0.1);
		}
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
		break;
	case 'e':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			updateLight(emissive, .1);
		}
		else {
			updateLight(emissive, -.1);
		}
		glLightfv(GL_LIGHT1, GL_EMISSION, emissive);
		break;
	case 'h':
		if (modifier == GLUT_ACTIVE_SHIFT) {
			shininess += 8;
			if (shininess > 128) {
				shininess = 128;
			}
		}
		else {
			shininess -= 8;
			if (shininess < 0) {
				shininess = 0;
			}
		}
		break;
	case 'q':
		cleanup();
		exit(0);
		break;
	}
	glutPostRedisplay();
}

/*
Updates an r,g,b lighting array by adding modifier.
*/
void updateLight(GLfloat *light, GLfloat modifier) {
	light[0] += modifier;
	light[1] += modifier;
	light[2] += modifier;
}

/*
Resize callback function.
*/
void resize(int width, int height) {
	DISPLAY_WIDTH = width;
	DISPLAY_HEIGHT = height;
}

void set(GLfloat *p, GLfloat r, GLfloat g, GLfloat b);

int main(int argc, char **argv) {

	ifstream file = solicit_file("./objects/", "Please enter an object model filename", "Unable to find the specified file.", filename);
	if (file.bad()) {
		return -1;
	} 
	cout << "Reading object file...\n";
	read_model_object(file, vertices, normals, faces);
	file.close();

	cout << "Vertices: " << vertices.size() << "  Normals: " << normals.size() << "  Faces: " << faces.size() << "\n";

	vec4 min;
	vec4 max;
	findMaxMin(vertices, min, max);

	

	//Initialize windows and display image
	init_window(argc, argv);
	other_init();
	glewInit();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMainLoop();
	cleanup();

	system("pause");
	return 0;
}
