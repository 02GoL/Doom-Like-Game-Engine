#include<iostream>
#include"window.h"

using namespace std;

Window* frame = NULL;

int main(int argc, char* argv[]){
    Uint32 frameStart;
    Uint32 frameEnd;
    int frameTime;

    const int FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint32 countedFrames = 0;
    Uint32 totalFrames = 0;
    float currentFPS = 0;

    frame = new Window();
    frame->init(700,700);
    
    while(frame->running()){
        frameStart = SDL_GetTicks();
        totalFrames++;

        frame->eventHandler();
        frame->render();
        frame->update();
        
        frameTime = SDL_GetTicks()-frameStart;
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay-frameTime);    
        }
        
        frameEnd = SDL_GetTicks();
        
        currentFPS = (frameEnd-frameStart)/1000.0f;
        countedFrames += frameEnd-frameStart;
        //cout << "Current FPS: " << to_string(1.0f/currentFPS) << " | ";
        //cout << "Average FPS: " << to_string(1000.0f/((float)countedFrames/totalFrames)) << "\n";
    }
    frame->clean();

    return 0;
}
