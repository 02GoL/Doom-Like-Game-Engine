#include<iostream>
#include<SDL2/SDL.h>
#include"Player.h"
#include"Maptree.h"
using namespace std;

class Engine{
    private:
        SDL_Renderer *renderWindow;
        Player player;
        MapTree mapTree;
    public:
        void engineInit(SDL_Renderer *renderWindow){
            this->renderWindow = renderWindow;
            mapTree.setRenderWindow(renderWindow);
            mapTree.loadTree("../test/mapData.txt");
            player.playerInit(400,400);
        }
        void inputHandler(){
            player.inputHandler();
        }
        void renderGame(){
            mapTree.renderVectorMap();
            player.renderPlayer(renderWindow);
        }
        void movementHandler(){
            player.movementHandler();
        }

};