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
    float z;
    Point(float x, float y, float z):x(x), y(y), z(z){}
    Point(){};
};

struct Vector{
    Point p1;
    Point p2;
    Point midPoint;
    Point normal;
    int index;
    int facingDir;
    Vector(Point p1, Point p2):p1(p1), p2(p2){}
    Vector(){}
    void setMidPoint();
    void setNormal();
};

struct NumberRange{
    float lower;
    float upper;
    NumberRange(float lower, float upper):lower(lower), upper(upper){}
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
