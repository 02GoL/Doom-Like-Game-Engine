#include"Engine.h"

void Engine::engineInit(SDL_Renderer *renderWindow){
    this->renderWindow = renderWindow;
    mapTree.loadTree("../test/mapData.txt");
    player = new Player(400,400,0,3,1.75);
}

void Engine::inputHandler(){
    player->inputHandler();
}

void Engine::renderGame(){
    mapTree.renderVectorMap(renderWindow);
    player->renderPlayer(renderWindow);
}

void Engine::movementHandler(){
    player->movementHandler();
}