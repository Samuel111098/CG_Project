//
//  main.cpp
//  triangle
//
//  Created by Samuel Ravella on 24/04/19.
//  Copyright © 2019 Samuel Ravella. All rights reserved.
//

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

/* Handler for window-repaint event. Called back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    
    // Render a pyramid consists of 4 triangles
    glLoadIdentity();                  // Reset the model-view matrix
    glTranslatef(0.0f, 0.0f, -7.0f);// Move left and into the screen
    glRotatef(90, 1, 0, 0);
    
    glBegin(GL_POLYGON);           // Begin drawing the pyramid with 4 triangles
    // Front
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(-1.5f, -1.0f, 0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(1.5f, -1.0f, 0.5f);
    
    // Right
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(1.5f, -1.0f, 0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(1.5f, -1.0f, -0.5f);
    
    // Back
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(1.5f, -1.0f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-1.5f, -1.0f, -0.5f);
    
    // Left
    glColor3f(1.0f,0.0f,0.0f);       // Red
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glColor3f(0.0f,0.0f,1.0f);       // Blue
    glVertex3f(-1.5f,-1.0f,-0.5f);
    glColor3f(0.0f,1.0f,0.0f);       // Green
    glVertex3f(-1.5f,-1.0f, 0.5f);
    glEnd();   // Done drawing the pyramid
    
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

/* Handler for window re-size event. Called back when the window first appears and
 whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    
    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
    
    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(75.0f, aspect, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW); // To operate on model-view matrix
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(640, 480);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow("Paraglide");          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    initGL();                       // Our own OpenGL initialization
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}
