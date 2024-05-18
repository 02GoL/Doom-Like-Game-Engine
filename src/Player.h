#include"Util.h"
#include"KeyHandler.h"

class Player{
    private:
        KeyHandler keyHandler;
        SDL_FRect playerSprite;
        SDL_FRect directionLine;
        int verticalVel;
        int horizontalVel;
        int turnDir;
        float screenSize;
        float fov;
        float dx;
        float dy;
        float moveSpeed;
        float turnRate;
        float angle;
    public:
        Player(int x,int y, float angle, float turnRate, float moveSpeed);
        void inputHandler();
        void movementHandler();
        void setMoveSpeed(float moveSpeed);
        void setTurnRate(float turnRate);
        void setAngle(float angle);
        float getAngle(){return angle;}
        float getFov(){return fov;}
        Point getPosition();
        SDL_FRect getPlayerSprite(){return playerSprite;}
};