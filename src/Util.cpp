#include"Util.h"

float toRad(float angle){
    return angle*M_PI/180.0f;
}

int getMax(int a, int b){
    if(a > b){
        return a;
    }else{
        return b;
    }
}

int getDet(Point p1, Point p2, Point p3){
    return (p2.x-p1.x) * (p3.y-p1.y) - (p3.x-p1.x) * (p2.y-p1.y);
}

void setMidPoint(Vector* vector){
    vector->midPoint.x = (vector->p1.x+vector->p2.x)/2;
    vector->midPoint.y = (vector->p1.y+vector->p2.y)/2;
}

void setNormal(Vector* vector){
    int a = vector->p2.x - vector->p1.x;
    int b = vector->p2.y - vector->p1.y;
    int uVect = sqrt(pow(a,2) + pow(b,2));
    if(vector->facingDir == 1){ // 1 == inward 0 == outward
        vector->normal.x = vector->midPoint.x + (a*cos(M_PI/2) - b*sin(M_PI/2)) * 10 / uVect;
        vector->normal.y = vector->midPoint.y + (a*sin(M_PI/2) + b*cos(M_PI/2)) * 10 / uVect;
    }else{
        vector->normal.x = vector->midPoint.x - (a*cos(M_PI/2) - b*sin(M_PI/2)) * 10 / uVect;
        vector->normal.y = vector->midPoint.y - (a*sin(M_PI/2) + b*cos(M_PI/2)) * 10 / uVect;
    }
}

Point getItersection(Vector* v1, Vector* v2){
    Point p;

    int day = v1->p2.y - v1->p1.y;
    int dax = v1->p1.x - v1->p2.x;
    int da = day*v1->p1.x + dax*v1->p1.y;

    int dby = v2->p2.y - v2->p1.y;
    int dbx = v2->p1.x - v2->p2.x;
    int db = dby*v2->p1.x + dbx*v2->p1.y;

    double det = double(day*dbx - dax*dby);

    int x = double(dbx*da - dax*db)/det;
    int y = double(day*db - dby*da)/det;
    p.x = x;
    p.y = y;
    return p;
}