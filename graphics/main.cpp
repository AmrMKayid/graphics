#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#include <stdio.h>
#include <math.h>

void Display(void);
void Anim(void);

int main(int argc, char** argr)
{
    glutInit(&argc, argr);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Batman");
    glutDisplayFunc(Display);
    glutIdleFunc(Anim);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    glutMainLoop();
}

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    
    glPushMatrix();
    glScaled(10, 10, 1);
    glTranslated(25, 25, 0);
    
    glBegin(GL_POINTS);
    
    for (double xcord = 7; xcord >= 4; xcord -= 0.01)
    {
        double ycordPos = 2 * sqrt((double)(-1 * (abs(abs(xcord) - 1))*abs(3 - abs(xcord)) / ((abs(xcord) - 1)*(3 - abs(xcord)))))*
        (1 + abs(abs(xcord) - 3) / (abs(xcord) - 3))*sqrt((double)(1 - pow((xcord / 7), 2))) +
        (5 + 0.97*(abs(xcord - 0.5) + abs(xcord + 0.5)) - 3 * (abs(xcord - 0.75) + abs(xcord + 0.75)))*
        (1 + abs(1 - abs(xcord)) / (1 - abs(xcord)));
        glVertex3d(xcord, ycordPos, 0);
        glVertex3d(-xcord, ycordPos, 0); // Mirror X
        
        double ycordNeg = -3 * sqrt((double)(1 - pow((xcord / 7), 2)))*sqrt((double)(abs(abs(xcord) - 4) / (abs(xcord) - 4)));
        glVertex3d(xcord, ycordNeg, 0);
        glVertex3d(-xcord, ycordNeg, 0); // Mirror X
    }
    
    for (double xcord = 3.99; xcord >= 0; xcord -= 0.01)
    {
        double ycordPos;
        if (xcord >= 3 || xcord <= 1)
        {
            ycordPos = 2 * sqrt((double)(-1 * (abs(abs(xcord) - 1))*abs(3 - abs(xcord)) / ((abs(xcord) - 1)*(3 - abs(xcord)))))*
            (1 + abs(abs(xcord) - 3) / (abs(xcord) - 3))*sqrt((double)(1 - pow((xcord / 7), 2))) +
            (5 + 0.97*(abs(xcord - 0.5) + abs(xcord + 0.5)) - 3 * (abs(xcord - 0.75) + abs(xcord + 0.75)))*
            (1 + abs(1 - abs(xcord)) / (1 - abs(xcord)));
        }
        else
        {
            ycordPos = 2.71052 + (1.5 - 0.5*abs(xcord)) - 1.35526 * sqrt((double)(4 - pow(abs(xcord) - 1, 2)))*
            sqrt((double)(abs(abs(xcord) - 1) / (abs(xcord) - 1))) + 0.9;
        }
        glVertex3d(xcord, ycordPos, 0);
        glVertex3d(-xcord, ycordPos, 0);
        
        double ycordNeg = abs(xcord / 2) - 0.0913722*pow(xcord, 2) - 3 + sqrt((double)(1 - pow((abs(abs(xcord) - 2) - 1), 2)));
        glVertex3d(xcord, ycordNeg, 0);
        glVertex3d(-xcord, ycordNeg, 0);
    }
    
    glEnd();
    
    glPopMatrix();
    
    glFlush();
}

void Anim()
{
    
    glutPostRedisplay();
}
