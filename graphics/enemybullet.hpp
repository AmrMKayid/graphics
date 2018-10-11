//
//  enemybullet.hpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#ifndef enemybullet_hpp
#define enemybullet_hpp

#include "bullet.hpp"

class EnemyBullet : public Bullet {
private:
    constexpr static const Scalar INITIAL_SPEED_X = 0;
    constexpr static const Scalar INITIAL_SPEED_Y = -400;
    static const GLfloat COLOR[3];
    static Vector2d speedPerSec;
    
public:
    EnemyBullet(Scalar llx, Scalar lly);
    bool update(int elapsedTime);
    void draw() const;
    
};

#endif /* enemybullet_hpp */
