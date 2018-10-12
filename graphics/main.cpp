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
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

// -----------------------------------
//              Constant
// -----------------------------------
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 800
#define STEP 10
#define BULLET_SPEED 10
#define SCORE_PLUS 10

// -----------------------------------
//          Methods Signatures
// -----------------------------------

void gameStates();
void gameMenu();
void display();
void gameOver();

void myTimer( int valor);
void Anim();

void keyboardListener(unsigned char key, int x, int y);
void keyboardSpecialListener(int key, int x, int y);

void enemy_movement();
int* bezier(float t, int* p0,int* p1,int* p2,int* p3);
void init_bezier();
void restart();
string convertInt(int number);
void rendertext(float x,float y, string strings);
void drawRect(int x, int y, int w, int h);
void drawCircle(int x, int y, float r);

// -----------------------------------
//              Classes
// -----------------------------------

class Object{
    
public:
    double x;
    double y;
    double width;
    double height;
    double full_width;
    double full_height;
    
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
    
    bool collided(double fX, double fY){
        if ( (fX >= this->x && fX <= this->x + this->width) && (fY >= this->y && fY <= this->y + this->height) )
            return true;
        return false;
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
    int score = 0;
    
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
        return new Bullet(this->centerX() - 17, this->centerY() + 35, 5, 25);
    }
};


class Enemy : public Object {
public:
    int health = 100;
    int bullet_timer = 0;
    
    Enemy(double xx, double xy, double xwidth, double xheight):Object(xx,xy,xwidth,xheight) {}

    void draw() {
        glColor3f(1.0, 0.0, 0.0);
        drawRect(x, y, width, height);
    }

    Bullet* shoot(){
        return new Bullet(this->centerX() - 7, this->centerY() - 30, 5, 25);
    }
    
};


class Collider {
public:
    BulletObserver* bulletObserver;
    Enemy* enemy;
    
    Collider(BulletObserver* bulletObserver, Enemy* enemy){
        this->bulletObserver = bulletObserver;
        this->enemy = enemy;
    }
    
    int checkForCollisions(){
        int score = 0;
        BulletNode* previous = NULL;
        BulletNode* current = bulletObserver->bulletList;
        
        while(current != NULL){
            if(enemy->collided(current->bullet->x, current->bullet->y)){
                score += SCORE_PLUS;
                if( previous == NULL)
                    bulletObserver->bulletList = current->next;
                else{
                    current = current -> next;
                    previous->next = current;
                }
            }
            previous = current;
            if(current != NULL)
                current = current -> next;
        }
        
        return score;
    }
};


class ColliderShip{
public:
    BulletObserver* bulletObserver;
    SpaceShip* ship;
    ColliderShip(BulletObserver* bulletObserver, SpaceShip* ship){
        this->bulletObserver = bulletObserver;
        this->ship = ship;
    }
    
    int checkForCollisions(){
        int score = 0;
        BulletNode* previous = NULL;
        BulletNode* current = bulletObserver->bulletList;
        
        while(current != NULL){
            if(ship->collided(current->bullet->x, current->bullet->y)){
                score += SCORE_PLUS;
                if( previous == NULL)
                    bulletObserver->bulletList = current->next;
                else{
                    current = current -> next;
                    previous->next = current;
                }
            }
            previous = current;
            if(current != NULL)
                current = current -> next;
        }
        
        return score;
    }
};



// -----------------------------------
//          Global Variables
// -----------------------------------

int gamestate = 0;
int p0[2], p1[2], p2[2], p3[2];
double t = 0, beizer_timer = 0;
bool movement_reverse = false;

SpaceShip *ship = new SpaceShip(WINDOW_WIDTH / 2, 50, 30, 30);
BulletObserver *ship_bullets = new BulletObserver();

Enemy *enemy = new Enemy(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100, 50, 50);
BulletObserver *enemy_bullets = new BulletObserver();

Enemy *enemy_defender = new Enemy(WINDOW_WIDTH, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 10, 30);

Collider *collider = new Collider(ship_bullets, enemy);
ColliderShip *colliderShip = new ColliderShip(enemy_bullets, ship);


// -----------------------------------
//               Main
// -----------------------------------
int main(int argc, char** argv) {
    /* Initializers */
    srand((unsigned)time(0));
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Kayid - Space Invaders");
    
    init_bezier();
    
    /* Callbacks */
    glutDisplayFunc(gameStates);
    glutIdleFunc(Anim);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(keyboardSpecialListener);
    glutTimerFunc(0, myTimer, 0);
    
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    
    /* Main loop */
    glutMainLoop();
    return 0;
}


// -----------------------------------
//          Game World Methods
// -----------------------------------

void gameStates() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    switch(gamestate) {
        case 0:
            gameMenu();
            break;
        case 1:
            display();
            break;
        case 2:
            gameOver();
            break;
    }
}

void gameMenu() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    string txtGameName="Kayid Space Invader";
    string txtStart="Press (P)lay to Start";
    
    string temp=convertInt(ship->score);
    
    glColor3f(.56,.75,.23);
    glRasterPos2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 250);
    int i;
    for(i=0;i<= txtGameName.length();i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)txtGameName[i]);
        
    }
    
    glRasterPos2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 150);
    for(i=0;i<= txtStart.length();i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)txtStart[i]);
        
    }
    
    glFlush();
    glutSwapBuffers();
    
}

void gameOver() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    string txtGameover="GAME OVER";
    string txtTotalScore="Total Score : ";
    string txtRestart="Press r to restart";
    
    string temp=convertInt(ship->score);
    
    glColor3f(.26,.32,.77);
    glRasterPos2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 100);
    int i;
    for(i=0;i<=txtGameover.length();i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)txtGameover[i]);
        
    }
    
    glRasterPos2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 50);
    
    for(i=0;i<=txtTotalScore.length();i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)txtTotalScore[i]);
    }
    
    for(i=0;i<=temp.length();i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)temp[i]);
    }
    glRasterPos2f(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2);
    
    for(i=0;i<=txtRestart.length();i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)txtRestart[i]);
    }
    glFlush();
    glutSwapBuffers();
    
}

void display() {
    start:
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    ship->draw();
    ship_bullets->draw();
    
    enemy->draw();
    enemy_bullets->draw();
    
    enemy_defender->draw();
    
    
    string scoreStr="Score: "+ convertInt(ship->score);
    rendertext(10, WINDOW_HEIGHT-20, scoreStr);
    
    string healthStr="Health: "+ convertInt(enemy->health);
    rendertext(WINDOW_WIDTH - 150, WINDOW_HEIGHT-20, healthStr);
    
    
    
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

// -----------------------------------
//          Timer Function
// -----------------------------------

void myTimer( int valor) {
    
    enemy_movement();
    
    enemy_defender->translateX(-4*(STEP + STEP));
    if(enemy_defender->x < 0) {
        enemy_defender->x = WINDOW_WIDTH;
    }
    
    ship_bullets->update(BULLET_SPEED);
    
    enemy->bullet_timer += 10;
    if (enemy->bullet_timer == 300) {
        enemy->bullet_timer = 0;
        enemy_bullets->addBullet(enemy->shoot());
    }
    enemy_bullets->update(-BULLET_SPEED);

    glutTimerFunc(0, myTimer, 0);
    glutPostRedisplay();
}


void Anim() {
    int collision_amount = collider->checkForCollisions();
    ship->score += collision_amount;
    enemy->health -= collision_amount / 2;
    
    if(colliderShip->checkForCollisions() > 0) {
        gamestate = 2;
    }
}


// -----------------------------------
//          Keyboard Methods
// -----------------------------------

void keyboardListener(unsigned char key, int x, int y) {
    switch(key){
        case 32:
            if(ship_bullets->isEmpty()){
                Bullet *bullet = ship->shoot();
                ship_bullets->setBullet(bullet);
            } else {
                ship_bullets->addBullet(ship->shoot());
            }
            break;
        case 'R':
        case 'r':
            gamestate=1;
            break;
        case 'P':
        case 'p':
            gamestate=1;
            break;
        case 27:
            exit(0);
    }
    
    glutPostRedisplay();
}

void keyboardSpecialListener(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            ship->translateX(STEP);
            break;
        case GLUT_KEY_LEFT:
            ship->translateX(-STEP);
            break;
    }
    
    //update display
    glutPostRedisplay();
}

// -----------------------------------
//          Helper Methods
// -----------------------------------

void enemy_movement() {
    beizer_timer += 30;
    if(beizer_timer > 100) {
        beizer_timer = 0;
        t += (movement_reverse)? (-0.01) : (0.01);
    }
    if(t > 0.9) {
        movement_reverse = true;
    } else if(t < 0.1) {
        movement_reverse = false;
    }
    int *res = bezier(t, p0, p1, p2, p3);
    
    int enemy_newX = res[0], enemy_newY = res[1];
    enemy->x = enemy_newX;
    enemy->y = enemy_newY;
}

int* bezier(float t, int* p0,int* p1,int* p2,int* p3) {
    int res[2];
    res[0]=pow((1-t),3)*p0[0]+3*t*pow((1-t),2)*p1[0]+3*pow(t,2)*(1-t)*p2[0]+pow(t,3)*p3[0];
    res[1]=pow((1-t),3)*p0[1]+3*t*pow((1-t),2)*p1[1]+3*pow(t,2)*(1-t)*p2[1]+pow(t,3)*p3[1];
    return res;
}

void init_bezier() {
    p0[0]=WINDOW_WIDTH / 4;
    p0[1]=WINDOW_WIDTH / 4 + 100;
    
    p1[0]=WINDOW_WIDTH / 4;
    p1[1]=WINDOW_WIDTH / 4 + 400;
    
    p2[0]=WINDOW_WIDTH / 4 + 800;
    p2[1]=WINDOW_WIDTH / 4 + 400;
    
    p3[0]=WINDOW_WIDTH / 4 + 800;
    p3[1]=WINDOW_WIDTH / 4 + 100;
}

void restart() {
    ship = new SpaceShip(WINDOW_WIDTH / 2, 50, 30, 30);
    enemy = new Enemy(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100, 50, 50);
}

void drawRect(int x, int y, int w, int h) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawCircle(int x, int y, float r) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    GLUquadric *quadObj = gluNewQuadric();
    gluDisk(quadObj, 0, r, 50, 50);
    glPopMatrix();
}

string convertInt(int number) {
    stringstream ss;
    
    ss << number;
    return ss.str();
}

void rendertext(float x,float y, string strings) {
    
    glColor3f(.26,.32,.77);
    
    glRasterPos2d(x,y);
    glDisable(GL_TEXTURE);
    glDisable(GL_TEXTURE_2D);
    for(int i=0;i<=strings.length();i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)strings[i]);
    }
    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
    
}
