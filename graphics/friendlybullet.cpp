//
//  friendlybullet.cpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#include "friendlybullet.hpp"

Vector2d FriendlyBullet::speedPerSec = Vector2d(INITIAL_SPEED_X, INITIAL_SPEED_Y);
const GLfloat FriendlyBullet::COLOR[3] = {1.0, 1.0, 0.0};

FriendlyBullet::FriendlyBullet(Scalar llx, Scalar lly) : Bullet(llx, lly) {}

bool FriendlyBullet::update(int elapsedTime) {
    rect = translate(rect, speedPerSec*elapsedTime/1000);
    if (rect.getLowerLeft().y > 1000) {
        kill();
        return false;
    }
    return true;
}


void FriendlyBullet::draw() const {
    drawRectangle(rect, COLOR);
}
