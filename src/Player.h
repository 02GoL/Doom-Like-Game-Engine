#pragma once
#include"Util.h"
#include"KeyHandler.h"

class Player{
    private:
        KeyHandler keyHandler;
        SDL_FRect playerSprite;
        SDL_FRect directionLine;
        int horizontalXVel;
        int horizontalYVel;
        int verticalZVel;
        int turnDir;
        float angle;
        float verticalHeight;
        float screenSize;
        float fov;
        float dx;
        float dy;
        float moveSpeed;
        float turnRate;
    public:
        Player(int x,int y, float angle, float turnRate, float moveSpeed);
        void movementHandler();
        void setMoveSpeed(float moveSpeed);
        void setTurnRate(float turnRate);
        void setAngle(float angle);
        void setHorizontalXVel(int horizontalXVel){this->horizontalXVel = horizontalXVel;}
        void setHorizontalYVel(int horizontalYVel){this->horizontalYVel = horizontalYVel;}
        void setVerticalZVel(int verticalZVel){this->verticalZVel = verticalZVel;}
        void setTurnDir(int turnDir){this->turnDir = turnDir;}
        float getAngle(){return angle;}
        float getFov(){return fov;}
        float getVerticalHeight(){return verticalHeight;}
        Point getPosition();
        SDL_FRect getPlayerSprite(){return playerSprite;}
};