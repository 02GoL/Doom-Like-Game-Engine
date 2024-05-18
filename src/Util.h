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
bool inTargetAngle(float angle, float lowerAngle, float upperAngle, float range);
void setMidPoint(Vector* vector);
void setNormal(Vector* vector);
Point getIntersection(Vector* v1, Vector* v2);
Point getIntersection(Vector* v1, Point p, Point p1);
bool onLine(Vector* vector, Point point);