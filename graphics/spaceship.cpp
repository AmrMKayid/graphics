//
//  spaceship.cpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#include "spaceship.hpp"
#include "Utilities.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

const GLfloat SpaceShip::COLOR[3] = {0.0, 0.0, 1.0};

SpaceShip::SpaceShip() : body(Point2d(INITIAL_X, INITIAL_Y), Point2d(INITIAL_X+BODY_WIDTH, INITIAL_Y+BODY_HEIGHT)),
cannon(
       Point2d(INITIAL_X+(BODY_WIDTH-CANON_WIDTH)/2, INITIAL_Y+BODY_HEIGHT),
       Point2d(INITIAL_X+(BODY_WIDTH+CANON_WIDTH)/2, INITIAL_Y+BODY_HEIGHT+CANON_HEIGHT)
       ),
bullets(),
lastTime(getMilliCount()),
alive(true) {}


SpaceShip& SpaceShip::operator=(const SpaceShip& s) {
    body = s.body;
    cannon = s.cannon;
    bullets = s.bullets;
    lastTime = s.lastTime;
    alive = s.alive;
    return *this;
}

bool SpaceShip::isAlive() const {
    return alive;
}

bool SpaceShip::moveLeft() {
    Rect2d newBody = translate(body, Vector2d(-1*RIGHT_STEP_SCALAR, 0));
    if (newBody.getLowerLeft().x < 0) {
        return false;
    }
    body = newBody;
    cannon = translate(cannon, Vector2d(-1*RIGHT_STEP_SCALAR, 0));
    return true;
}

bool SpaceShip::moveRight() {
    Rect2d newBody = translate(body, Vector2d(RIGHT_STEP_SCALAR, 0));
    if (newBody.getUpperRight().x > 1000) {
        return false;
    }
    body = newBody;
    cannon = translate(cannon, Vector2d(RIGHT_STEP_SCALAR, 0));
    return true;
}

bool SpaceShip::shootBullet() {
    int nTime = getMilliCount();
    if (nTime - lastTime > RATE_OF_FIRE) {
        lastTime = nTime;
        if (bullets.size() < MAX_BULLETS) {
            bullets.push_back(FriendlyBullet(cannon.getLowerLeft().x, cannon.getLowerLeft().y));
            return true;
        }
    }
    return false;
}

void SpaceShip::updateBullets(int elapsedTime) {
    for (int i = 0; i < bullets.size(); ++i) {
        if (bullets[i].isAlive()) {
            if (!bullets[i].update(elapsedTime)) {
                bullets.erase(bullets.begin() + i);
            }
        }
    }
}

bool SpaceShip::hit(const Rect2d& other) {
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

void SpaceShip::kill() {
    alive = false;
}

void SpaceShip::draw() const {
    drawRectangle(body, COLOR);
    drawRectangle(cannon, COLOR);
    for (int i = 0; i < bullets.size(); ++i) {
        if (bullets[i].isAlive()) {
            bullets[i].draw();
        }
    }
}

const Rect2d& SpaceShip::getBody() const {
    return body;
}

const Rect2d& SpaceShip::getCannon() const {
    return cannon;
}
