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
        Node* insertNode(Node* node, Vector* vector);
        void getRenderOrder(Node* node, queue<Vector*>& vectorList, Point playerPosition);
    public:
        void loadTree(string mapDataPath);
        int getVectorPos(Vector* v1, Vector* v2);
        int getPointPos(Vector* v1, Point p);
        int getHeight(Node* node);
        queue<Vector*> getRenderOrder(Point playerPosition, float angle);
};