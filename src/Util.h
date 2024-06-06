#pragma once
#include<iostream>
#include<vector>
#include<SDL2/SDL.h>
#include<sstream>
#include<fstream>
#include<math.h>
#include<queue>
#define eps 0.001
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
    float ceilingHeight;
};

struct NumberRange{
    float lowerX;
    float upperX;
    NumberRange(float lowerX, float upperX):lowerX(lowerX), upperX(upperX){}
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
