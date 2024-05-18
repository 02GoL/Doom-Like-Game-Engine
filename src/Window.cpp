#include"window.h"

Window::Window(int xDim, int yDim){
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        cout << "Initializing...\n";
        window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,xDim,yDim,SDL_WINDOW_SHOWN);
        if(window){
            cout << "Window Created...\n";
        }
        renderer = SDL_CreateRenderer(window,-1,0);
        if(renderer){
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            cout << "Render Created...\n";
        }
        running = true;
    }else{
        running = false;
    }
}

void Window::eventHandler(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            running = false;
        }
        SDL_PushEvent(&event);
    }
}

void Window::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "System Quit...\n";
}

bool Window::isRunning(){
    return running;
}

SDL_Renderer* Window::getRenderWindow(){
    return renderer;
};
