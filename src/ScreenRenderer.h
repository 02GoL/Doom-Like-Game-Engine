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
        vector<RenderedSection*> renderedSection;
    public:
        ScreenRenderer(SDL_Renderer* renderWindow, Player* player, float screenSizeX, float screenSizeY, vector<Sector*> sectorData);
        void renderTopDown(queue<Vector*> renderQueue);
        void renderPlayer();
        void renderFOV(queue<Vector*> renderQueue);
        bool inFOV(Vector* vector);
        void renderWall(Vector* vector);
        void drawWall(Point* p0, Point* p1, float slope, float floor, float ceiling);
        void drawWall(Point* p0, Point* p1, float slope, float floor, float ceiling, RenderedSection* portal);
        void clipWall(float lowerTheta, float upperTheta, Vector* vector, Vector* screenVector);
};