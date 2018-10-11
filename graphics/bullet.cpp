//
//  bullet.cpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#include "bullet.hpp"

Bullet::Bullet(Scalar llx, Scalar lly) :
rect(Point2d(llx, lly), Point2d(llx+INITIAL_WIDTH,lly+INITIAL_HEIGHT)), alive(true) {}

Bullet::Bullet(const Point2d &ll) :
rect(ll, Point2d(ll.x+INITIAL_WIDTH,ll.y+INITIAL_HEIGHT)), alive(true) {}

bool Bullet::isAlive() const {
    return alive;
}

bool Bullet::hit(const Rect2d& other) const {
    return rect.overlaps(other);
}

bool Bullet::kill() {
    alive = false;
    return alive;
}
