#include<iostream>
#include"Engine.h"

using namespace std;

int main(int argc, char* argv[]){
    Engine* engineInstance = new Engine(800,600);
    engineInstance->startEngineLoop();
    return 0;
}
