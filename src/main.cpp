#include<iostream>
#include"window.h"

using namespace std;

Window *frame = nullptr;

int main(int argc, char* argv[]){
    Uint32 frameStart;
    int frameTime;

    const int FPS = 60;
    const int frameDelay = 1000/FPS;

    frame = new Window();
    frame->init(SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
    
    while(frame->running()){
        frameStart = SDL_GetTicks();
        
        frame->eventHandler();
        frame->render();
        frame->update();
        
        frameTime = SDL_GetTicks()-frameStart;
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay-frameTime);
        }
    }
    frame->clean();

    return 0;
}
