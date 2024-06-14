#include"player.h"

Player::Player(int x,int y, float angle, float turnRate, float moveSpeed){
    playerSprite.h = 5;
    playerSprite.w = 5;
    playerSprite.x = x;
    playerSprite.y = y;
    this->fov = toRad(90)/2;
    this->moveSpeed = moveSpeed;
    this->turnRate = toRad(turnRate);
    this->angle = toRad(angle);
    this->verticalHeight = 0;
}

void Player::movementHandler(){
    if(turnDir == 1){
        angle = normalizeAngle(angle-turnRate);
    }else if(turnDir == -1){
        angle = normalizeAngle(angle+turnRate);   
    }

    dx = moveSpeed*(horizontalYVel*cos(angle)+horizontalXVel*cos(angle+PI/2));
    dy = moveSpeed*(horizontalYVel*sin(angle)+horizontalXVel*sin(angle+PI/2));
    verticalHeight += moveSpeed*verticalZVel;

    playerSprite.x += dx;
    playerSprite.y += dy;
}

void Player::setMoveSpeed(float moveSpeed){
    this->moveSpeed = moveSpeed;
}

void Player::setTurnRate(float turnRate){
    this->turnRate = turnRate;
}

void Player::setAngle(float angle){
    this->angle = angle;
}

Point Player::getPosition(){
    Point playerPosition(playerSprite.x+2,playerSprite.y+2);
    return playerPosition;
}