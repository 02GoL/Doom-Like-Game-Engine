#include<iostream>
#include<vector>
#include<math.h>
#include<SDL2/SDL.h>

using namespace std;

class Player{
    private:
        SDL_Rect playerSprite;
        SDL_Rect directionLine;
        int moveSpeed;
        int turnRate;
        int velocity;
        int sideOffset;
        int dx;
        int dy;
        int turnDir;
        float angle;
    public:
        void playerInit(int x, int y);
        void renderPlayer(SDL_Renderer *renderWindow);
        void inputHandler();
        void movementHandler();
        void setX();
};