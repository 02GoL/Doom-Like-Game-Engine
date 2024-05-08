#include"map.h"

struct Node{
    vector<Vector*> vectors;
    Node* back = NULL;
    Node* front = NULL;
    int height = 1;
};

class MapTree{
    private:
        Node* root;
        Map map;
        vector<Vector*> mapVectors;
        SDL_Renderer* renderWindow;
        Node* insertNode(Node* node,Vector* vector);
        void renderVectorMap(Node* node);
    public:
        void setRenderWindow(SDL_Renderer* renderWindow);
        void loadTree(string mapDataPath);
        int getVectorPos(Vector* v1, Vector* v2);
        int getPointPos(Vector* v1, Point p);
        int getDet(Point p1, Point p2, Point p3);
        bool checkPoints(Point p1, Point p2);
        int getHeight(Node* node);
        int getMax(int a, int b);
        void renderVectorMap();
        void drawVector(vector<Vector*> vectors);
};