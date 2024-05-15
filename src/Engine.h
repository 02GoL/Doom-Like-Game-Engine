#include<iostream>
#include<SDL2/SDL.h>
#include"Player.h"
#include"Maptree.h"

using namespace std;

class Engine{
    private:
        SDL_Renderer* renderWindow;
        Player* player;
        MapTree mapTree;
        bool b = false;
    public:
        void engineInit(SDL_Renderer* renderWindow);
        void inputHandler();
        void renderGame();
        void movementHandler();
        void renderMap();
        void renderPlayer();
};