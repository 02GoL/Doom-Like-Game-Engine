#include"Util.h"

class FOVRenderer{
    private:
        SDL_Renderer* renderWindow;
        float fov;
        float screenSizeX;
        float screenSizeY;
        float screenDist;
        vector<Vector*> coveringVectors;
    public:
        FOVRenderer(SDL_Renderer* renderWindow,float screenSizeX, float screenSizeY, float fov);
        void renderTopDown(queue<Vector*> renderQueue, Point playerPosition, float playerAngle);
        void renderFOV(queue<Vector*> renderQueue, Point playerPosition, float playerAngle);
        bool inFOV(Vector* vector, Point playerPosition, float playerAngle);
        void renderWalll(Vector* vector, Point playerPosition, float playerAngle);
        void drawClipWall(float a, float b, float c, float d);
};