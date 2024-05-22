#include"MapTree.h"

void MapTree::loadFrom(string mapDataPath){
    this->root = NULL;
    this->mapVectors = map.loadData(mapDataPath);
    while(!mapVectors.empty()){
        cout << "Added vector " << mapVectors.front()->index << " at " << mapVectors.front()->p1.x <<  ", "<< mapVectors.front()->p1.y << " to " << mapVectors.front()->p2.x <<  ", "<< mapVectors.front()->p2.y << "\n";
        this->root = insertNode(root,mapVectors.front());
    }
    cout << this->root->height << "\n";
}

Node* MapTree::insertNode(Node* node, Vector* vector){
    if(node == NULL){
        Node* newNode = new Node;
        newNode->vectors.push_back(vector);
        mapVectors.erase(mapVectors.begin());
        return newNode;
    }
    int position = getVectorPos(node->vectors.at(0),vector);
    if(position == 1){
        cout << "Added to front\n";
        node->front = insertNode(node->front,vector);
    }else if(position == 2){
        cout << "Added to back\n";
        node->back = insertNode(node->back,vector);
    }else if(position == 3){
        cout << "Split line\n";
        Point splitPoint = intersectingPoint(node->vectors.at(0),vector);
        Vector* v1 = new Vector;
        v1->p1 = vector->p1;
        v1->p2 = splitPoint;
        v1->index = vector->index;
        v1->facingDir = vector->facingDir;
        setMidPoint(v1);
        setNormal(v1);
        Vector* v2 = new Vector;
        v2->p1 = splitPoint;
        v2->p2 = vector->p2;
        v2->index = vector->index;
        v2->facingDir = vector->facingDir;
        setMidPoint(v2);
        setNormal(v2);
        mapVectors.push_back(v1);
        mapVectors.push_back(v2);
        mapVectors.erase(mapVectors.begin());
    }else if(position == 0){
        cout << "Line is co-linear\n";
        node->vectors.push_back(vector);
        mapVectors.erase(mapVectors.begin());
    }
    node->height = getMax(getHeight(node->front),getHeight(node->back))+1;
    return node; 
}

int MapTree::getHeight(Node* node){
    if(node == NULL){
        return 0;
    }else{
        return node->height;
    }
}

int MapTree::getVectorPos(Vector* v1, Vector* v2){ // v1 is the hyper plane vector while v2 is the vector being tested
    float detA = getDet(v1->p1,v1->p2,v2->p1);
    float detB = getDet(v1->p1,v1->p2,v2->p2);
    if(v1->facingDir == 0){
        detA = -detA;
        detB = -detB;
    }
    if(detA > 0 && detB >= 0 || detA >= 0 && detB > 0){
        return 1; // both points are on the back side 
    }else if(detA < 0 && detB <= 0 || detA <= 0 && detB < 0){
        return 2; // both points are on the front side
    }else if(detA > 0 && detB < 0 || detA < 0 && detB > 0){
        return 3; // the vector is being split by the hyper plane
    }else{
        return 0; // the vector is colinear
    }
}

int MapTree::getPointPos(Vector* v1, Point p){
    float detA = getDet(v1->p1,v1->p2,p);
    if(v1->facingDir == 0){
        detA = -detA;
    }
    if(detA > 0){
        return 1; // both points are on the back side 
    }else if(detA < 0){
        return 2; // both points are on the front side
    }else{
        return 0; // the vector is colinear
    }
}

queue<Vector*> MapTree::getRenderOrder(Point playerPosition, float angle){
    queue<Vector*> renderQueue;
    getRenderOrder(this->root,renderQueue,playerPosition);
    return renderQueue;
}

// might want to improve this so it doesn't need vector list as an input/have it return something other than void
void MapTree::getRenderOrder(Node* node, queue<Vector*>& renderQueue, Point playerPosition){
    if(node == NULL){
        return;
    }else{
        int position = getPointPos(node->vectors.at(0),playerPosition);
        if(position == 1){ // back
            getRenderOrder(node->front,renderQueue,playerPosition);
            for(Vector* vector:node->vectors){
                renderQueue.push(vector);
            }
            getRenderOrder(node->back, renderQueue,playerPosition);
        }else if(position == 2){ // front
            getRenderOrder(node->back, renderQueue,playerPosition);
            for(Vector* vector:node->vectors){
                renderQueue.push(vector);
            }
            getRenderOrder(node->front,renderQueue,playerPosition);
        }else{ // colinear
            getRenderOrder(node->front,renderQueue,playerPosition);
            getRenderOrder(node->back,renderQueue,playerPosition);
        }
    }
}