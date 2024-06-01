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
        vector<NumberRange*> occupiedAngle;
    public:
        ScreenRenderer(SDL_Renderer* renderWindow, Player* player, float screenSizeX, float screenSizeY);
        void renderTopDown(queue<Vector*> renderQueue);
        void renderPlayer();
        void renderFOV(queue<Vector*> renderQueue);
        bool inFOV(Vector* vector);
        void renderWalll(Vector* vector);
        void drawWall(float x1, float x2, float y1, float y2);
        void clipWall(float lowerTheta, float upperTheta, Vector* vector, Vector* screenVector);
};