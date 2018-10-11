//
//  bullet.hpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#ifndef bullet_hpp
#define bullet_hpp

#include "Vector2d.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

class SpaceShip;

class Bullet {
    /* Private variables */
private:
    constexpr static const Scalar INITIAL_WIDTH = 5;
    constexpr static const Scalar INITIAL_HEIGHT = 10;
    
protected:
    Rect2d rect;
    bool alive;
    
    /* Public methods */
public:
    Bullet(Scalar llx, Scalar lly);
    Bullet(const Point2d &ll);
    bool isAlive() const;
    bool hit(const Rect2d& other) const;
    bool kill();
    virtual bool update(int ElapsedTime) = 0;
    virtual void draw() const = 0;
};

#endif /* bullet_hpp */
