#pragma once
#include<iostream>
#include<vector>
#include<SDL2/SDL.h>
#include<sstream>
#include<fstream>
#include<math.h>
#include<queue>
#define eps 0.00005
#define PI 3.14159

using namespace std;

struct Point{
    float x;
    float y;

    Point(float x, float y):x(x), y(y){}
    Point(){};
};

struct Vector{
    Point p1;
    Point p2;
    Point midPoint;
    Point normal;
    int facingDir;
    int sectorIndex;
    int vectorIndex;
    int portalingSector;

    Vector(Point p1, Point p2):p1(p1), p2(p2){}
    Vector(){}
    void setMidPoint();
    void setNormal();
};

struct Sector{
    int sectorIndex;
    float floorHeight;
    float ceilHeight;
};

struct RenderedSection{
    float t0;
    float t1;
    float x0;
    float y0;
    float slope;
    float floor;
    float ceiling;
    bool portal;
    
    RenderedSection(float t0, float t1, float x0, float y0, float slope, float floor, float ceiling, bool portal){
        this->t0 = t0;
        this->t1 = t1;
        this->x0 = x0;
        this->y0 = y0;
        this->slope = slope;
        this->floor = floor;
        this->ceiling = ceiling;
        this->portal = portal;
    }
    RenderedSection(float t0, float t1){
        this->t0 = t0;
        this->t1 = t1;
    }
    RenderedSection(){};
};

float toRad(float angle);
float pytha(float a, float b);
int getMax(int a, int b);
int getMin(int a, int b);
float getMaxF(float a, float b);
float getMinF(float a, float b);
float getDet(Point p1, Point p2, Point p3);
float getAngleDiff(float angle, float relativeAngle);
float getAbsAngleDiff(float angle, float relativeAngle);
float normalizeAngle(float angle);
bool inAngleRange(float angle, float lowerAngleBound, float upperAnglerBound);
bool inRange(float x, float boundA, float boundB);
Point intersectingPoint(Vector* v1, Vector* v2);
Point intersectingPoint(Vector* v1, Point p, Point p1);
Point intersectingPoint(Vector* v1, Point p1, float angle);
bool isIntersectingSeg(Vector* v1, Point p1, float angle);
bool isIntersectingSeg(Vector* v1, Point p1, Point p2);