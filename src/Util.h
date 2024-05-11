#include<iostream>
#include<vector>
#include<SDL2/SDL.h>
#include<sstream>
#include<fstream>
#include<math.h>
#pragma once

using namespace std;

struct Point{
    int x;
    int y;
    int z;
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
int getDet(Point p1, Point p2, Point p3);
void setMidPoint(Vector* vector);
void setNormal(Vector* vector);
Point getItersection(Vector* v1, Vector* v2);