#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include <stdlib.h>
#include <math.h>


// ------------------ Method declaration ------------------ //
void changeSize(int w, int h);
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void drawSnowMan();
void Display(void);

// --------------------------------------------------------------- //

// ------------------ Global Variables ------------------ //
//float angle = 0.0f;
float red=1.0f, blue=1.0f, green=1.0f;

// ----- Camera ----- //
// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f, lz=-1.0f;
// XZ position of the camera
float x=0.0f, z=5.0f;

// --------------------------------------------------------------- //


int main(int argc, char** argr) {
    
    // ----- init GLUT and create window ----- //
    glutInit(&argc, argr);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("Space Invaders");
    
    
    // ----- register callbacks ----- //
    glutDisplayFunc(Display);
    glutReshapeFunc(changeSize);
    
    // for animation
    glutIdleFunc(Display);
    
    // for keys
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);

    
    // ----- enter GLUT event processing cycle ----- //
    glutMainLoop();
    
//    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
//    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    
    
    return 1;
}

void Display(void) {
    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(    x, 1.0f, z,
             x+lx, 1.0f,  z+lz,
             0.0f, 1.0f,  0.0f);
    
//    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    
    // Draw ground
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
        glVertex3f(-100.0f, 0.0f, -100.0f);
        glVertex3f(-100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f, -100.0f);
    glEnd();
    
//    angle+=0.1f;
    
    // Draw 36 SnowMen
    for(int i = -3; i < 3; i++)
        for(int j=-3; j < 3; j++) {
            glPushMatrix();
            glTranslatef(i*10.0,0,j * 10.0);
            drawSnowMan();
            glPopMatrix();
        }
    
    glutSwapBuffers();
}

// ------------------ Method declaration ------------------ //
void processNormalKeys(unsigned char key, int x, int y) {
    
    if (key == 27)
        exit(0);
    else if (key=='r') {
        int mod = glutGetModifiers();
        if (mod == GLUT_ACTIVE_ALT)
            red = 0.0;
        else
            red = 1.0;
    }
}

void processSpecialKeys(int key, int x, int y) {
    
    float fraction = 0.1f;
    
    switch (key) {
        case GLUT_KEY_LEFT :
            angle -= 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_RIGHT :
            angle += 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_UP :
            x += lx * fraction;
            z += lz * fraction;
            break;
        case GLUT_KEY_DOWN :
            x -= lx * fraction;
            z -= lz * fraction;
            break;
    }
}

// --------------------------------------------------------------- //

void drawSnowMan() {
    
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
}

void changeSize(int w, int h) {
    
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;
    
    float ratio =  w * 1.0 / h;
    
    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    
    // Reset Matrix
    glLoadIdentity();
    
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    // Set the correct perspective.
    gluPerspective(45,ratio,1,100);
    
    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}




