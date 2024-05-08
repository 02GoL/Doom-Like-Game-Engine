#include<iostream>
#include<vector>
#include<SDL2/SDL.h>
#include<sstream>
#include<fstream> //For some reason i cannot import just ifstream

using namespace std;

struct Point{
    int x;
    int y;
    int z;
};

struct Vector{
    Point p1;
    Point p2;
    Point midPoint;
    Point normal;
    int index;
    int facingDir;
};

void setMidPoint(Vector* vector);
void setNormal(Vector* vector);
Point getItersection(Vector* v1, Vector* v2);

class Map{
    private:
        vector<Vector*> vectors;
        int cellSize;
    public:
        void loadData(string dataPath);
        void renderMap(SDL_Renderer *renderWindow);
        void renderVectorMap(SDL_Renderer *renderWindow);
        vector<Vector*> getMapVectors(){return vectors;};
};
