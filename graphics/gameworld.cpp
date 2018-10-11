//
//  gameworld.cpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#include "gameworld.hpp"
#include "Utilities.hpp"
#include <cstdlib>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#include <iostream>

const char GameWorld::WIN_MESSAGE[] = "You win! You killed all the aliens!";
const char GameWorld::LOSE_MESSAGE[] = "You lost! You got hit!";

GameWorld::GameWorld() {
    init();
}


void GameWorld::init() {
    playerShip = SpaceShip();
    alienDirection = Alien::RIGHT;
    Alien::resetSpeed();
    Scalar startX = 130;
    Scalar stepX = 100;
    Scalar startY = 1000 - (75-20)/2.0;
    Scalar stepY = 75;
    Scalar currX;
    Scalar currY = startY;
    for (int row = 0; row < ALIEN_ROWS; ++row) {
        currX = startX;
        for (int col = 0; col < ALIENS_PER_ROW; ++col) {
            aliens[row][col] = Alien(currX, currY);
            currX += stepX;
        }
        currY -= stepY;
    }
    winner = NONE;
    gameState = IN_GAME;
    resetTime();
}

void GameWorld::updateState() {
    if (inGame()) {
        int time2 = getMilliCount();
        int elapsed = time2 - nTime;
        nTime = time2;
        playerShip.updateBullets(elapsed);
        updateAliens(elapsed);
        randomlyShootLasers();
        checkForCollisions();
        if (checkGameOver()) {
            gameState = GAME_OVER;
        }
    }
}

bool GameWorld::checkGameOver() {
    if (gameState == GAME_OVER) {
        return true;
    }
    if (!playerShip.isAlive()) {
        winner = COMPUTER;
        return true;
    }
    for (int row = 0; row < ALIEN_ROWS; ++row) {
        for (int col = 0; col < ALIENS_PER_ROW; ++col) {
            if (aliens[row][col].isAlive()) {
                return false;
            }
        }
    }
    winner = PLAYER;
    return true;
}

void GameWorld::updateAliens(int elapsed) {
    int col;
    if (alienDirection == Alien::RIGHT) {
        col = getLastNonEmptyCol();
    } else {
        col = getFirstNonEmptyCol();
    }
    for (int row = 0; row < ALIEN_ROWS; ++row) {
        if (aliens[row][col].isAlive()) {
            if (aliens[row][col].goingToHitWall(elapsed)) {
                for (int row2 = 0; row2 < ALIEN_ROWS; ++row2) {
                    for (int col2 = 0; col2 < ALIENS_PER_ROW; ++col2) {
                        if (aliens[row2][col2].isAlive()) {
                            aliens[row2][col2].dropDown();
                        }
                    }
                }
                Alien::hitWall();
                if (alienDirection == Alien::RIGHT) {
                    alienDirection = Alien::LEFT;
                } else {
                    alienDirection = Alien::RIGHT;
                }
            } else {
                for (int row2 = 0; row2 < ALIEN_ROWS; ++row2) {
                    for (int col2 = 0; col2 < ALIENS_PER_ROW; ++col2) {
                        if (aliens[row2][col2].isAlive()) {
                            aliens[row2][col2].update(elapsed);
                        }
                    }
                }
            }
            break;
        }
    }
    for (int row = 0; row < ALIEN_ROWS; ++row) {
        for (int col = 0; col < ALIENS_PER_ROW; ++col) {
            aliens[row][col].updateBullets(elapsed);
        }
    }
}

void GameWorld::checkForCollisions() {
    for (int row = 0; row < ALIEN_ROWS; ++row) {
        for (int col = 0; col < ALIENS_PER_ROW; ++col) {
            //Tie goes to the player
            if (aliens[row][col].isAlive()) {
                if (playerShip.hit(aliens[row][col].getBody())) {
                    aliens[row][col].kill();
                }
            }
            if (aliens[row][col].hit(playerShip.getBody()) || aliens[row][col].hit(playerShip.getCannon())) {
                playerShip.kill();
                return;
            }
        }
    }
}

int GameWorld::getLastNonEmptyCol() const {
    for (int col = ALIENS_PER_ROW-1; col >= 0; --col) {
        for (int row = 0; row < ALIEN_ROWS; ++row) {
            if (aliens[row][col].isAlive()) {
                return col;
            }
        }
    }
    return -1;
}

int GameWorld::getFirstNonEmptyCol() const {
    for (int col = 0; col < ALIENS_PER_ROW; ++col) {
        for (int row = 0; row < ALIEN_ROWS; ++row) {
            if (aliens[row][col].isAlive()) {
                return col;
            }
        }
    }
    return -1;
}

bool GameWorld::reset() {
    init();
    return true;
}

void GameWorld::drawAllObjects() const {
    for (int row = 0; row < ALIEN_ROWS; ++row) {
        for (int col = 0; col < ALIENS_PER_ROW; ++col) {
            aliens[row][col].draw();
        }
    }
    playerShip.draw();
}

void GameWorld::processKeyPress(unsigned char c, int x, int y) {
    switch (c) {                                // c is the key that is hit
        case 'q':                               // 'q' means quit
            exit(0);
            break;
        case 'r':
            reset();
            break;
        case 'a':
            playerShip.moveLeft();
            break;
        case 'd':
            playerShip.moveRight();
            break;
    }
}

void GameWorld::processMouse(int b, int s, int x, int y) {
    if (s == GLUT_DOWN) {
        if (b == GLUT_LEFT_BUTTON) {
            playerShip.shootBullet();
        }
    }
}

GameWorld::Winner GameWorld::getWinner() const {
    return winner;
}

bool GameWorld::inGame() const {
    return gameState == IN_GAME;
}

bool GameWorld::gameOver() const {
    return gameState == GAME_OVER;
}

void GameWorld::drawGame() const {
    if (inGame()) {
        glClearColor(0.0, 0.0, 0.0, 1.0);           // background is black
        glClear(GL_COLOR_BUFFER_BIT);               // clear the window
        drawAllObjects();
    } else if (gameOver()) {
        glClearColor(1.0, 1.0, 1.0, 1.0);            // background is white
        glClear(GL_COLOR_BUFFER_BIT);                // clear the window
        if (winner == PLAYER) {
            glRasterPos2i(250, 500);
            static GLfloat BLUE_RGB[] = {0.0, 0.0, 1.0};
            glColor3fv(BLUE_RGB);
            for (int i = 0; i < sizeof(WIN_MESSAGE); ++i) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, WIN_MESSAGE[i]);
            }
        } else {
            glRasterPos2i(350, 500);
            static GLfloat RED_RGB[] = {1.0, 0.0, 0.0};
            glColor3fv(RED_RGB);
            for (int i = 0; i < sizeof(LOSE_MESSAGE); ++i) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, LOSE_MESSAGE[i]);
            }
        }
    }
}

void GameWorld::randomlyShootLasers() {
    for (int row = 0; row < ALIEN_ROWS; ++row) {
        for (int col = 0; col < ALIENS_PER_ROW; ++col) {
            if (aliens[row][col].isAlive()) {
                double randNum = unifRand(0, 100);
                if (randNum < 0.1) {
                    aliens[row][col].shootBullet();
                }
            }
        }
    }
}

void GameWorld::resetTime() {
    nTime = getMilliCount();
}
