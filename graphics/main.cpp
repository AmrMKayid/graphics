/* Libraries */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <windows.h>
#include <mmsystem.h>
#endif

using namespace std;

// -----------------------------------
//              Constant
// -----------------------------------

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
#define WINDOW_WIDTH 1700
#define WINDOW_HEIGHT 1000


// -----------------------------------
//          Methods Signatures
// -----------------------------------

void Display();

void Keyboard(unsigned char key, int x, int y);
void Special(int key, int x, int y);

void drawWall(double thickness);
void drawTableLeg(double thick, double len);
void drawJackPart();
void drawJack();
void drawTable(double topWid, double topThick, double legThick, double legLen);
void drawSnowMan();

void bed();
void mirror();
void closet();

void setupLights();
void setupCamera();
void topCam();
void defaultCam();
void roomsWalls();



// -----------------------------------
//              Classes
// -----------------------------------

class Vector3f {
public:
    float x, y, z;

    Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f operator+(Vector3f const &v) {
        return Vector3f(x + v.x, y + v.y, z + v.z);
    }

    Vector3f operator-(Vector3f const &v) {
        return Vector3f(x - v.x, y - v.y, z - v.z);
    }

    Vector3f operator*(float n) {
        return Vector3f(x * n, y * n, z * n);
    }

    Vector3f operator/(float n) {
        return Vector3f(x / n, y / n, z / n);
    }

    Vector3f unit() {
        return *this / sqrt(x * x + y * y + z * z);
    }

    Vector3f cross(Vector3f v) {
        return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
};

class Camera {
public:
    Vector3f eye, center, up;

    Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
        eye = Vector3f(eyeX, eyeY, eyeZ);
        center = Vector3f(centerX, centerY, centerZ);
        up = Vector3f(upX, upY, upZ);
    }

    void moveX(float d) {
        Vector3f right = up.cross(center - eye).unit();
        eye = eye + right * d;
        center = center + right * d;
    }

    void moveY(float d) {
        eye = eye + up.unit() * d;
        center = center + up.unit() * d;
    }

    void moveZ(float d) {
        Vector3f view = (center - eye).unit();
        eye = eye + view * d;
        center = center + view * d;
    }

    void rotateX(float a) {
        Vector3f view = (center - eye).unit();
        Vector3f right = up.cross(view).unit();
        view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
        up = view.cross(right);
        center = eye + view;
    }

    void rotateY(float a) {
        Vector3f view = (center - eye).unit();
        Vector3f right = up.cross(view).unit();
        view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
        right = view.cross(up);
        center = eye + view;
    }

    void look() {
        gluLookAt(
            eye.x, eye.y, eye.z,
            center.x, center.y, center.z,
            up.x, up.y, up.z
        );
    }
};

// -----------------------------------
//          Global Variables
// -----------------------------------

Camera camera;

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Assignment 2");
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(Special);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glShadeModel(GL_SMOOTH);

    glutMainLoop();
}

void Display() {
    setupCamera();
    setupLights();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    drawSnowMan();
    
    // LEFT Room (Bed Room)
    bed();
    mirror();
    closet();
    
    
    // Rooms walls
    roomsWalls();

//    printf("EyeX: %f, EyeY: %f, EyeZ: %f | cX: %f, cY: %f, cZ: %f | uX: %f, uY: %f, uZ: %f \n\n", camera.eye.x, camera.eye.y, camera.eye.z, camera.center.x, camera.center.y, camera.center.z, camera.up.x, camera.up.y, camera.up.z );
    
    glFlush();
}

// -----------------------------------
//          Keyboard Methods
// -----------------------------------

void Keyboard(unsigned char key, int x, int y) {
    float d = 0.1;

    switch (key) {
    case 'w':
        camera.moveY(d);
        break;
    case 's':
        camera.moveY(-d);
        break;
    case 'a':
        camera.moveX(d);
        break;
    case 'd':
        camera.moveX(-d);
        break;
    case 'q':
        camera.moveZ(d);
        break;
    case 'e':
        camera.moveZ(-d);
        break;
    case 't':
            topCam();
            break;
    case 'r':
            defaultCam();
            break;

    case GLUT_KEY_ESCAPE:
        exit(EXIT_SUCCESS);
    }

    glutPostRedisplay();
}

void Special(int key, int x, int y) {
    float a = 5.0;

    switch (key) {
    case GLUT_KEY_UP:
        camera.rotateX(a);
        break;
    case GLUT_KEY_DOWN:
        camera.rotateX(-a);
        break;
    case GLUT_KEY_LEFT:
        camera.rotateY(a);
        break;
    case GLUT_KEY_RIGHT:
        camera.rotateY(-a);
        break;
    }

    glutPostRedisplay();
}

// -----------------------------------
//          Drawing Methods
// -----------------------------------

void drawWall(double thickness) {
    glPushMatrix();
    glTranslated(0.5, 0.5 * thickness, 0.5);
    glScaled(1.0, thickness, 1.0);
    glutSolidCube(1);
    glPopMatrix();
}
void drawTableLeg(double thick, double len) {
    glPushMatrix();
    glTranslated(0, len / 2, 0);
    glScaled(thick, len, thick);
    glutSolidCube(1.0);
    glPopMatrix();
}
void drawJackPart() {
    glPushMatrix();
    glScaled(0.2, 0.2, 1.0);
    glutSolidSphere(1, 15, 15);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0, 0, 1.2);
    glutSolidSphere(0.2, 15, 15);
    glTranslated(0, 0, -2.4);
    glutSolidSphere(0.2, 15, 15);
    glPopMatrix();
}
void drawJack() {
    glPushMatrix();
    drawJackPart();
    glRotated(90.0, 0, 1, 0);
    drawJackPart();
    glRotated(90.0, 1, 0, 0);
    drawJackPart();
    glPopMatrix();
}
void drawTable(double topWid, double topThick, double legThick, double legLen){
    glPushMatrix();
    glTranslated(0, legLen, 0);
    glScaled(topWid, topThick, topWid);
    glutSolidCube(1.0);
    glPopMatrix();

    double dist = 0.95*topWid / 2.0 - legThick / 2.0;
    glPushMatrix();
    glTranslated(dist, 0, dist);
    drawTableLeg(legThick, legLen);
    glTranslated(0, 0, -2 * dist);
    drawTableLeg(legThick, legLen);
    glTranslated(-2 * dist, 0, 2 * dist);
    drawTableLeg(legThick, legLen);
    glTranslated(0, 0, -2 * dist);
    drawTableLeg(legThick, legLen);
    glPopMatrix();
}

void drawSnowMan() {
    glPushMatrix();
    
    glScaled(0.1, 0.1, 0.1);
    glTranslated(0.7, 3, 0.7);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Draw Body
    glTranslatef(0.0f ,0.75f, 0.0f);
    glutSolidSphere(0.75f,20,20);
    
    // Draw Head
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f,20,20);
    
    // Draw Eyes
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f,10,10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f,10,10);
    glPopMatrix();
    
    // Draw Nose
    glColor3f(1.0f, 0.5f , 0.5f);
    glutSolidCone(0.08f,0.5f,10,2);
    
    glPopMatrix();
}


void bed() {
    glPushMatrix();
    glColor3f(1.0, 0.4, 0.2);
    glTranslated(0.2, 0.0, 0.2);
    drawTable(0.4, 0.07, 0.05, 0.2);
    glColor3f(0.5, 0.5, 0.5);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0, 0.4, 0.9);
    glTranslated(0.2, 0.0, 0.2);
    glScaled(0.3, 0.3, 0.3);
    glutSolidCube(1);
    glColor3f(0.5, 0.5, 0.5);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0, 0.6, 0.4);
    glTranslated(0.01, 0.3, 0.2);
    glScaled(0.05, 0.15, 0.4);
    glutSolidCube(1);
    glColor3f(0.5, 0.5, 0.5);
    glPopMatrix();
}

void mirror() {
    glPushMatrix();
    glColor3f(1.0, 0.4, 0.6);
    glTranslated(0.15, 0.0, 0.7);
    drawTable(0.3, 0.02, 0.03, 0.4);
    glColor3f(0.5, 0.5, 0.5);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.6, 0.8, 0.4);
    glTranslated(0.01, 0.65, 0.7);
    glScaled(0.05, 0.5, 0.3);
    glutSolidCube(1);
    glColor3f(0.5, 0.5, 0.5);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.2, 0.6, 0.7);
    glTranslated(0.04, 0.67, 0.71);
    glScaled(0.01, 0.4, 0.2);
    glutSolidCube(1);
    glColor3f(0.5, 0.5, 0.5);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glTranslated(0.24, 0.45, 0.6);
    glScaled(0.5, 0.5, 0.5);
    glRotated(30, 0, 1, 0);
    glutSolidTeapot(0.08);
    glColor3f(0.5, 0.5, 0.5);
    glPopMatrix();
}

void closet() {
    glPushMatrix();
    glColor3f(0.5, 0.9, 0.4);
    glTranslated(0.73, 0.35, 0.1);
    glScaled(0.45, 0.75, 0.2);
    glutSolidCube(1);
    glColor3f(0.5, 0.5, 0.5);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.7, 0.2, 0.4);
    glTranslated(0.85, 0.13, 0.17);
    glScaled(0.15, 0.1, 0.1);
    glutSolidCube(1);
    glColor3f(0.5, 0.5, 0.5);
    glPopMatrix();
    
    
    glPushMatrix();
    glColor3f(0.7, 0.2, 0.4);
    glTranslated(0.65, 0.13, 0.17);
    glScaled(0.15, 0.1, 0.1);
    glutSolidCube(1);
    glColor3f(0.5, 0.5, 0.5);
    glPopMatrix();
    
    
    glPushMatrix();
    glColor3f(0.9, 0.6, 0.4);
    glTranslated(0.75, 0.45, 0.17);
    glScaled(0.35, 0.45, 0.1);
    glutSolidCube(1);
    glColor3f(0.5, 0.5, 0.5);
    glPopMatrix();
}

// -----------------------------------
//          Helper Methods
// -----------------------------------


void setupLights() {
    GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
    GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat shininess[] = { 50 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
    GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}

void setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, WINDOW_WIDTH / WINDOW_HEIGHT, 0.001, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    camera.look();
}

void topCam() {
    camera.eye.x = 0.572740;
    camera.eye.y = 2.717326;
    camera.eye.z = 0.0;
    
    camera.center.x = 0.501404;
    camera.center.x = 1.719873;
    camera.center.x = 0.0;
    
    camera.up.x = -0.997452;
    camera.up.x = 0.071336;
    camera.up.x = 0.0;
}

void defaultCam() {
    camera.eye.x = 2.502736;
    camera.eye.y = 0.645169;
    camera.eye.z = 0.032397;
    
    camera.center.x = 1.526539;
    camera.center.x = 0.432568;
    camera.center.x = -0.010505;
    
    camera.up.x = -0.212396;
    camera.up.x =  0.977139;
    camera.up.x = -0.009335;
}

void roomsWalls() {
    drawWall(0.02);

    glPushMatrix();
    glRotated(90, 0, 0, 1.0);
    drawWall(0.02);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0.6, 0.5);
    glRotated(-90, 1.0, 0.0, 0.0);
    drawWall(0.02);
    glColor3f(0.5, 0.5, 0.5);
    glPopMatrix();

    glPushMatrix();
    glRotated(180, 1, 0, 0.0);
    drawWall(0.02);
    glPopMatrix();
    
    glPushMatrix();
    glRotated(90, 0, 0, 1.0);
    glPushMatrix();
    glRotated(180, 1, 0, 0.0);
    drawWall(0.02);
    glPopMatrix();
    glPopMatrix();
}
