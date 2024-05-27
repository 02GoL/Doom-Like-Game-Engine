#include<iostream>
#include<SDL2/SDL.h>
#include"Player.h"
#include"Maptree.h"
#include"Window.h"
#include"ScreenRenderer.h"

using namespace std;

class Engine{
    private:
        Window* window;
        SDL_Renderer* renderWindow;
        Player* player;
        ScreenRenderer* screenRenderer;
        MapTree mapTree;

        void eventHandler();
        void renderEngine();
        void updateEngine();
        void clean();
        bool isRunning();

    public:
        Engine(int screenSizeX, int screenSizeY);
        void startEngineLoop();
};