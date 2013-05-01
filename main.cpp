#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
//In your projcts properties (ab Build targets)), chose Console as app-type and check "Pause when execution ends" or check "Project -> Set program's arguments -> Run host in terminal" .
bool* keyStates = new bool[256]; // Create an array of boolean values of length 256 (0-255)

int amount; // The amount of rotation for each arrow press

typedef glm::mat3 mat3 ;
typedef glm::mat4 mat4 ; 
typedef glm::vec3 vec3 ; 
typedef glm::vec4 vec4 ; 
const float pi = 3.14159265 ; // For portability across platforms

vec3 eye; // The (regularly updated) vector coordinates of the eye location 
vec3 up;  // The (regularly updated) vector coordinates of the up location 
const vec3 eyeinit(0.0,0.0,5.0); // Initial eye position, also for resets
const vec3 upinit(0.0,1.0,0.0); // Initial up position, also for resets
const int amountinit = 5; //Initial step amount for camera movement, also for resets

GLfloat redDiffuseMaterial[] = {1.0, 0.0, 0.0}; //set the material to red
GLfloat whiteSpecularMaterial[] = {1.0, 1.0, 1.0}; //set the material to white
GLfloat greenEmissiveMaterial[] = {0.0, 1.0, 0.0}; //set the material to green
GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; //set the light specular to white
GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0}; //set the light ambient to black
GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0}; //set the diffuse light to white
GLfloat blankMaterial[] = {0.0, 0.0, 0.0}; //set the diffuse light to white
GLfloat mShininess[] = {128}; //set the shininess of the material

bool diffuse = false;
bool emissive = false;
bool specular = false;

mat3 rotate(const float degrees, const vec3& axis) {
    float radians = degrees * (pi / 180);
    mat3 myMatrix1 = mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
    mat3 myMatrix2 = mat3(axis.x * axis.x, axis.x * axis.y, axis.x * axis.z, axis.x * axis.y, axis.y * axis.y, axis.y * axis.z, axis.x * axis.z, axis.y * axis.z, axis.z * axis.z);
    mat3 myMatrix3 = mat3(0.0, axis.z, -1 * axis.y, -1 * axis.z, 0.0, axis.x, axis.y, -1 * axis.x, 0.0);
    mat3 rotation = cos(radians) * myMatrix1 + (1 - cos(radians)) * myMatrix2 + sin(radians) * myMatrix3;
    return rotation;
}

// Transforms the camera left around the "crystal ball" interface

void left(float degrees, vec3& eye, vec3& up) {
    printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));
    eye = rotate(degrees, up)*eye;
}

// Transforms the camera up around the "crystal ball" interface

void fup(float degrees, vec3& eye, vec3& up) {
    vec3 ortho_axis = glm::cross(eye, up);
    ortho_axis = glm::normalize(ortho_axis);

    eye = rotate(degrees, ortho_axis)*eye;
    up = rotate(degrees, ortho_axis)*up;
}

void keyOperations (void) {
    if (keyStates['a']) {

    }
}

void renderPrimitive (void) {
    glBegin(GL_QUADS); // Start drawing a quad primitive

    glColor3f(0.2f, 0.4f, 0.2f);
    glVertex3f(-1.0f, -1.0f, 0.0f); // The bottom left corner
    glColor3f(1.0f, 0.4f, 0.2f);
    glVertex3f(-1.0f, 1.0f, 0.0f); // The top left corner
    glColor3f(0.0f, 0.0f, 0.2f);
    glVertex3f(1.0f, 1.0f, 0.0f); // The top right corner
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f); // The bottom right corner

    glEnd();
}

void renderCube (void) {
    float angle = 35.0f;
    //glScalef( 2.0, 0.5, 1.0 );  //twice as wide, half the height,
    glRotatef(angle, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glColor4f(1.0, 0.0, 0.0, 0.1); //set the color and alpha of the cube
    //glutSolidCube(1);
    glColor4f(0.0, 1.0, 0.0, 1.0); //set the color and alpha of the cube
    glutSolidCube(.5);
}

void init (void) {
    glEnable(GL_DEPTH_TEST); //enables depth testing
    glEnable(GL_LIGHTING); //enable OpenGL lighting support
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL); //enables color!
    
    	// Set up initial position for eye,up and amount
	// As well as booleans 

	eye = eyeinit; 
	up = upinit; 
	amount = amountinit;

	glEnable(GL_DEPTH_TEST);
}

void light (void) {
    //Now I am just assigning the colours to the current light
    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
}

void display (void) {
    keyOperations();
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Clear the background of our window to red
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    
    	glMatrixMode(GL_MODELVIEW);
	mat4 mv; 
	const vec3 center(0.0,0.0,0.0); 

	mv = glm::lookAt(eye,center,up); 
	glLoadMatrixf(&mv[0][0]); 
    
    glEnable(GL_BLEND); //enable the blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //set the blend function

    //glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    light();
    //gluLookAt (0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //glTranslatef(0.0f, 0.0f, -5.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive

    //renderPrimitive();
    //renderCube();

    //glTranslatef(0.0f, yLocation, 0.0f); // Translate our object along the y axis
    //glRotatef(yRotationAngle, 0.0f, 0.0f, 0.0f); // Rotate our object around the y axis
    glColor3f(0.2f, 0.4f, 0.2f);
    glutSolidTeapot(1.0f);

    glFlush(); // Flush the OpenGL buffers to the window
    //glutSwapBuffers(); //Used for double buffers
}

void reshape (int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window
    glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed
    glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
    glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly
}

void specialKey(int key,int x,int y) {
	switch(key) {
		case 100: //left
			left(amount,eye,up);
			break;
		case 101: //up
			fup(-amount,eye,up);
			break;
		case 102: //right
			left(-amount,eye,up);
			break;
		case 103: //down
			fup(amount,eye,up);
			break;
	}
	glutPostRedisplay();
}

void keyPressed (unsigned char key, int x, int y) {
    keyStates[key] = true; // Set the state of the current key to pressed
}

void keyUp (unsigned char key, int x, int y) {
    keyStates[key] = false; // Set the state of the current key to not pressed
}

void keyboard (unsigned char key, int x, int y) {
    if (key == 's') {
        if (specular == false) {
            specular = true;
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
        } else if (specular == true) {
            specular = false;
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blankMaterial);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, blankMaterial);
        }
    }

    if (key == 'd') {
        if (diffuse == false) {
            diffuse = true;
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redDiffuseMaterial);
        } else if (diffuse == true) {
            diffuse = false;
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blankMaterial);
        }
    }

    if (key == 'e') {
        if (emissive == false) {
            emissive = true;
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, greenEmissiveMaterial);
        } else if (emissive == true) {
            emissive = false;
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blankMaterial);
        }
    }
}

int main(int argc, char **argv) {
    for(unsigned int i = 0; i < 256; i++) {
        keyStates[i] = false;
    }

    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); // Set up a basic display buffer (only single buffered for now)
    glutInitWindowSize(500, 500); // Set the width and height of the window
    glutInitWindowPosition(100, 100); // Set the position of the window
    glutCreateWindow("Your first OpenGL Window"); // Set the title for the window
    init();
    glutDisplayFunc(display); //we want to tell GLUT what method will store our code for drawing.
    glutIdleFunc(display);
    glutReshapeFunc(reshape); // Tell GLUT to use the method "reshape" for rendering
    glutSpecialFunc(specialKey);
    glutKeyboardFunc(keyboard); // Tell GLUT to use the method "keyPressed" for key presses
    glutKeyboardUpFunc(keyUp); // Tell GLUT to use the method "keyUp" for key up events

    glutMainLoop(); //Now that GLUT knows what method to use for drawing, all we have to do is tell GLUT to enter its Main loop.
}
