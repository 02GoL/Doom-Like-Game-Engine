#include"Util.h"

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

float toRad(float angle){
    if(angle == 360){
        angle = 0;
    }
    return angle*PI/180.0f;
}

float pytha(float a, float b){
    return sqrt(pow(a,2)+pow(b,2));
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

float getAngleDiff(float angle, float relativeAngle){
    float diff = fmod(angle-relativeAngle+PI,2*PI);
    if(diff < 0){
        diff += 2*PI;
    }
    return diff-PI;
}

float getAbsAngleDiff(float angle,float relativeAngle){ //lower bound | upper bound
    return abs(getAngleDiff(angle,relativeAngle));
}

float normalizeAngle(float angle){
    angle = fmod(angle,2*PI);
    if(angle < 0){
        angle += 2*PI;
    }
    return angle;
}

bool inAngleRange(float angle, float lowerAngleBound, float upperAnglerBound){
    float range = getAbsAngleDiff(lowerAngleBound,upperAnglerBound)+eps;
    return getAbsAngleDiff(angle,lowerAngleBound) <= range && getAbsAngleDiff(angle,upperAnglerBound) <= range;
}

bool inRange(float angle, float boundA, float boundB){
    return angle >= getMinF(boundA,boundB)-eps && angle <= getMaxF(boundA,boundB)+eps;
}

// need to add more info about this function since its different
bool isIntersectingSeg(Vector* v1, Point p1, float angle){
    Point p2 = p1;
    p2.x += cos(angle);
    p2.y += sin(angle);
    float t = float(((v1->p1.x-p1.x)*(p1.y-p2.y)-(v1->p1.y-p1.y)*(p1.x-p2.x))/
                    ((v1->p1.x-v1->p2.x)*(p1.y-p2.y)-(v1->p1.y-v1->p2.y)*(p1.x-p2.x)));
    /*
    float u = float(-((v1->p1.x-v1->p2.x)*(v1->p1.y-p1.y)-(v1->p1.y-v1->p2.y)*(v1->p1.x-p1.x))/
                ((v1->p1.x-v1->p2.x)*(p1.y-p2.y)-(v1->p1.y-v1->p2.y)*(p1.x-p2.x)));
    */
    float x = float(v1->p1.x+t*(v1->p2.x-v1->p1.x));
    float y = float(v1->p1.y+t*(v1->p2.y-v1->p1.y));
    
    float theta = normalizeAngle(atan2(y-p1.y,x-p1.x));
    return inRange(t,0,1) && inAngleRange(theta,angle,angle);
}

bool isIntersectingSeg(Vector* v1, Point p1, Point p2){
    float t = float(((v1->p1.x-p1.x)*(p1.y-p2.y)-(v1->p1.y-p1.y)*(p1.x-p2.x))/
                    ((v1->p1.x-v1->p2.x)*(p1.y-p2.y)-(v1->p1.y-v1->p2.y)*(p1.x-p2.x)));
    float u = float(-((v1->p1.x-v1->p2.x)*(v1->p1.y-p1.y)-(v1->p1.y-v1->p2.y)*(v1->p1.x-p1.x))/
                ((v1->p1.x-v1->p2.x)*(p1.y-p2.y)-(v1->p1.y-v1->p2.y)*(p1.x-p2.x)));
    return inRange(t,0,1) && inRange(u,0,1);
}

Point intersectingPoint(Vector* v1, Vector* v2){
    float t = float((v1->p1.x-v2->p1.x)*(v2->p1.y-v2->p2.y)-(v1->p1.y-v2->p1.y)*(v2->p1.x-v2->p2.x))/
                ((v1->p1.x-v1->p2.x)*(v2->p1.y-v2->p2.y)-(v1->p1.y-v1->p2.y)*(v2->p1.x-v2->p2.x));
    Point newPoint(float(v1->p1.x+t*(v1->p2.x-v1->p1.x)),float(v1->p1.y+t*(v1->p2.y-v1->p1.y)));
    return newPoint;
}

Point intersectingPoint(Vector* v1, Point p1, Point p2){
    float t = float((v1->p1.x-p1.x)*(p1.y-p2.y)-(v1->p1.y-p1.y)*(p1.x-p2.x))/
                ((v1->p1.x-v1->p2.x)*(p1.y-p2.y)-(v1->p1.y-v1->p2.y)*(p1.x-p2.x));
    Point newPoint(float(v1->p1.x+t*(v1->p2.x-v1->p1.x)),float(v1->p1.y+t*(v1->p2.y-v1->p1.y)));
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

void Vector::setMidPoint(){
    this->midPoint.x = (this->p1.x+this->p2.x)/2;
    this->midPoint.y = (this->p1.y+this->p2.y)/2;
}

void Vector::setNormal(){
    float a = this->p2.x-this->p1.x;
    float b = this->p2.y-this->p1.y;
    float uVect = sqrt(pow(a,2) + pow(b,2));
    if(this->facingDir == 1){ 
        this->normal.x = this->midPoint.x+(a*cos(M_PI/2)-b*sin(M_PI/2))*10/uVect;
        this->normal.y = this->midPoint.y+(a*sin(M_PI/2)+b*cos(M_PI/2))*10/uVect;
    }else{
        this->normal.x = this->midPoint.x-(a*cos(M_PI/2)-b*sin(M_PI/2))*10/uVect;
        this->normal.y = this->midPoint.y-(a*sin(M_PI/2)+b*cos(M_PI/2))*10/uVect;
    }
}
