#include<iostream>
#include<set>
#include<SDL2/SDL.h>

using namespace std;

class KeyHandler{
    private:
        set<SDL_Keycode> keybordEvents;
    public:
        void setKeyDown(SDL_Keycode event);
        void setKeyRelease(SDL_Keycode event);
        bool isPressed(SDL_Keycode event);
        bool hasKeyEvent();
};