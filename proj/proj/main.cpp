//
//  main.cpp
//  proj
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
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
float sealevel; float polysize;

int seed(float x, float y) {
    static int a = 1588635695, b = 1117695901;
    int xi = *(int *)&x;
    int yi = *(int *)&y;
    return ((xi * a) % b) - ((yi * b) % a);
}

void mountain(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float s)
{
    float x01,y01,z01,x12,y12,z12,x20,y20,z20;
    
    glShadeModel(GL_SMOOTH);
    if (s < (polysize)) {
        x01 = x1 - x0;
        y01 = y1 - y0;
        z01 = z1 - z0;
        
        x12 = x2 - x1;
        y12 = y2 - y1;
        z12 = z2 - z1;
        
        x20 = x0 - x2;
        y20 = y0 - y2;
        z20 = z0 - z2;
        
        float nx = y01*(-z20) - (-y20)*z01;
        float ny = z01*(-x20) - (-z20)*x01;
        float nz = x01*(-y20) - (-x20)*y01;
        
        float den = sqrt(nx*nx + ny*ny + nz*nz);
        
        if (den > 0.0) {
            nx /= den;
            ny /= den;
            nz /= den;
        }
        
        glNormal3f(nx,ny,nz);
        glBegin(GL_TRIANGLES);
        glVertex3f(x0,y0,z0);
        glVertex3f(x1,y1,z1);
        glVertex3f(x2,y2,z2);
        glEnd();
        
        return;
    }
    
    x01 = 0.5*(x0 + x1);
    y01 = 0.5*(y0 + y1);
    z01 = 0.5*(z0 + z1);
    
    x12 = 0.5*(x1 + x2);
    y12 = 0.5*(y1 + y2);
    z12 = 0.5*(z1 + z2);
    
    x20 = 0.5*(x2 + x0);
    y20 = 0.5*(y2 + y0);
    z20 = 0.5*(z2 + z0);
    
    s *= 0.5;
    
    srand(seed(x01,y01));
    z01 += 0.3*s*(3.0*((float)rand()/(float)RAND_MAX) - 1.0);
    srand(seed(x12,y12));
    z12 += 0.3*s*(3.0*((float)rand()/(float)RAND_MAX) - 1.0);
    srand(seed(x20,y20));
    z20 += 0.3*s*(3.0*((float)rand()/(float)RAND_MAX) - 1.0);
    
    mountain(x0,y0,z0,x01,y01,z01,x20,y20,z20,s);
    mountain(x1,y1,z1,x12,y12,z12,x01,y01,z01,s);
    mountain(x2,y2,z2,x20,y20,z20,x12,y12,z12,s);
    mountain(x01,y01,z01,x12,y12,z12,x20,y20,z20,s);
    
}


void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.8);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    gluLookAt(0.0,0.0,0.0,
              1.0,0.0,0.0,
              0.0,0.0,1.0);
    sealevel = -0.01;
    polysize = 0.01;
}
//Simply calls the general gl reshaping functions, making sure things remain as they should
void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(75.0,(double)w / (double)h,0.0001,10.0);
    glMatrixMode (GL_MODELVIEW);
}

class Vect
{
public:
    double x;
    double y;
    double z;

    Vect() {}
    Vect(double firstDim, double secDim, double thirdDim)
    {
        x = firstDim;
        y = secDim;
        z = thirdDim;
    }
    
    //A very basic function to add two vectors together, and return the result
    Vect operator+(Vect temp)
    {
        double xNew = x + temp.x;
        double yNew = y + temp.y;
        double zNew = z + temp.z;
        Vect newVect = Vect(xNew, yNew, zNew);
        return newVect;
    }
    
    void normalizeVect()
    {
        double xSq = x*x;
        double ySq = y*y;
        double zSq = z*z;
        double totalMagSq = xSq + ySq + zSq;
        if (fabs(totalMagSq) > 0.00001)
        {
            if(fabs(totalMagSq - 1.0) > 0.00001)
            {
                double newMag = sqrt(totalMagSq);
                x = x / newMag;
                y = y / newMag;
                z = z / newMag;
            }
        }
    }
};

class ComplexVect
{
public:
    double w;
    double x;
    double y;
    double z;
    
    ComplexVect() {}
    
    ComplexVect(double tbuW, double tbuX, double tbuY, double tbuZ)
    {
        w = tbuW;
        x = tbuX;
        y = tbuY;
        z = tbuZ;
    }

    ComplexVect operator*(const ComplexVect &otherVect) const
    {
        double wW = w*otherVect.w;
        double xX = x*otherVect.x;
        double yY = y*otherVect.y;
        double zZ = z*otherVect.z;
        double newW = wW - xX - yY - zZ;
        
        
        double wX = w*otherVect.x;
        double xW = x*otherVect.w;
        double yZ = y*otherVect.z;
        double zY = z*otherVect.y;
        double newX = wX + xW + yZ - zY;
        
        
        double wY = w*otherVect.y;
        double yW = y*otherVect.w;
        double zX = z*otherVect.x;
        double xZ = x*otherVect.z;
        double newY = wY + yW + zX - xZ;
        
        
        double wZ = w*otherVect.z;
        double zW = z*otherVect.w;
        double xY = x*otherVect.y;
        double yX = y*otherVect.x;
        double newZ = wZ + zW + xY - yX;
        
        ComplexVect newVect = ComplexVect(newW, newX, newY, newZ);
        
        return newVect;
    }
    
    //Normalize the vectors in the interest of safe rotations
    void normalizeCompVect()
    {
        double newW = w*w;
        double newX = x*x;
        double newY = y*y;
        double newZ = z*z;
        double newTotMagSq = newW + newX + newZ + newY;
        if (fabs(newTotMagSq) > 0.00001)
        {
            if(fabs(newTotMagSq - 1.0) > 0.00001)
            {
                double newMag = sqrt(newTotMagSq);
                w = w/newMag;
                x = x/newMag;
                y = y/newMag;
                z = z/newMag;
            }
        }
    }

    Vect operator*(const Vect &normVec) const
    {
        Vect normVecCopy = Vect(normVec.x,normVec.y,normVec.z);
        normVecCopy.normalizeVect();
        
        ComplexVect complexVect1 = ComplexVect(0.0, normVecCopy.x, normVecCopy.y, normVecCopy.z);
        ComplexVect complexVect2 = ComplexVect();
        
        complexVect2 = *this * (complexVect1 * ComplexVect(w, -x, -y, -z));
        return Vect(complexVect2.x, complexVect2.y, complexVect2.z);
    }
};

/*
 * Plane class is used for tracking locations and directions
 * without rotations systems problems kicking in (hopefully, wrote this descriprion early in the process)
 */
class Plane
{
public:
    //A vector used to handle rotation
    ComplexVect rotationVect;
    //A vector to handle more simple things, position here
    Vect posVect;
    double planeSpeed;
    
    Plane() {
        //Defaulted to .1 on y
        posVect = Vect(-0.5,0.1,0.0);
        planeSpeed = 0.003;
        rotationVect = ComplexVect(1.0,0.0,0.0,0.0);
        rotationVect.normalizeCompVect();
    }
    
    void moveForward(double distance)
    {
        Vect newVect = Vect(distance, 0.0, 0.0);
        posVect = posVect + rotationVect*newVect;
    }
    
    void moveForwardHelper(double time)
    {
        moveForward(planeSpeed * time);
    }

    void rollTide(double rollFactor)
    {
        Vect basicVect = Vect(1,0,0);
        Vect vect = rotationVect * basicVect;
        ComplexVect rotVect = ComplexVect(rollFactor, vect.x, vect.y, vect.z);
        rotVect.normalizeCompVect();
        rotationVect = rotVect * rotationVect;
        rotationVect.normalizeCompVect();
    }
    
    void yawPlane(double yawFactor)
    {
        rotationVect = ComplexVect(yawFactor, 0, 1, 0) * rotationVect;
        rotationVect.normalizeCompVect();
    }

    void pitchPlane(double pitchFactor)
    {
        Vect basicVect = Vect(0, 0, 1);
        Vect vect = rotationVect * basicVect;
        ComplexVect rotVect(pitchFactor, vect.x, vect.y, vect.z);
        rotVect.normalizeCompVect();
        rotationVect = rotVect * rotationVect;
        rotationVect.normalizeCompVect();
    }
};


Plane plane;

double planeArr[3] = {1, 0, 0};
double planeUpArray[3]  = {0, 0, 1};

void updateVectors() {
    plane.moveForwardHelper(0.07);
    
    Vect basicUpVect = Vect(0, 1, 0);
    Vect vec = plane.rotationVect * basicUpVect;
    planeUpArray[0] = vec.x;
    planeUpArray[1] = vec.z;
    planeUpArray[2] = vec.y;
    
    Vect basicForwVect = Vect(1, 0, 0);
    Vect vect = plane.rotationVect * basicForwVect;
    planeArr[0] = vect.x;
    planeArr[1] = vect.z;
    planeArr[2] = vect.y;
}

// lighting of the map

void setUpLights(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat white[] = {1.5, 1.5, 1.5, 1.5};
    GLfloat lightPos[] = {5, 5, 5, 0};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
}

//It sets the position of the function to look at the position of the Plane object, then calls lights

void display(void)
{
    glLoadIdentity ();
    updateVectors();
    gluLookAt (plane.posVect.x,plane.posVect.z,plane.posVect.y, plane.posVect.x + planeArr[0], plane.posVect.z + planeArr[1], plane.posVect.y + planeArr[2], planeUpArray [0],planeUpArray [1],planeUpArray [2]);
    setUpLights();
    
    GLfloat tanamb[] = {0.2,0.15,0.1,1.0};
    GLfloat tandiff[] = {0.4,0.3,0.2,1.0};
    
    GLfloat seaamb[] = {0.0,0.1,0.2,1.0};
    GLfloat seadiff[] = {0.2,0.6,0.8,1.0};
    GLfloat seaspec[] = {0.5,0.7,1.0,1.0};
    
    GLfloat paraamb[] = {0.4,0.4,0.3,1.0};
    GLfloat paradiff[] = {0.3,0.3,0.4,1.0};
    
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);
    glTranslatef (-0.5, -0.5, 0.0);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tanamb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tandiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tandiff);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);
    
    mountain(0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0);
    mountain(1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, seaamb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, seadiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, seaspec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);
    
    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_QUADS);
    glVertex3f(0.0,0.0,sealevel);
    glVertex3f(1.0,0.0,sealevel);
    glVertex3f(1.0,1.0,sealevel);
    glVertex3f(0.0,1.0,sealevel);
    glEnd();
    
    glLoadIdentity();                  // Reset the model-view matrix
    glTranslatef(0.0f, 0.0f, -3.0f);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2,0.2,0.2);
    setUpLights();
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, paraamb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, paradiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, paradiff);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);
    
    glNormal3f(0.0,1.0,0.0);
    glBegin(GL_TRIANGLES);           // Begin drawing the paraglide with 4 triangles
    // Front
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glVertex3f(-1.5f, -1.0f, 0.3f);
    glVertex3f(1.5f, -1.0f, 0.3f);
    // Right
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.5f, -1.0f, 0.3f);
    glVertex3f(1.5f, -1.0f, -0.3f);
    // Back
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.5f, -1.0f, -0.3f);
    glVertex3f(-1.5f, -1.0f, -0.3f);
    // Left
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glVertex3f(-1.5f,-1.0f,-0.3f);
    glVertex3f(-1.5f,-1.0f, 0.3f);
    glEnd();   // Done drawing the paraglide
    
    glutSwapBuffers();
    glFlush ();
    
    glutPostRedisplay();
}


void keyInputHandler(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) { plane.rollTide(25); }
    else if(key == GLUT_KEY_RIGHT) { plane.rollTide(-25); }
    else if(key == GLUT_KEY_UP) { plane.pitchPlane(-25); }
    else if(key == GLUT_KEY_DOWN) { plane.pitchPlane(25); }
}

void keyboard(unsigned char key, int x, int y)
{
    //below is the given key inputs
    
    if(key == '-') { sealevel -= 0.01; }
    else if(key == '=') { sealevel += 0.01; }
    else if(key == 'f') { polysize /= 2.0; }
    else if(key == 'c') { polysize *= 2.0; }
    
    if(key == 'w')
    { plane.planeSpeed += 0.01; }
    else if(key == 's')
    { plane.planeSpeed -= 0.01; }
    else if(key == 'd')
    { plane.yawPlane(35); }
    else if(key == 'a')
    { plane.yawPlane(-35); }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyInputHandler);
    glutMainLoop();
    return 0;
}
