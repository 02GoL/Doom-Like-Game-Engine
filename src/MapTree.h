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
        Node* insertNode(Node* node,Vector* vector);
        void renderVectorMap(Node* node,SDL_Renderer *renderWindow);
        void drawVector(vector<Vector*> vectors,SDL_Renderer *renderWindow);
    public:
        void loadTree(string mapDataPath);
        void renderVectorMap(SDL_Renderer *renderWindow);
        int getVectorPos(Vector* v1, Vector* v2);
        int getPointPos(Vector* v1, Point p);
        int getHeight(Node* node);
};