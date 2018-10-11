//
//  alien.hpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#ifndef alien_hpp
#define alien_hpp

#include "Vector2d.hpp"
#include "enemybullet.hpp"
#include <vector>

class Alien {
    
public:
    enum Direction {LEFT, RIGHT};
    static Vector2d speedPerSec;
    
private:
    static const int MAX_BULLETS = 5;
    constexpr static const Scalar INITIAL_SPEED_X = 40;
    constexpr static const Scalar INITIAL_SPEED_Y = 0;
    static const Vector2d DROP_DOWN_MOVEMENT;
    constexpr static const Scalar HIT_WALL_SPEEDUP = -1.2;
    constexpr static const Scalar BODY_WIDTH = 40;
    constexpr static const Scalar BODY_HEIGHT = 20;
    static const GLfloat COLOR[3];
    std::vector<EnemyBullet> bullets;
    bool alive;
    Direction direction;
    Rect2d body;
    
public:
    Alien();
    Alien(Scalar ulx, Scalar uly);
    void update(int elapsedTime);
    bool isAlive() const;
    bool hit(const Rect2d &other);
    bool shootBullet();
    bool updateBullets(int elapsedTime);
    void kill();
    void draw() const;
    bool goingToHitWall(int elapsedTime);
    void dropDown();
    static void hitWall();
    static void resetSpeed();
    Alien& operator=(const Alien& a);
    const Rect2d& getBody() const;
};


#endif /* alien_hpp */
