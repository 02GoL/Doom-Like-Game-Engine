#include<iostream>
#include<SDL2/SDL.h>

using namespace std;

class Window{
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        const char* title = "Engine Window";
        bool running;
    public: 
        Window(int xDim, int yDim);
        void eventHandler();
        void clean();
        bool isRunning();
        SDL_Renderer* getRenderWindow();
};