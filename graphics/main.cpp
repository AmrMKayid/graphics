#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif


void Display(void);

int main(int argc, char** argr) {
    
    // init GLUT and create window
    glutInit(&argc, argr);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("Space Invaders");
    
    // register callbacks
    glutDisplayFunc(Display);
    
    // enter GLUT event processing cycle
    glutMainLoop();
    
//    glutIdleFunc(Anim);
//    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
//    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    
    
    return 1;
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5,-0.5,0.0);
    glVertex3f(0.5,0.0,0.0);
    glVertex3f(0.0,0.5,0.0);
    glEnd();
    
    glutSwapBuffers();
}


