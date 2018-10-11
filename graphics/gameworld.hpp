//
//  gameworld.hpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#ifndef gameworld_hpp
#define gameworld_hpp

#include "spaceship.hpp"
#include "alien.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

class GameWorld
{
public:
    enum Winner {NONE, PLAYER, COMPUTER};
    
public:
    static const int STARTING_WINDOW_X = 100;
    static const int STARTING_WINDOW_Y = 100;
    constexpr static const GLdouble WINDOW_WIDTH = 1000;
    constexpr static const GLdouble WINDOW_HEIGHT = 1000;
    static const int SUGGESTED_WIDTH = 600;
    static const int SUGGESTED_HEIGHT = 600;
    static const GLint REFRESH_RATE = 10;
    
private:
    static const int ALIEN_ROWS = 4;
    static const int ALIENS_PER_ROW = 8;
    static const char WIN_MESSAGE[];
    static const char LOSE_MESSAGE[];
    Alien aliens[ALIEN_ROWS][ALIENS_PER_ROW];
    Alien::Direction alienDirection;
    enum GameState {IN_GAME, GAME_OVER};
    GameState gameState;
    SpaceShip playerShip;
    enum Winner winner;
    
    int nTime;
    
private:
    void init();
    int getLastNonEmptyCol() const;
    int getFirstNonEmptyCol() const;
    void updateAliens(int elapsed);
    void checkForCollisions();
    bool checkGameOver();
    void randomlyShootLasers();
    
public:
    GameWorld();
    void updateState();
    void drawAllObjects() const;
    bool inGame() const;
    bool gameOver() const;
    void processKeyPress(unsigned char c, int x, int y);
    void processMouse(int b, int s, int x, int y);
    Winner getWinner() const;
    bool reset();
    void drawGame() const;
    void resetTime();
};


#endif /* gameworld_hpp */
