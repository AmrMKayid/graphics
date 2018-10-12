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
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

// -----------------------------------
//              Constant
// -----------------------------------
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define STEP 50

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
        x += (deltaX > 0 & x < WINDOW_WIDTH)? deltaX: (deltaX < 0 & x > 0)? deltaX: 0;
    }
    
    void translateY(double deltaY){
        y += deltaY;
    }
    
    void draw(){
        glColor3f(1.0, 0.0, 0.0);
        drawRect(x, y, width, height);
    }
    
    double centerX(){
        return this->x + (this->width)/2;
    }
    
    double centerY(){
        return this->y + (this->height)/2;
    }

};


class Bullet : public Object {
public:
    Bullet(double xx, double xy, double xwidth, double xheight):Object(xx,xy,xwidth,xheight) {}
    Bullet(double xx, double xy):Object(xx,xy,1,1) {}
};

class BulletNode{
public:
    Bullet* bullet;
    BulletNode* next;
    
    BulletNode() {
        this->bullet = NULL;
        this->next = NULL;
    }
    
    BulletNode(Bullet* bullet) {
        this->bullet = bullet;
        this->next = NULL;
    }
    
    void add(Bullet* bullet){
        if(this->bullet == NULL)
            this->bullet = bullet;
        else{
            BulletNode *node = this;
            while(node->next != NULL) node = node->next;
            node->next = new BulletNode(bullet);
        }
    }

};

class BulletObserver{
    
public:
    BulletNode* bulletList;
    
    BulletObserver() { this->bulletList = NULL; }
    
    void addBullet(Bullet* bullet){
        if(bullet != NULL){
            if(this->bulletList == NULL)
                this->setBullet(bullet);
            else
                this->bulletList->add(bullet);
        }
    }
    
    void setBullet(Bullet* bullet){
        if(bullet != NULL)
            this->bulletList = new BulletNode(bullet);
    }
    
    void draw(){
        BulletNode *node = this->bulletList;
        while(node != NULL){
//            cout<<"Draw bullet";
//            cout<<node->bullet->x<<" "<<node->bullet->y<<" "<<node->bullet->width<<node->bullet->height;
            if(node->bullet->y > 100 )
                this->bulletList = NULL;
            else
                if (node->bullet->y > -90)
                    node->bullet->draw();
            node = node->next;
        }
    }
    
    void update(int n){
        BulletNode *node = this->bulletList;
        while(node != NULL){
            node->bullet->translateY(n);
            node = node->next;
        }
    }
    
    bool isEmpty(){
        return (this->bulletList == NULL);
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
    
    Bullet* shoot(){
        return new Bullet(this->centerX(), this->centerY(), 10, 10);
    }
};


class Enemy : public Object {
public:
    Enemy(double xx, double xy, double xwidth, double xheight):Object(xx,xy,xwidth,xheight) {}
    
    void draw() {
        glColor3f(1.0, 0.0, 0.0);
        drawCircle(x, y, (width + height) / 2);
    }
    bool update();
    void shoot(double x);

};





// -----------------------------------
//          Global Variables
// -----------------------------------
SpaceShip *ship = new SpaceShip(WINDOW_WIDTH / 2, 50, 50, 50);
BulletObserver *bulletObserver = new BulletObserver();
Enemy *enemy = new Enemy(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50, 10, 10);

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
            ship->translateX(-STEP);
            break;
        case 'd':
            ship->translateX(STEP);
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
    enemy->draw();
    bulletObserver->draw();
    
    glFlush();
}

// -----------------------------------
//               Main
// -----------------------------------
int main(int argc, char** argv) {
    /* Initializers */
    srand((unsigned)time(0));
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Kayid - Space Invaders");
    
    /* Callbacks */
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    /* Main loop */
    glutMainLoop();
    return 0;
}
