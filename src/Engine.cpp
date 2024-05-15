#include"Engine.h"

void Engine::engineInit(SDL_Renderer* renderWindow){
    this->renderWindow = renderWindow;
    mapTree.loadTree("../test/mapData.txt");
    player = new Player(350,350,0,3,1.75);
}

void Engine::inputHandler(){
    player->inputHandler();
}

void Engine::renderGame(){
    renderMap();
    renderPlayer();
}

void Engine::movementHandler(){
    player->movementHandler();
}

void Engine::renderMap(){
    queue<Vector*> renderQueue = mapTree.getRenderOrder(player->getPosition(), player->getAngle());
    while(!renderQueue.empty()){
        if(player->inFOV(renderQueue.front())){
            SDL_SetRenderDrawColor(renderWindow,210,4,45,255);
        }else{
            SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
        }
        // renders vector
        SDL_RenderDrawLine(renderWindow,renderQueue.front()->p1.x,renderQueue.front()->p1.y,
                            renderQueue.front()->p2.x,renderQueue.front()->p2.y);
        // renders normal
        SDL_RenderDrawLine(renderWindow,renderQueue.front()->midPoint.x,renderQueue.front()->midPoint.y,
                            renderQueue.front()->normal.x,renderQueue.front()->normal.y);
        renderQueue.pop();
    }
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

    SDL_RenderDrawLineF(renderWindow,x,y,x+(400*cos(angle)),y+(400*sin(angle))); // Middle line
    SDL_RenderDrawLineF(renderWindow,x,y,x+(400*cos(angle+fov)),y+(400*sin(angle+fov))); //Upper line
    SDL_RenderDrawLineF(renderWindow,x,y,x+(400*cos(angle-fov)),y+(400*sin(angle-fov))); // lower line
}