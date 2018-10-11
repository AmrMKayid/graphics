//
//  spaceship.hpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#ifndef spaceship_hpp
#define spaceship_hpp

#include "friendlybullet.hpp"
#include <vector>
#include <ctime>

class SpaceShip
{
private:
    static const int RATE_OF_FIRE = 40; //milliseconds
    static const int MAX_BULLETS = 8;
    constexpr static const Scalar BODY_WIDTH = 50;
    constexpr static const Scalar BODY_HEIGHT = 20;
    constexpr static const Scalar CANON_WIDTH = 5;
    constexpr static const Scalar CANON_HEIGHT = 10;
    constexpr static const Scalar INITIAL_X = 475;
    constexpr static const Scalar INITIAL_Y = 100;
    constexpr static const Scalar RIGHT_STEP_SCALAR = 20;
    static const GLfloat COLOR[3];
    Rect2d body;
    Rect2d cannon;
    std::vector<FriendlyBullet> bullets;
    int lastTime;
    bool alive;
    
public:
    SpaceShip();
    SpaceShip& operator=(const SpaceShip& s);
    bool isAlive() const;
    bool hit(const Rect2d &other);
    bool moveLeft();
    bool moveRight();
    bool shootBullet();
    void updateBullets(int elapsedTime);
    void kill();
    void draw() const;
    void reset();
    const Rect2d& getBody() const;
    const Rect2d& getCannon() const;
};


#endif /* spaceship_hpp */
