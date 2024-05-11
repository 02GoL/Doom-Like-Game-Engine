#include<iostream>
#include<SDL2/SDL.h>
#include"Engine.cpp"


using namespace std;

class Window{
    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        Engine gameEngine;
        const char *title = "Raycasting";
        int xDim;
        int yDim;
        bool isRunning;
    public: 
        void init(int x, int y);
        void eventHandler();
        void render();
        void clean();
        void update();
        bool running();
};
