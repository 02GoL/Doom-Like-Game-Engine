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
}

void Player::inputHandler(){
    SDL_Event keyE;
    while(SDL_PollEvent(&keyE)){
        const Uint8* currentKeyState = SDL_GetKeyboardState(NULL);
        switch(keyE.type){
            case SDL_KEYDOWN:
                keyHandler.setKeyDown(keyE.key.keysym.sym);
                break;
            case SDL_KEYUP:
                keyHandler.setKeyRelease(keyE.key.keysym.sym);
                break;
        }
        if(!keyHandler.isPressed(SDLK_w) && !keyHandler.isPressed(SDLK_s) ||
            (keyHandler.isPressed(SDLK_w) && keyHandler.isPressed(SDLK_s))){
            verticalVel = 0; // Vertically velocity relative to the player's facing direction
        }else{
            if(keyHandler.isPressed(SDLK_w)){
                verticalVel = 1;
            }
            if(keyHandler.isPressed(SDLK_s)){
                verticalVel = -1;
            }
        }
        if((!keyHandler.isPressed(SDLK_a) && !keyHandler.isPressed(SDLK_d)) ||
            (keyHandler.isPressed(SDLK_a) && keyHandler.isPressed(SDLK_d))){
            horizontalVel = 0;
        }else{
            if(keyHandler.isPressed(SDLK_a)){
                horizontalVel = -1;
            }
            if(keyHandler.isPressed(SDLK_d)){
                horizontalVel = 1;
            }
        }
        if(!keyHandler.isPressed(SDLK_q) && !keyHandler.isPressed(SDLK_e) ||
            (keyHandler.isPressed(SDLK_q) && keyHandler.isPressed(SDLK_e))){
            turnDir = 0;
        }else{
            if(keyHandler.isPressed(SDLK_q)){
                turnDir = 1;
            }
            if(keyHandler.isPressed(SDLK_e)){
                turnDir = -1;
            }
        }
    
        if(keyHandler.hasKeyEvent()){
            verticalVel = 0;
            horizontalVel = 0;
            turnDir = 0;
        }
    }
}

void Player::movementHandler(){
    if(turnDir == 1){
        angle = normalizeAngle(angle-turnRate);
    }else if(turnDir == -1){
        angle = normalizeAngle(angle+turnRate);   
    }

    dx = moveSpeed*(verticalVel*cos(angle)+horizontalVel*cos(angle+PI/2));
    dy = moveSpeed*(verticalVel*sin(angle)+horizontalVel*sin(angle+PI/2));

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
    Point playerPosition(playerSprite.x+2,playerSprite.y+2,0);
    return playerPosition;
}