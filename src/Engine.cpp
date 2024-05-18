#include"Engine.h"

Engine::Engine(float screenSizeX, float screenSizeY){
    window = new Window(screenSizeX,screenSizeY);
    this->renderWindow = window->getRenderWindow();
    player = new Player(350,350,90,3,1.75);
    fovRenderer = new FOVRenderer(renderWindow,screenSizeX,screenSizeY,90);
    mapTree.loadFrom("../test/mapDataSimple.txt");
}

void Engine::eventHandler(){
    window->eventHandler();
    player->inputHandler();
}

void Engine::renderEngine(){
    SDL_RenderClear(renderWindow);
    SDL_SetRenderDrawColor(renderWindow,255,255,255,255);

    fovRenderer->renderFOV(mapTree.getRenderOrder(player->getPosition(),player->getAngle()),player->getPosition(),player->getAngle());
    renderTopDown();

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

void Engine::renderTopDown(){
    queue<Vector*> vectorQueue = mapTree.getRenderOrder(player->getPosition(),player->getAngle());
    while(!vectorQueue.empty()){
        if(fovRenderer->inFOV(vectorQueue.front(),player->getPosition(),player->getAngle())){
            SDL_SetRenderDrawColor(renderWindow,210,4,45,255);
        }else{
            SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
        }
        // renders vector
        SDL_RenderDrawLine(renderWindow,vectorQueue.front()->p1.x,vectorQueue.front()->p1.y,
                            vectorQueue.front()->p2.x,vectorQueue.front()->p2.y);
        // renders normal
        SDL_RenderDrawLine(renderWindow,vectorQueue.front()->midPoint.x,vectorQueue.front()->midPoint.y,
                            vectorQueue.front()->normal.x,vectorQueue.front()->normal.y);
        vectorQueue.pop();
    }
    renderPlayer();
}

void Engine::renderPlayer(){
    Point playerPosition = player->getPosition();
    SDL_FRect playerSprite = player->getPlayerSprite();
    float angle = player->getAngle();
    float fov = player->getFov();

    SDL_SetRenderDrawColor(renderWindow,0,255,0,255);
    SDL_RenderFillRectF(renderWindow,&playerSprite);
    
    int x = playerPosition.x+playerSprite.h/2;
    int y = playerPosition.y+playerSprite.w/2;

    SDL_RenderDrawLineF(renderWindow,x,y,x+(200*cos(angle)),y+(200*sin(angle))); // Middle line
    SDL_RenderDrawLineF(renderWindow,x,y,x+(400*cos(angle+fov)),y+(400*sin(angle+fov))); //Upper line
    SDL_RenderDrawLineF(renderWindow,x,y,x+(400*cos(angle-fov)),y+(400*sin(angle-fov))); // lower line
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