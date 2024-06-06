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
        vector<Sector*> sectorData;
        vector<NumberRange*> occupiedAngle;
    public:
        ScreenRenderer(SDL_Renderer* renderWindow, Player* player, float screenSizeX, float screenSizeY, vector<Sector*> sectorData);
        void renderTopDown(queue<Vector*> renderQueue);
        void renderPlayer();
        void renderFOV(queue<Vector*> renderQueue);
        bool inFOV(Vector* vector);
        void renderWalll(Vector* vector);
        void drawWall(float x1, float x2, float y1, float y2, float floor, float ceiling);
        void drawWall(float x0, float x1, float y0, float y1, float floor, float ceiling, float minY, float maxY);
        void clipWall(float lowerTheta, float upperTheta, Vector* vector, Vector* screenVector);
};