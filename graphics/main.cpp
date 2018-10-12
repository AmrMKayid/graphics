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
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 800
#define STEP 70

// -----------------------------------
//          Methods Signatures
// -----------------------------------

void gameStates();
void gameMenu();
void display();
void gameOver();

void myTimer( int valor);

void keyboardListener(unsigned char key, int x, int y);
void keyboardSpecialListener(int key, int x, int y);

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
        return new Bullet(this->centerX() - 8, this->centerY() + 35, 5, 25);
    }
};


class Enemy : public Object {
public:
    Enemy(double xx, double xy, double xwidth, double xheight):Object(xx,xy,xwidth,xheight) {}
    int padding;
    double direction;
    
    void draw() {
        glColor3f(1.0, 0.0, 0.0);
        drawCircle(x, y, (width + height) / 2);
    }
    
    
    bool collided(double fX, double fY){
        if ((this->x + this->full_width >= fX) && (this->x <= fX)
            && ((this->y - this->full_height <= fY) && (this->y >= fY))) {
            int _x = (fX - this->x)/(this->width+this->padding);
            if(fX >= (_x * (this->width+this->padding)+ this->x)-5 &&
               fX < (_x * (this->width+this->padding)+ this->x + this->width)){
                //          int _y = (fY - (this->y - this->full_height))/(this->height+this->padding);
                cout<<"Collided\n";
                return true;
            }
        }
        return false;
    }
    
    bool update(){
        
        if(
           this->x <= (85 - this->full_width) &&
           this->x >= -85
           ){
            this->translateX(this->direction);
        }else{
            this->direction = -1 * this->direction;
            this->translateX(this->direction);
            this->translateY(-1);
        }
        
        if(this->y - this->full_height < -90)
            return true;
        return false;
    }
    
    Bullet* generateBullet(int i, int j){
        int x = this->x + (j* (this->width+this->padding));
        int y = this->y - (i* (this->height+this->padding));
        y -= this->height + 3;
        x += this->width/2;
        return new Bullet(x,y,1,3);
    }
    
    Bullet* shoot(double x){
        srand((unsigned)time(0));
        
        int _x = (x - this->x)/(this->width+this->padding);
        int dx = rand()%2;
        int ix = _x - dx;
        int sx = _x + dx;
        if(ix < 0) ix = 0;
        if(ix > 7) ix = 7;
        if(sx < 0) sx = 0;
        if(sx > 7) sx = 7;
        
        for(int j = ix; j<=sx;j++){
            if(rand()%10 > 1){
                int o = 3;
                while(o >= 0){
                    return generateBullet(o,j);
                    //                    o--;
                }
            }
        }
        return NULL;
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
                score += 10;
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
    
};



// -----------------------------------
//          Global Variables
// -----------------------------------

GLuint bg[1];
int gamestate = 0, score = 0, health = 100, level = 0, lives = 3, speed = 50, bulletYield = 0, bulletYieldIndex = 0;
bool paused = true, init = true, levelUpeado = true;
SpaceShip *ship = new SpaceShip(WINDOW_WIDTH / 2, 50, 10, 10);
BulletObserver *ship_bullets = new BulletObserver();
Enemy *enemy = new Enemy(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50, 10, 10);
Collider *collider = new Collider(ship_bullets, enemy);
BulletObserver *enemiesBulletObserver = new BulletObserver();
ColliderShip *colliderShip = new ColliderShip(enemiesBulletObserver, ship);


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
    
    /* Callbacks */
    glutDisplayFunc(gameStates);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(keyboardSpecialListener);
    glutTimerFunc(speed, myTimer, 1);
    
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
    
    string temp=convertInt(score);
    
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
    
    string temp=convertInt(score);
    
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
    (new Bullet(enemy->centerX() - 7, enemy->centerY() - 30, 5, 25))->draw();
    
    enemiesBulletObserver->draw();
    
    
    string scoreStr="Score: "+ convertInt(score);
    string healthStr="Health: "+ convertInt(health);
    rendertext(10, WINDOW_HEIGHT-20, healthStr);
    rendertext(10, WINDOW_HEIGHT-40, scoreStr);
    
    
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

// -----------------------------------
//          Timer Function
// -----------------------------------



bool dead(){
    return (lives < 0);
}

void levelUp(){
    level++;
    if(speed > 20)
        speed-=10;
    
    //    if(bulletYieldIndex < 19)
    //        bulletYieldIndex--;
    //    resetElements();
    //    enemies->generate();
}

void myTimer( int valor) {
    ship_bullets->update(10);
    cout<<"Timer:: Bullet Movement\n";
    if(!paused && !init){
        ship_bullets->update(10);
        enemiesBulletObserver->update(-1);
        if( !dead() && !enemy->update()){
            glutTimerFunc(speed,myTimer,1);
            int scum = score;
            score += collider->checkForCollisions();
            if(score > scum)
                levelUpeado = false;
            //RESET
            if(score > 1 && score%320 == 0 && !levelUpeado){
                levelUp();
                levelUpeado = true;
            }
            if(bulletYield == 0){
                enemiesBulletObserver->addBullet(enemy->shoot(ship->x));
                bulletYield = 20 - bulletYieldIndex;
            }else
                bulletYield--;
            
            //            if(ship->checkForCollisions() > 0)
            //                lives--;
        }
    }else{
        //TODO
        //render pause
        glutTimerFunc(speed,myTimer,1);
    }
    glutPostRedisplay();
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

string convertInt(int number) {
    stringstream ss;
    
    ss << number;
    return ss.str();
}

// render text  and show it
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
