#include"Util.h"
#include"Player.h"

class ScreenRenderer{
    private:
        SDL_Renderer* renderWindow;
        Player* player;
        float fov;
        float screenSizeX;
        float screenSizeY;
        float screenDist;
        vector<Vector*> coveringVectors;
    public:
        ScreenRenderer(SDL_Renderer* renderWindow, Player* player, float screenSizeX, float screenSizeY);
        void renderTopDown(queue<Vector*> renderQueue);
        void renderPlayer();
        void renderFOV(queue<Vector*> renderQueue);
        bool inFOV(Vector* vector);
        void renderWalll(Vector* vector);
        void drawClipWall(float a, float b, float c, float d);
};