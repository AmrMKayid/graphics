//
//  alien.cpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#include "alien.hpp"

Vector2d Alien::speedPerSec = Vector2d(INITIAL_SPEED_X, INITIAL_SPEED_Y);
const Vector2d Alien::DROP_DOWN_MOVEMENT = Vector2d(0, -30);
const GLfloat Alien::COLOR[3] = {1.0, 0.0, 0.0};

Alien::Alien() : body(), bullets(), direction(RIGHT), alive(false) {}

Alien::Alien(Scalar ulx, Scalar uly) : body(Point2d(ulx, uly-BODY_HEIGHT), Point2d(ulx+BODY_WIDTH, uly)),
bullets(),
direction(RIGHT),
alive(true) {}



Alien& Alien::operator=(const Alien& a) {
    body = a.body;
    bullets = a.bullets;
    direction = a.direction;
    alive = a.alive;
    return *this;
}

bool Alien::goingToHitWall(int elapsedTime) {
    Rect2d newBody = translate(body, speedPerSec*elapsedTime/1000);
    if (direction == LEFT) {
        if (newBody.getLowerLeft().x < 0) {
            return true;
        } else {
            return false;
        }
    } else {
        if (newBody.getUpperRight().x > 1000) {
            return true;
        } else {
            return false;
        }
    }
}

void Alien::dropDown() {
    body = translate(body, DROP_DOWN_MOVEMENT);
    /* Swap Direction */
    if (direction == LEFT) {
        direction = RIGHT;
    } else {
        direction = LEFT;
    }
}

void Alien::hitWall() {
    speedPerSec *= HIT_WALL_SPEEDUP;
}

void Alien::update(int elapsedTime) {
    body = translate(body, speedPerSec*elapsedTime/1000);
}

bool Alien::isAlive() const {
    return alive;
}

bool Alien::hit(const Rect2d &other) {
    if (body.overlaps(other)) {
        return true;
    }
    for (int i = 0; i < bullets.size(); ++i) {
        if (bullets[i].isAlive()) {
            if (bullets[i].hit(other)) {
                bullets.erase(bullets.begin() + i);
                return true;
            }
        }
    }
    return false;
}

bool Alien::shootBullet() {
    if (bullets.size() < MAX_BULLETS) {
        bullets.push_back(EnemyBullet(body.getLowerLeft().x+(BODY_WIDTH+5)/2, body.getLowerLeft().y));
        return true;
    }
    return false;
}

bool Alien::updateBullets(int elapsedTime) {
    for (int i = 0; i < bullets.size(); ++i) {
        if (bullets[i].isAlive()) {
            if (!bullets[i].update(elapsedTime)) {
                bullets.erase(bullets.begin() + i);
            }
        }
    }
    return true;
}

void Alien::kill() {
    alive = false;
}

void Alien::draw() const {
    if (isAlive()) {
        drawRectangle(body, COLOR);
    }
    for (int i = 0; i < bullets.size(); ++i) {
        if (bullets[i].isAlive()) {
            bullets[i].draw();
        }
    }
}

const Rect2d& Alien::getBody() const {
    return body;
}

void Alien::resetSpeed() {
    speedPerSec = Vector2d(INITIAL_SPEED_X, INITIAL_SPEED_Y);
}
