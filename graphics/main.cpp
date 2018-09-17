#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(150.0f, 200.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(100.0f, 100.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(200.0f, 100.0f, 0.0f);
    glEnd();
    
    glFlush();
}

int main(int argc, char** argr) {
    glutInit(&argc, argr);
    
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(150, 150);
    
    glutCreateWindow("OpenGL - 2D Template");
    glutDisplayFunc(Display);
    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gluOrtho2D(0.0, 300, 0.0, 300);
    
    glutMainLoop();
}
