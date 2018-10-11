//
//  friendlybullet.hpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#ifndef friendlybullet_hpp
#define friendlybullet_hpp

#include "bullet.hpp"

class FriendlyBullet : public Bullet {
private:
    constexpr static const Scalar INITIAL_SPEED_X = 0;
    constexpr static const Scalar INITIAL_SPEED_Y = 1500;
    static const GLfloat COLOR[3];
    static Vector2d speedPerSec;
    
public:
    FriendlyBullet(Scalar llx, Scalar lly);
    bool update(int elapsedTime);
    void draw() const;
    
};

#endif /* friendlybullet_hpp */
