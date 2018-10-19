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
#include <cstdlib>
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
#define ANGEL 10
#define STEP 30
#define BULLET_SPEED 10
#define SCORE_PLUS 10
#define HEALTH(level) (level * 10)

// -----------------------------------
//          Methods Signatures
// -----------------------------------

void gameStates();
void gameMenu();
void display();
void gameOver();

void myTimer(int value);
void Anim();

void keyboardListener(unsigned char key, int x, int y);
void keyboardSpecialListener(int key, int x, int y);
void releaseKey(int key, int x, int y);

void ship_movement();
void enemy_movement();
void ship_shooting();
void enemy_shooting();
void enemy_defender_appearance();
void powerups_movement();
void checkForPowerUps();

void init_bezier();
void new_bezier();
int* bezier(float t, int* p0,int* p1,int* p2,int* p3);

void backgound();
void draw_ship();
void new_enemy();
void enemy_drawing();

string convertInt(int number);
void rendertext(float x,float y, string strings);
void drawRect(int x, int y, int w, int h);
void drawCircle(int x, int y, float r);
int random(int n, int m);

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

    void draw() {
        glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        drawRect(x, y, width, height);
        glColor3f(1.0, 1.0, 1.0);
        drawCircle(centerX(), centerY(), 5);
        
        glPointSize(3.0);
        glBegin(GL_POINTS);
        glColor3f(0.0, 0.0, 0.0);
        glVertex2f(centerX(), centerY());
        glEnd();
       

        glPopMatrix();
    }
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

class Powerups : public Object {
public:
    int id;
    Powerups(double xx, double xy, double xwidth, double xheight, int id):Object(xx,xy,xwidth,xheight) {
        this->id = id;
    }

    void draw() {
        if(id == 1) {
            glPushMatrix();
            glColor3f(1.0, 1.0, 0.0);
            drawRect(x, y, width, height);
            glPopMatrix();
        } else if(id == 2) {
            glPushMatrix();
            glColor3f(0.0, 1.0, 0.0);
            drawCircle(x, y, (width + height) / 2);
            glPopMatrix();
        }
    }

};


class SpaceShip : public Object {
public:
    int score, bullet_timer;
    bool is_moving_right, is_moving_left, is_firing;
    
    SpaceShip(double xx, double xy, double xwidth, double xheight):Object(xx,xy,xwidth,xheight) {
        score = 0;
        bullet_timer = 0;
        is_moving_right = false;
        is_moving_left = false;
        is_firing = false;
    }
    
    void draw(){
        glPointSize(10.0);
        
        glBegin(GL_POINTS);//front shooter
        glColor3f(1,1,1);//111
        glVertex2f((x + (width / 2)), (y + (height / 2)) +40);
        glEnd();
        
        glBegin(GL_QUADS);
        glColor3f(0.137255,0.419608,0.556863);//middle body
        glVertex2f((x + (width / 2)) - 25, (y + (height / 2)) + 25);
        glVertex2f((x + (width / 2)) - 25, (y + (height / 2)) - 25);
        glVertex2f((x + (width / 2)) + 25, (y + (height / 2)) - 25);
        glVertex2f((x + (width / 2)) + 25, (y + (height / 2)) + 25);
        glEnd();
        
        glBegin(GL_POINTS);//design on middle
        glColor3f(0.90,0.91,0.98);
        glVertex2f((x + (width / 2)) -10, (y + (height / 2)) -5);
        glVertex2f((x + (width / 2)) +10, (y + (height / 2)) -5);
        glEnd();
        
        glBegin(GL_POINTS);//thrusters
        glColor3f(1,1,1);
        glVertex2f((x + (width / 2)) -10, (y + (height / 2)) -30);
        glVertex2f((x + (width / 2)) +10, (y + (height / 2)) -30);
        glEnd();
        
        glBegin(GL_TRIANGLES);//right wing
        glColor3f(0.196078,0.8,0.196078);
        glVertex2f((x + (width / 2)) +25, (y + (height / 2)) +25);
        glVertex2f((x + (width / 2)) +25, (y + (height / 2)) -25);
        glVertex2f((x + (width / 2)) +45, (y + (height / 2)) -35);
        glEnd();
        
        glBegin(GL_TRIANGLES);//left wing
        glColor3f(0.196078,0.8,0.196078);
        glVertex2f((x + (width / 2)) -25, (y + (height / 2)) +25);
        glVertex2f((x + (width / 2)) -25, (y + (height / 2)) -25);
        glVertex2f((x + (width / 2)) -45, (y + (height / 2)) -35);
        glEnd();
        
        
        glBegin(GL_QUADS);//up body
        glColor3f(0.99609, 0.83984, 0);
        glVertex2f((x + (width / 2)) - 25, (y + (height / 2)) + 25);
        glVertex2f((x + (width / 2)) - 18, (y + (height / 2)) + 40);
        glVertex2f((x + (width / 2)) + 18, (y + (height / 2)) + 40);
        glVertex2f((x + (width / 2)) + 25, (y + (height / 2)) + 25);
        glEnd();


        // Borders
        /*
        glLineWidth(10); 
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(x, y, 0.0);
        glVertex3f(x + width, y, 0);
        glEnd();

        glLineWidth(10); 
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(x + width, y, 0.0);
        glVertex3f(x + width, y + height, 0);
        glEnd();

        glLineWidth(10); 
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(x + width, y + height, 0.0);
        glVertex3f(x, y + height, 0);
        glEnd();

        glLineWidth(10); 
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(x, y + height, 0.0);
        glVertex3f(x, y, 0);
        glEnd();
        */
    }

    
    Bullet* shoot(){
        return new Bullet(this->centerX() - 3, this->centerY() + 30, 5, 25);
    }
};


class Enemy : public Object {
public:
    int health, bullet_timer, type;
    
    Enemy(double xx, double xy, double xwidth, double xheight, int health, int type):Object(xx,xy,xwidth,xheight) {
        this->health = health;
        bullet_timer = 0;
        this->type = type;
    }

    void draw() {
        if(type == 0) {
            glPushMatrix();
            glTranslatef(this->x, this->y, 0);
            enemy_drawing();
            glPopMatrix();
        } else {
            glPushMatrix();
            glColor3f(0.3, 0.3, 0.3);
            drawRect(x, y, width - 10, height - 10);
            
            glPointSize(5.0);
            glBegin(GL_POINTS);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2f(centerX() - 40, centerY() - 5);
            glEnd();

            glBegin(GL_POINTS);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2f(centerX() - 30, centerY() - 5);
            glEnd();

            glBegin(GL_POINTS);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2f(centerX() - 20, centerY() - 5);
            glEnd();

            glBegin(GL_POINTS);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2f(centerX() - 10, centerY() - 5);
            glEnd();

            glBegin(GL_POINTS);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2f(centerX(), centerY() - 5);
            glEnd();

            glBegin(GL_POINTS);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2f(centerX() + 10, centerY() - 5);
            glEnd();

            glBegin(GL_POINTS);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2f(centerX() + 20, centerY() - 5);
            glEnd();

            glColor3f(0.9, 0.5, 0.3);
            drawCircle(x + width - 10, y + 10, 15);
            glColor3f(0.9, 0.5, 0.3);
            drawCircle(x - 10, y + 10, 15);
            glPopMatrix();
        }
    }

    Bullet* shoot(){
        return new Bullet(this->centerX() - 7, this->centerY() - 30, 5, 25);
    }
    
};


class Collider {
public:
    BulletObserver* bulletObserver;
    Enemy* enemy;
    Enemy* enemy_defender;
    
    Collider(BulletObserver* bulletObserver, Enemy* enemy, Enemy* enemy_defender){
        this->bulletObserver = bulletObserver;
        this->enemy = enemy;
        this->enemy_defender = enemy_defender;
    }
    
    int checkForCollisions(){
        int score = 0;
        BulletNode* previous = NULL;
        BulletNode* current = bulletObserver->bulletList;
        
        while(current != NULL){
            if (enemy_defender->collided(current->bullet->x, current->bullet->y)) {
                current->bullet->x = -10;
                current->bullet->y = -10;
            }
            if(enemy->collided(current->bullet->x, current->bullet->y)){
                current->bullet->x = -10;
                current->bullet->y = -10;
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

int gamestate = 0, level = 1;
int background_x=0, background_y=0, background_y2=-WINDOW_HEIGHT;
int p0[2], p1[2], p2[2], p3[2];
double t = 0, beizer_timer = 0, new_enemy_timer = 0, enemy_defender_timer = 0;
bool movement_reverse = false, restart = false, enemy_dead = false, isleveledUp = false;
bool playGame = false;

SpaceShip *ship = new SpaceShip(WINDOW_WIDTH / 2, 30, 70, 70);
BulletObserver *ship_bullets = new BulletObserver();

Enemy *enemy = new Enemy(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 200, 100, 100, HEALTH(level), 0);
BulletObserver *enemy_bullets = new BulletObserver();

Enemy *enemy_defender = new Enemy(WINDOW_WIDTH, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 10, 30, HEALTH(level), 1);

int powerups_timer = 0;
Powerups *pu1 = new Powerups(random(50, WINDOW_WIDTH), 0, 30, 30, 1);
Powerups *pu2 = new Powerups(random(50, WINDOW_WIDTH), 0, 30, 30, 2);

Collider *collider = new Collider(ship_bullets, enemy, enemy_defender);
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
    glutIgnoreKeyRepeat(1);
    glutSpecialUpFunc(releaseKey);
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

    backgound();

    // healthbar
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    double healthbar = ((enemy->health / ((double) HEALTH(level))));
    healthbar = (healthbar <= 0)? 0 : healthbar;
    drawRect(WINDOW_WIDTH - 250, WINDOW_HEIGHT - 50, healthbar * (230), 30);
    glPopMatrix();
    
    draw_ship();
    ship_bullets->draw();

    if(!enemy_dead) {
        if(t > 0.5) {
            glPushMatrix(); 
            glTranslatef((ship->x + (ship->width / 2)), (ship->y + (ship->height / 2)), 1);
            glRotatef(-ANGEL * t, 0, 0, 1);
            glTranslatef(-(ship->x + (ship->width / 2)), -(ship->y + (ship->height / 2)), -1);
            enemy->draw();
            glPopMatrix();
        } else {
            glPushMatrix(); 
            glTranslatef((ship->x + (ship->width / 2)), (ship->y + (ship->height / 2)), 1);
            glRotatef(ANGEL * t, 0, 0, 1);
            glTranslatef(-(ship->x + (ship->width / 2)), -(ship->y + (ship->height / 2)), -1);
            enemy->draw();
            glPopMatrix();
        }
        
        enemy_bullets->draw();
        enemy_defender->draw();
    }

    pu1->draw();
    pu2->draw();
    
    
    string scoreStr="Score: "+ convertInt(ship->score);
    rendertext(10, WINDOW_HEIGHT-20, scoreStr);
    
    int enemy_health_str = (enemy->health <= 0)? 0 : enemy->health;
    string healthStr="Health: "+ convertInt(enemy_health_str);
    rendertext(WINDOW_WIDTH - 170, WINDOW_HEIGHT-40, healthStr);

    if(enemy_dead) {
        rendertext(WINDOW_WIDTH / 2 - (WINDOW_WIDTH / 10), WINDOW_HEIGHT / 2, "YOU WON!! Level Up: "+ convertInt(level));
    }
    
    
    
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

// -----------------------------------
//          Timer Function
// -----------------------------------

void myTimer(int value) {
    if(playGame) {
        ship_movement();
        ship_shooting();
        
        enemy_movement();
        enemy_shooting();
        
        enemy_defender_appearance();

        background_y = (background_y >= WINDOW_HEIGHT)? 0 : background_y + 10;
        background_y2 = (background_y2 >= 0)? -WINDOW_HEIGHT : background_y2 + 10;

        powerups_movement();

        new_enemy();
    }
    glutTimerFunc(0, myTimer, 0);
    glutPostRedisplay();
}


void Anim() {
    if(playGame) {
        int collision_amount = collider->checkForCollisions();
        ship->score += collision_amount;
        enemy->health -= collision_amount / 2;
        
        if(colliderShip->checkForCollisions() > 0) {
            gamestate = 2;
            playGame = false;
        }

        if(restart && gamestate != 1) {
            ship = new SpaceShip(WINDOW_WIDTH / 2, 30, 70, 70);
            enemy = new Enemy(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 200, 100, 100, HEALTH(1), 0);
            collider = new Collider(ship_bullets, enemy, enemy_defender);
            colliderShip = new ColliderShip(enemy_bullets, ship);
            restart = false;
            gamestate=1;
            level = 1;
        }

        checkForPowerUps();
    }
}


// -----------------------------------
//          Keyboard Methods
// -----------------------------------

void keyboardListener(unsigned char key, int x, int y) {
    switch(key){
        case 32:
            ship->is_firing = true;
            break;
        case 'R':
        case 'r':
            restart = true;
            playGame = true;
            break;
        case 'P':
        case 'p':
            playGame = true;
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
            ship->is_moving_right = true;
            break;
        case GLUT_KEY_LEFT:
            ship->is_moving_left = true;
            break;
    }
    
    //update display
    glutPostRedisplay();
}

void releaseKey(int key, int x, int y) {

    switch (key) {
        case GLUT_KEY_RIGHT: 
            ship->is_moving_right = false;
            break;
        case GLUT_KEY_LEFT:
            ship->is_moving_left = false;
            break;
        case 32:
            ship->is_firing = false;
            break;
    }
}

// -----------------------------------
//          Movement & Shooting
// -----------------------------------

void ship_movement() {
    if(ship->is_moving_right) {
        ship->translateX(STEP);
    } else if(ship->is_moving_left) {
        ship->translateX(-STEP);
    }
}

void enemy_movement() {
    beizer_timer += 30;
    if(beizer_timer > 70) {
        beizer_timer = 0;
        t += (movement_reverse)? (-0.05) : (0.05);
    }
    if(t > 0.99) {
        movement_reverse = true;
        new_bezier();
    } else if(t < 0.01) {
        movement_reverse = false;
        new_bezier();
    }
    int *res = bezier(t, p0, p1, p2, p3);
    
    int enemy_newX = res[0], enemy_newY = res[1];
    enemy->x = enemy_newX;
    enemy->y = enemy_newY;
}

void ship_shooting() {
    if(ship->is_firing) {
        ship->bullet_timer += 10;
        if (ship->bullet_timer == 30) {
            ship->bullet_timer = 0;
            if(ship_bullets->isEmpty()){
                Bullet *bullet = ship->shoot();
                ship_bullets->setBullet(bullet);
            } else {
                ship_bullets->addBullet(ship->shoot());
            }
        }
    }
    
    ship_bullets->update(BULLET_SPEED);
}

void enemy_shooting() {
    enemy->bullet_timer += 10;
    if (enemy->bullet_timer == 300) {
        enemy->bullet_timer = 0;
        enemy_bullets->addBullet(enemy->shoot());
    }
    enemy_bullets->update(-BULLET_SPEED);
}

void enemy_defender_appearance() {
    enemy_defender_timer += 10;
    if (enemy_defender_timer == 300) {
        enemy_defender_timer = 0;
        enemy_defender->translateX(-4*(STEP + STEP));
        if(enemy_defender->x < 0) {
            enemy_defender->x = WINDOW_WIDTH;
        }
        enemy_bullets->addBullet(enemy_defender->shoot());
    }
    
}

void powerups_movement() {
    powerups_timer += 10;
    if (powerups_timer == 5000) {
        pu1 = new Powerups(random(50, WINDOW_WIDTH - 50), WINDOW_HEIGHT, 30, 30, 1);
    }
    if (powerups_timer == 10000) {
        pu2 = new Powerups(random(50, WINDOW_WIDTH - 50), WINDOW_HEIGHT, 30, 30, 2);
        powerups_timer = 0;
    }

    pu1->translateY(-STEP);
    pu2->translateY(-STEP);
}


void checkForPowerUps() {
    if(ship->collided(pu1->x, pu1->y)) {
        pu1->translateY(-1000);
        enemy->health /= 2;
    } else if(ship->collided(pu2->x, pu2->y)) {
        pu2->translateY(-1000);
        ship->score += 100;
    }
}

// -----------------------------------
//          Helper Methods
// -----------------------------------


void backgound() {
    glPushMatrix(); 
    glColor3f(0.0, 0.0, 0.0);
    drawRect(background_x, background_y, WINDOW_WIDTH, WINDOW_HEIGHT);

    for(int i = 0; i < 10; ++i) {
        glColor3f(1.0, 1.0, 1.0);
        drawCircle(random(0, WINDOW_WIDTH), random(0, WINDOW_HEIGHT), 1.5);
    }

    glPopMatrix();


    glPushMatrix(); 
    glColor3f(0.03, 0.03, 0.03);
    drawRect(background_x, background_y2, WINDOW_WIDTH, WINDOW_HEIGHT);

    for(int i = 0; i < 10; ++i) {
        glColor3f(1.0, 0.5, 1.0);
        drawCircle(random(0, WINDOW_WIDTH), random(0, WINDOW_HEIGHT), 1.5);
    }

    glPopMatrix();
}

void draw_ship() {
    if(ship->is_moving_right) {
        glPushMatrix(); 
        glTranslatef((ship->x + (ship->width / 2)), (ship->y + (ship->height / 2)), 1);
        glRotatef(-ANGEL, 0, 0, 1);
        glTranslatef(-(ship->x + (ship->width / 2)), -(ship->y + (ship->height / 2)), -1);
        ship->draw();
        glPopMatrix();
    } else if(ship->is_moving_left) {
        glPushMatrix();
        glTranslatef((ship->x + (ship->width / 2)), (ship->y + (ship->height / 2)), 1); 
        glRotatef(ANGEL, 0, 0, 1);
        glTranslatef(-(ship->x + (ship->width / 2)), -(ship->y + (ship->height / 2)), -1);
        ship->draw();
        glPopMatrix();
    } else {
        ship->draw();
    }
}

void new_enemy() {
    if (enemy-> health <= 0) {
        enemy_dead = true;
        new_enemy_timer += 10;
        if(new_enemy_timer == 300) {
            enemy = new Enemy(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 200, 100, 100, HEALTH(level), 0);
            collider = new Collider(ship_bullets, enemy, enemy_defender);
            enemy_dead = false;
            isleveledUp = false;
            new_enemy_timer = 0;
        }
    }

    if(enemy_dead && !isleveledUp) {
        level++;
        isleveledUp = true;
    }
}

void enemy_drawing() {

    // foot
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f); 
    drawRect(0, 0, 100, 10);
    drawCircle(0, 5, 5);
    drawCircle(100, 5, 5);

    glColor3f(0.0f, 1.0f, 1.0f); 
    drawCircle(10, 5, 5);
    drawCircle(30, 5, 5);
    drawCircle(50, 5, 5);
    drawCircle(70, 5, 5);
    drawCircle(90, 5, 5);

    glPopMatrix();

    // Body
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f); 
    drawRect(10, 10, 80, 70);

    glColor3f(0.0f, 1.0f, 0.0f); 
    drawCircle(22, 20, 7);
    drawCircle(78, 20, 7);

    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(23, 20, 3);
    drawCircle(77, 20, 3);

    glColor3f(0.0f, 0.7f, 0.5f);
    drawRect(40, 15, 25, 10);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(37, 35, 5);
    drawCircle(52, 35, 5);
    drawCircle(67, 35, 5);
    glPopMatrix();

    // Head
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f); 
    drawRect(20, 80, 60, 35);

    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(30, 100, 7);
    drawCircle(70, 100, 7);

    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    drawRect(85, 60, 35, 10);
    drawRect(115, 60, 10, 35);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    drawRect(10, 60, -35, 10);
    drawRect(-25, 60, 10, 35);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(25.0f, 80.0f, 0.0f);
        glVertex3f(80.0f, 80.0f, 0.0f);
        glVertex3f(50.0f, 90.0f, 0.0f);
    glEnd();
    glPopMatrix();
}

int* bezier(float t, int* p0,int* p1,int* p2,int* p3) {
    int res[2];
    res[0]=pow((1-t),3)*p0[0]+3*t*pow((1-t),2)*p1[0]+3*pow(t,2)*(1-t)*p2[0]+pow(t,3)*p3[0];
    res[1]=pow((1-t),3)*p0[1]+3*t*pow((1-t),2)*p1[1]+3*pow(t,2)*(1-t)*p2[1]+pow(t,3)*p3[1];
    return res;
}

void init_bezier() {
    p0[0]=WINDOW_WIDTH / 10.0;
    p0[1]=WINDOW_WIDTH / 4.0 + 100;
    
    p1[0]=WINDOW_WIDTH / 10.0;
    p1[1]=WINDOW_WIDTH / 4.0 + 400;
    
    p2[0]=WINDOW_WIDTH / 10.0 + 800;
    p2[1]=WINDOW_WIDTH / 4.0 + 400;
    
    p3[0]=WINDOW_WIDTH / 10.0 + 800;
    p3[1]=WINDOW_WIDTH / 4.0 + 100;
}

void new_bezier() {
    p1[0]=random(50, WINDOW_WIDTH - 50);
    p3[0]=random(50, WINDOW_WIDTH - 50);
    p1[1]=random(WINDOW_HEIGHT / 2, WINDOW_HEIGHT - enemy->height);
    p2[1]=random(WINDOW_HEIGHT / 2, WINDOW_HEIGHT - enemy->height);
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
    
    // glColor3f(.26,.32,.77);

    glColor3f(1, 1, 1);
    
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

int random(int n, int m) {
    return rand() % (m - n + 1) + n;
}

void playSound(string filename) {

}
