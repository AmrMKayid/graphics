//
//  enemybullet.cpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#include "enemybullet.hpp"

Vector2d EnemyBullet::speedPerSec = Vector2d(INITIAL_SPEED_X, INITIAL_SPEED_Y);
const GLfloat EnemyBullet::COLOR[3] = {0.0, 1.0, 0.0};

EnemyBullet::EnemyBullet(Scalar llx, Scalar lly) : Bullet(llx, lly) {}

bool EnemyBullet::update(int elapsedTime) {
    rect = translate(rect, speedPerSec*elapsedTime/1000);
    if (rect.getLowerLeft().y < 0) {
        kill();
        return false;
    }
    return true;
}


void EnemyBullet::draw() const {
    drawRectangle(rect, COLOR);
}
