#include"Engine.h"

Engine::Engine(int screenSizeX, int screenSizeY){
    window = new Window(screenSizeX,screenSizeY);
    renderWindow = window->getRenderWindow();
    mapTree.loadFrom("../test/mapDataSimple.txt");
    player = new Player(350,350,0,3,1.75);
    screenRenderer = new ScreenRenderer(renderWindow,player,screenSizeX,screenSizeY,mapTree.getSectorData());
}

void Engine::eventHandler(){
    window->eventHandler();
    SDL_Event keyE;
    while(SDL_PollEvent(&keyE)){
        const Uint8* currentKeyState = SDL_GetKeyboardState(NULL);
        switch(keyE.type){
            case SDL_KEYDOWN:
                keyHandler.setKeyDown(keyE.key.keysym.sym);
                break;
            case SDL_KEYUP:
                keyHandler.setKeyRelease(keyE.key.keysym.sym);
                break;
        }
        if((!keyHandler.isPressed(SDLK_d) && !keyHandler.isPressed(SDLK_a)) ||
            (keyHandler.isPressed(SDLK_d) && keyHandler.isPressed(SDLK_a))){
            player->setHorizontalXVel(0);
        }else{
            if(keyHandler.isPressed(SDLK_d)){
                player->setHorizontalXVel(1);
            }
            if(keyHandler.isPressed(SDLK_a)){
                player->setHorizontalXVel(-1);
            }
        }
        if(!keyHandler.isPressed(SDLK_w) && !keyHandler.isPressed(SDLK_s) ||
            (keyHandler.isPressed(SDLK_w) && keyHandler.isPressed(SDLK_s))){
            player->setHorizontalYVel(0); // Vertically velocity relative to the player's facing direction
        }else{
            if(keyHandler.isPressed(SDLK_w)){
                player->setHorizontalYVel(1);
            }
            if(keyHandler.isPressed(SDLK_s)){
                player->setHorizontalYVel(-1);
            }
        }
        if(!keyHandler.isPressed(SDLK_SPACE) && !keyHandler.isPressed(SDLK_LSHIFT) ||
            (keyHandler.isPressed(SDLK_SPACE) && keyHandler.isPressed(SDLK_LSHIFT))){
            player->setVerticalZVel(0); // Vertically velocity relative to the player's facing direction
        }else{
            if(keyHandler.isPressed(SDLK_SPACE)){
                player->setVerticalZVel(1);
            }
            if(keyHandler.isPressed(SDLK_LSHIFT)){
                player->setVerticalZVel(-1);
            }
        }
        if(!keyHandler.isPressed(SDLK_q) && !keyHandler.isPressed(SDLK_e) ||
            (keyHandler.isPressed(SDLK_q) && keyHandler.isPressed(SDLK_e))){
            player->setTurnDir(0);
        }else{
            if(keyHandler.isPressed(SDLK_q)){
                player->setTurnDir(1);
            }
            if(keyHandler.isPressed(SDLK_e)){
                player->setTurnDir(-1);
            }
        }
    
        if(keyHandler.hasKeyEvent()){
            player->setHorizontalXVel(0);
            player->setHorizontalYVel(0);
            player->setVerticalZVel(0);
            player->setTurnDir(0);
        }
    }
}

void Engine::renderEngine(){
    SDL_RenderClear(renderWindow);
    SDL_SetRenderDrawColor(renderWindow,255,255,255,255);

    screenRenderer->renderFOV(mapTree.getRenderOrder(player));
    //screenRenderer->renderTopDown(mapTree.getRenderOrder(player));

    SDL_SetRenderDrawColor(renderWindow,0,0,0,255);
    SDL_RenderPresent(renderWindow);
}

void Engine::updateEngine(){
    player->movementHandler();
}

void Engine::clean(){
    window->clean();
}

bool Engine::isRunning(){
    return window->isRunning();
}

void Engine::startEngineLoop(){
    Uint32 frameStart;
    Uint32 frameEnd;
    int frameTime;

    const int FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint32 countedFrames = 0;
    Uint32 totalFrames = 0;
    float currentFPS = 0;
    
    while(isRunning()){
        frameStart = SDL_GetTicks();
        totalFrames++;

        eventHandler();
        renderEngine();
        updateEngine();
        
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
    clean();
}
