#include"Util.h"

float toRad(float angle){
    if(angle == 360){
        angle = 0;
    }
    return angle*M_PI/180.0f;
}

float pytha(float a, float b){
    return sqrt(pow(a,2)+pow(b,2));
}

int getMax(int a, int b){
    if(a >= b){
        return a;
    }else{
        return b;
    }
}

int getMin(int a, int b){
    if(a < b){
        return a;
    }else{
        return b;
    }
}

float getMaxF(float a, float b){
    if(a >= b){
        return a;
    }else{
        return b;
    }
}

float getMinF(float a, float b){
    if(a < b){
        return a;
    }else{
        return b;
    }
}

float getDet(Point p1, Point p2, Point p3){
    return (p2.x-p1.x)*(p3.y-p1.y)-(p3.x-p1.x)*(p2.y-p1.y);
}

float getAngleDiff(float theta, float beta){
    float diff = fmod(beta-theta+M_PI,2*M_PI);
    if(diff < 0){
        diff += 2*M_PI;
    }
    return abs(diff-M_PI);
}

float normalizeAngle(float angle){
    angle = fmod(angle,2*M_PI);
    if(angle < 0){
        angle += 2*M_PI;
    }
    return angle;
}

bool inAngleRange(float angle, float lowerAngleBound, float upperAnglerBound){
    float range = getAngleDiff(lowerAngleBound,upperAnglerBound);
    return getAngleDiff(angle,lowerAngleBound) <= range && getAngleDiff(angle,upperAnglerBound) <= range;
}

bool inRange(float angle, float boundA, float boundB){
    return angle >= getMinF(boundA,boundB)-eps && angle <= getMaxF(boundA,boundB)+eps;
}

void setMidPoint(Vector* vector){
    vector->midPoint.x = (vector->p1.x+vector->p2.x)/2;
    vector->midPoint.y = (vector->p1.y+vector->p2.y)/2;
}

void setNormal(Vector* vector){
    float a = vector->p2.x-vector->p1.x;
    float b = vector->p2.y-vector->p1.y;
    float uVect = sqrt(pow(a,2) + pow(b,2));
    if(vector->facingDir == 1){ // 1 == inward 0 == outward
        vector->normal.x = vector->midPoint.x+(a*cos(M_PI/2)-b*sin(M_PI/2))*10/uVect;
        vector->normal.y = vector->midPoint.y+(a*sin(M_PI/2)+b*cos(M_PI/2))*10/uVect;
    }else{
        vector->normal.x = vector->midPoint.x-(a*cos(M_PI/2)-b*sin(M_PI/2))*10/uVect;
        vector->normal.y = vector->midPoint.y-(a*sin(M_PI/2)+b*cos(M_PI/2))*10/uVect;
    }
}

Point intersectingPoint(Vector* v1, Vector* v2){
    Point newPoint;
    float t = float((v1->p1.x-v2->p1.x)*(v2->p1.y-v2->p2.y)-(v1->p1.y-v2->p1.y)*(v2->p1.x-v2->p2.x))/
                ((v1->p1.x-v1->p2.x)*(v2->p1.y-v2->p2.y)-(v1->p1.y-v1->p2.y)*(v2->p1.x-v2->p2.x));
    newPoint.x = float(v1->p1.x+t*(v1->p2.x-v1->p1.x));
    newPoint.y = float(v1->p1.y+t*(v1->p2.y-v1->p1.y));
    return newPoint;
}

Point intersectingPoint(Vector* v1, Point p1, Point p2){
    Point newPoint;
    float t = float((v1->p1.x-p1.x)*(p1.y-p2.y)-(v1->p1.y-p1.y)*(p1.x-p2.x))/
                ((v1->p1.x-v1->p2.x)*(p1.y-p2.y)-(v1->p1.y-v1->p2.y)*(p1.x-p2.x));
    newPoint.x = float(v1->p1.x+t*(v1->p2.x-v1->p1.x));
    newPoint.y = float(v1->p1.y+t*(v1->p2.y-v1->p1.y));
    return newPoint;
}

Point intersectingPoint(Vector* v1, Point p1, float angle){
    Point p2 = p1;
    p2.x += cos(angle);
    p2.y += sin(angle);
    float t = float((v1->p1.x-p1.x)*(p1.y-p2.y)-(v1->p1.y-p1.y)*(p1.x-p2.x))/
                ((v1->p1.x-v1->p2.x)*(p1.y-p2.y)-(v1->p1.y-v1->p2.y)*(p1.x-p2.x));
    p2.x = float(v1->p1.x+t*(v1->p2.x-v1->p1.x));
    p2.y = float(v1->p1.y+t*(v1->p2.y-v1->p1.y));
    return p2;
}

bool isIntersectingSeg(Vector* v1, Point p1, float angle){
    Point p2 = p1;
    p2.x += cos(angle);
    p2.y += sin(angle);
    float t = float((v1->p1.x-p1.x)*(p1.y-p2.y)-(v1->p1.y-p1.y)*(p1.x-p2.x))/
                ((v1->p1.x-v1->p2.x)*(p1.y-p2.y)-(v1->p1.y-v1->p2.y)*(p1.x-p2.x));
    /*
    float u = ((v1->p1.x-v1->p2.x)*(v1->p1.y-v1->p1.y)-(v1->p1.y-v1->p2.y)*(v1->p1.x-p1.x))/
                ((v1->p1.x-v1->p2.x)*(v1->p1.y-p2.y)-(v1->p1.y-v1->p2.y)*(p1.x-p2.x));
    */
    float x = float(v1->p1.x+t*(v1->p2.x-v1->p1.x));
    float y = float(v1->p1.y+t*(v1->p2.y-v1->p1.y));
    
    float theta = normalizeAngle(atan2(y-p1.y,x-p1.x));
    return inRange(theta,angle,angle) && inRange(t,0,1);
}

bool onLine(Vector* vector, Point point){
    if(((point.x >= vector->p1.x-eps && point.x <= vector->p2.x+eps) || 
    (point.x >= vector->p2.x-eps && point.x <= vector->p1.x+eps)) &&
    ((point.y >= vector->p1.y-eps && point.y <= vector->p2.y+eps) || 
    (point.y >= vector->p2.y-eps && point.y <= vector->p1.y+eps))){
        return true;
    }else{
        return false;
    }
}