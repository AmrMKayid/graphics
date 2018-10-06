#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

// ------------------ Method declaration ------------------ //
void changeSize(int w, int h);
void Display(void);

// ------------------------------------------------ //

// ------------------ Variables ------------------ //
float angle = 0.0f;


// ------------------------------------------------ //


int main(int argc, char** argr) {
    
    // init GLUT and create window
    glutInit(&argc, argr);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("Space Invaders");
    
    // register callbacks
    glutDisplayFunc(Display);
    glutReshapeFunc(changeSize);
    glutIdleFunc(Display);  // For animation

    
    // enter GLUT event processing cycle
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
    gluLookAt(    0.0f, 0.0f, 10.0f,
              0.0f, 0.0f,  0.0f,
              0.0f, 1.0f,  0.0f);
    
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    
    glBegin(GL_TRIANGLES);
    glVertex3f(-2.0f,-2.0f, 0.0f);
    glVertex3f( 2.0f, 0.0f, 0.0);
    glVertex3f( 0.0f, 2.0f, 0.0);
    glEnd();
    
    angle+=0.1f;
    
    glutSwapBuffers();
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


