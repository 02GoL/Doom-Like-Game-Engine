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
int getMax(int a, int b);
float getDet(Point p1, Point p2, Point p3);
void setMidPoint(Vector* vector);
void setNormal(Vector* vector);
Point getIntersection(Vector* v1, Vector* v2);
Point getIntersection(Vector* v1, Point p, Point p1);