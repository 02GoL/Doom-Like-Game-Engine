#include<iostream>
#include<vector>
#include<SDL2/SDL.h>
#include<sstream>
#include<fstream>
#include<math.h>
#include<queue>
#define eps 0.01

#pragma once

using namespace std;

struct Point{
    float x;
    float y;
    float z;
};

struct Vector{
    Point p1;
    Point p2;
    Point midPoint;
    Point normal;
    int index;
    int facingDir;
};

float toRad(float angle);
float pytha(float a, float b);
int getMax(int a, int b);
int getMin(int a, int b);
float getMaxF(float a, float b);
float getMinF(float a, float b);
float getDet(Point p1, Point p2, Point p3);
float getAngleDiff(float theta, float beta);
float normalizeAngle(float angle);
bool inAngleRange(float angle, float lowerAngleBound, float upperAnglerBound);
bool inRange(float x, float boundA, float boundB);
void setMidPoint(Vector* vector);
void setNormal(Vector* vector);
Point intersectingPoint(Vector* v1, Vector* v2);
Point intersectingPoint(Vector* v1, Point p, Point p1);
Point intersectingPoint(Vector* v1, Point p1, float angle);
bool isIntersectingSeg(Vector* v1, Point p1, float angle);
bool onLine(Vector* vector, Point point);