/* Libraries */

#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <time.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

/* Personal include files */
#include "gameworld.hpp"

using namespace std;

/* Callback prototypes */
void reshape(int w, int h);
void display();
void keyboardListener(unsigned char c, int x, int y);
void mouseListener(int b, int s, int x, int y);
void update(int value);

/* Game world. Use to control things */
GameWorld world;

int main(int argc, char** argv) {
    /* Initializers */
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitWindowSize(GameWorld::SUGGESTED_WIDTH, GameWorld::SUGGESTED_HEIGHT);
    glutInitWindowPosition(GameWorld::STARTING_WINDOW_X, GameWorld::STARTING_WINDOW_Y);
    glutCreateWindow("Assignment 1 - Space Invaders");
    
    /* Callbacks */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardListener);
    glutMouseFunc(mouseListener);
    glutTimerFunc(GameWorld::REFRESH_RATE, update, 0);
    world.resetTime();
    /* Main loop */
    glutMainLoop();
    return 0;
}

void reshape(int w, int h) {
    glViewport (0, 0, w, h);                    // update the viewport
    glMatrixMode(GL_PROJECTION);                // update projection
    glLoadIdentity();
    gluOrtho2D(0.0, GameWorld::WINDOW_WIDTH, 0.0, GameWorld::WINDOW_HEIGHT);             // map unit square to viewport
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();                        // request redisplay
}

void display() {                          // display callback
    world.drawGame();
    glutSwapBuffers();                          // swap buffers
}

void mouseListener(int b, int s, int x, int y) {      // mouse click callback
    world.processMouse(b, s, x, y);
}
// keyboard callback
void keyboardListener(unsigned char c, int x, int y) {
    world.processKeyPress(c, x, y);
}

void update(int value) {
    world.updateState();
    glutPostRedisplay();
    glutTimerFunc(GameWorld::REFRESH_RATE, update, 0);
}
