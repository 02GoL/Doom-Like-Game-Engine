#include"KeyHandler.h"

void KeyHandler::setKeyDown(SDL_Keycode event){
    keybordEvents.insert(event);
}

void KeyHandler::setKeyRelease(SDL_Keycode event){
    keybordEvents.erase(event);
}

bool KeyHandler::isPressed(SDL_Keycode event){
    return keybordEvents.count(event);
}

bool KeyHandler::hasKeyEvent(){
    return keybordEvents.empty();
}