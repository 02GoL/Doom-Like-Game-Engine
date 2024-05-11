#include<iostream>
#include<SDL2/SDL.h>
#include"Engine.h"

using namespace std;

class Window{
    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        Engine gameEngine;
        const char *title = "Engine Window";
        int xDim;
        int yDim;
        bool isRunning;
    public: 
        void init(int xDim, int yDim);
        void eventHandler();
        void render();
        void clean();
        void update();
        bool running();
};