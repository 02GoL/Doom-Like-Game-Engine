#include<iostream>
#include<SDL2/SDL.h>
#include"Player.h"
#include"Maptree.h"
#include"Window.h"
#include"FOVRenderer.h"

using namespace std;

class Engine{
    private:
        Window* window;
        SDL_Renderer* renderWindow;
        Player* player;
        FOVRenderer* fovRenderer;
        MapTree mapTree;

        void eventHandler();
        void renderEngine();
        void updateEngine();
        void renderTopDown();
        void renderFOV();
        void renderPlayer();
        void clean();
        bool isRunning();

    public:
        Engine(float screenSizeX, float screenSizeY);
        void startEngineLoop();
};