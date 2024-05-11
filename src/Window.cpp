#include"window.h"

void Window::init(int xDim,int yDim){
    this->xDim = xDim;
    this->yDim = yDim;

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
        gameEngine.engineInit(renderer);

        isRunning = true;
    }else{
        isRunning = false;
    }
}

void Window::eventHandler(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            isRunning = false;
        }
        SDL_PushEvent(&event);
    }
    gameEngine.inputHandler();
}

void Window::render(){
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    gameEngine.renderGame();

    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderPresent(renderer);
}

void Window::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "System Quit...\n";
}

void Window::update(){
    gameEngine.movementHandler();
}

bool Window::running(){
    return isRunning;
}