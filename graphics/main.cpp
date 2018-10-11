/* Libraries */

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

// -----------------------------------
//          Constant
// -----------------------------------
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800


// -----------------------------------
//          Methods Signatures
// -----------------------------------
void drawRect(int x, int y, int w, int h);
void drawCircle(int x, int y, float r);
void keyboardListener(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);
void Display();


// -----------------------------------
//              Classes
// -----------------------------------
class Object{
    
public:
    double x;
    double y;
    double width;
    double height;
    
    Object(double xx, double xy, double xwidth, double xheight) {
        x = xx;
        y = xy;
        width = xwidth;
        height = xheight;
    }

    void translateX(double deltaX){
            x += deltaX;
    }
    
    void translateY(double deltaY){
        y += deltaY;
    }
    
    void draw(){
        drawRect(x, y, width, height);
    }
    
    double centerX(){
        return this->x + (this->width)/2;
    }
    
    double centerY(){
        return this->y + (this->height)/2;
    }

};


class SpaceShip : public Object {
public:
    SpaceShip(double xx, double xy, double xwidth, double xheight):Object(xx,xy,xwidth,xheight) {}
    void draw(){
        glPointSize(10.0);
        
        glBegin(GL_POINTS);//front shooter
        glColor3f(1,1,1);//111
        glVertex2f(x, y +40);
        glEnd();
        
        glBegin(GL_QUADS);
        glColor3f(0.137255,0.419608,0.556863);//middle body
        glVertex2f(x - 25, y + 25);
        glVertex2f(x - 25, y - 25);
        glVertex2f(x + 25, y - 25);
        glVertex2f(x + 25, y + 25);
        glEnd();
        
        glBegin(GL_POINTS);//design on middle
        glColor3f(0.90,0.91,0.98);
        glVertex2f(x-10,y-5);
        glVertex2f(x+10,y-5);
        glEnd();
        
        glBegin(GL_POINTS);//thrusters
        glColor3f(1,1,1);
        glVertex2f(x-10,y-30);
        glVertex2f(x+10,y-30);
        glEnd();
        
        glBegin(GL_TRIANGLES);//right wing
        glColor3f(0.196078,0.8,0.196078);
        glVertex2f(x+25,y+25);
        glVertex2f(x+25,y-25);
        glVertex2f(x+45,y-35);
        glEnd();
        
        glBegin(GL_TRIANGLES);//left wing
        glColor3f(0.196078,0.8,0.196078);
        glVertex2f(x-25,y+25);
        glVertex2f(x-25,y-25);
        glVertex2f(x-45,y-35);
        glEnd();
        
        
        glBegin(GL_QUADS);//up body
        glColor3f(0.99609, 0.83984, 0);
        glVertex2f(x - 25, y + 25);
        glVertex2f(x - 18, y + 40);
        glVertex2f(x + 18, y + 40);
        glVertex2f(x + 25, y + 25);
        glEnd();
    }
};


class Enemy : public Object {
public:
    Enemy(double xx, double xy, double xwidth, double xheight):Object(xx,xy,xwidth,xheight) {}
    
    void draw() {
        
    }
    bool update();
    void shoot(double x);

};




// -----------------------------------
//          Global Variables
// -----------------------------------
SpaceShip *ship = new SpaceShip(WINDOW_WIDTH / 2 + 25, 50, 50, 50);

// -----------------------------------
//              Methods
// -----------------------------------
// draws rectangles using the (x,y) of the bottom left vertex, width (w) and height (h)
void drawRect(int x, int y, int w, int h) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

// draws a circle using OpenGL's gluDisk, given (x,y) of its center and tis radius
void drawCircle(int x, int y, float r) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    GLUquadric *quadObj = gluNewQuadric();
    gluDisk(quadObj, 0, r, 50, 50);
    glPopMatrix();
}

void keyboardListener(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            ship->translateX(-10);
            break;
        case 'd':
            ship->translateX(10);
            break;
        default:
            break;
    }
    // ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    ship->draw();
    
    
    glFlush();
}

// -----------------------------------
//               Main
// -----------------------------------
int main(int argc, char** argv) {
    /* Initializers */
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Kayid - Space Invaders");
    
    /* Callbacks */
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
//    glutKeyboardUpFunc(KeyUp);
//    glutMouseFunc(mouseListener);
//    glutTimerFunc(GameWorld::REFRESH_RATE, update, 0);
    
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    /* Main loop */
    glutMainLoop();
    return 0;
}
