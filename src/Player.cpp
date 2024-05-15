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
        angle -= turnRate;
        if(angle < 0){
            angle += 2*M_PI;
        }
    }else if(turnDir == -1){
        angle += turnRate;
        if(angle >= 2*M_PI){
            angle -= 2*M_PI;
        }      
    }

    dx = moveSpeed*(verticalVel*cos(angle)+horizontalVel*cos(angle+M_PI/2));
    dy = moveSpeed*(verticalVel*sin(angle)+horizontalVel*sin(angle+M_PI/2));

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
    Point playerPosition;
    playerPosition.x = playerSprite.x;
    playerPosition.y = playerSprite.y;
    return playerPosition;
}

bool Player::inFOV(Vector* vector){
    Point facingPoint;
    facingPoint.x = playerSprite.x+cos(angle);
    facingPoint.y = playerSprite.y+sin(angle);
    facingPoint = getIntersection(vector,getPosition(),facingPoint);

    float theta = atan2(facingPoint.y-playerSprite.y,facingPoint.x-playerSprite.x);
    float alpha = atan2(vector->p1.y-playerSprite.y,vector->p1.x-playerSprite.x);
    float beta = atan2(vector->p2.y-playerSprite.y,vector->p2.x-playerSprite.x);
    if(theta < 0){
        theta += 2*M_PI;
    }
    if(alpha < 0){
        alpha += 2*M_PI;
    }
    if(beta < 0){
        beta += 2*M_PI;
    }

    float upper = angle+fov;
    float lower = angle-fov;
    if(upper > 2*M_PI){
        upper -= 2*M_PI;
    }
    if(lower < 0){
        lower += 2*M_PI;
    }

    if(((facingPoint.x >= vector->p1.x-eps && facingPoint.x <= vector->p2.x+eps) || 
    (facingPoint.x >= vector->p2.x-eps && facingPoint.x <= vector->p1.x+eps)) &&
    ((facingPoint.y >= vector->p1.y-eps && facingPoint.y <= vector->p2.y+eps) || 
    (facingPoint.y >= vector->p2.y-eps && facingPoint.y <= vector->p1.y+eps)) &&
    (theta <= angle+eps && theta >= angle-eps)){
        return true;
    }
    if(angle < fov || angle > 2*M_PI-fov){
        if(alpha <= upper || alpha >= lower || beta <= upper || beta >= lower){
            return true;
        }
    }else if((alpha <= upper && alpha >= lower) || (beta <= upper && beta >= lower)){
        return true;
    }
    return false;
}