#include"FOVRenderer.h"

FOVRenderer::FOVRenderer(SDL_Renderer* renderWindow,float screenSizeX, float screenSizeY, float fov){
    this->renderWindow = renderWindow;
    this->fov = toRad(fov);
    this->screenSizeX = screenSizeX;
    this->screenSizeY = screenSizeY;
    this->screenDist = screenSizeX/(2*tan(this->fov/2));
}

void FOVRenderer::renderFOV(queue<Vector*> renderQueue, Point playerPosition, float playerAngle){
    queue<Vector*> vectorQueue = renderQueue;
    coveringVectors.clear();
    while(!vectorQueue.empty()){
        if(inFOV(vectorQueue.front(),playerPosition,playerAngle)){
            SDL_SetRenderDrawColor(renderWindow,210,4,45,255);
            renderWalll(vectorQueue.front(),playerPosition,playerAngle);
        }else{
            SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
        }
        /*
        // renders vector
        SDL_RenderDrawLine(renderWindow,vectorQueue.front()->p1.x,vectorQueue.front()->p1.y,
                            vectorQueue.front()->p2.x,vectorQueue.front()->p2.y);
        // renders normal
        SDL_RenderDrawLine(renderWindow,vectorQueue.front()->midPoint.x,vectorQueue.front()->midPoint.y,
                            vectorQueue.front()->normal.x,vectorQueue.front()->normal.y);
        */
        vectorQueue.pop();
    }
}

bool FOVRenderer::inFOV(Vector* vector, Point playerPosition, float playerAngle){
    Point facingPoint = playerPosition;
    float x = facingPoint.x;
    float y = facingPoint.y;
    float angle = playerAngle;
    float lowerAngle = normalizeAngle(angle-fov/2);
    float upperAngle = normalizeAngle(angle+fov/2);

    facingPoint.x += cos(angle);
    facingPoint.y += sin(angle);
    facingPoint = intersectingPoint(vector,playerPosition,facingPoint);

    float theta = normalizeAngle(atan2(facingPoint.y-y,facingPoint.x-x)); // angle for a point infront
    float alpha = normalizeAngle(atan2(vector->p1.y-y,vector->p1.x-x)); // angle for the point p1
    float beta = normalizeAngle(atan2(vector->p2.y-y,vector->p2.x-x)); // angle for the point p2
    
    if(inAngleRange(alpha,lowerAngle,upperAngle) || inAngleRange(beta,lowerAngle,upperAngle)){
        // if the angle of p1 or p2 relative to the player is in the fov, draw
        return true;
    }
    if(onLine(vector,facingPoint) && inAngleRange(theta,lowerAngle,upperAngle)){
        // if both points of p1 and p2 are outside and the wall it creates is in the fov, draw
        return true;
    }
    return false;
}

void FOVRenderer::renderWalll(Vector* vector, Point playerPosition, float playerAngle){
    Point playerPoint = playerPosition;
    float angle = playerAngle;
    float hyp = pytha(screenDist,screenSizeX/2);
    float lowerAngle = normalizeAngle(angle-fov/2);
    float upperAngle = normalizeAngle(angle+fov/2);

    Vector* screenVector = new Vector;
    screenVector->p1 = playerPoint;
    screenVector->p1.x += hyp*cos(lowerAngle);
    screenVector->p1.y += hyp*sin(lowerAngle);
    screenVector->p2 = playerPoint;
    screenVector->p2.x += hyp*cos(upperAngle);
    screenVector->p2.y += hyp*sin(upperAngle);
    
    // gets the intersect of both end points to a vector and angle relative of the player
    Point pointA = vector->p1;
    Point pointB = vector->p2;
    float thetaA = normalizeAngle(atan2(pointA.y-playerPoint.y,pointA.x-playerPoint.x));
    float thetaB = normalizeAngle(atan2(pointB.y-playerPoint.y,pointB.x-playerPoint.x));
        
    // from the inital 2 points, the following will determine where the points end up on the
    // players fov projected on the screen
    if(!inAngleRange(thetaA,lowerAngle,upperAngle) && !inAngleRange(thetaB,lowerAngle,upperAngle)){
        // if both points are outside the fov reset both points to be the sides of the fov
        pointA = intersectingPoint(vector,playerPoint,lowerAngle);
        pointB = intersectingPoint(vector,playerPoint,upperAngle);
        thetaA = normalizeAngle(atan2(pointA.y-playerPoint.y,pointA.x-playerPoint.x));
        thetaB = normalizeAngle(atan2(pointB.y-playerPoint.y,pointB.x-playerPoint.x));
        SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
    }else if(inAngleRange(thetaA,lowerAngle,upperAngle) && !inAngleRange(thetaB,lowerAngle,upperAngle)){
        if(isIntersectingSeg(vector,playerPosition,upperAngle) && !isIntersectingSeg(vector,playerPosition,lowerAngle)){
            pointB = intersectingPoint(vector,playerPoint,upperAngle);
        }else{
            pointB = intersectingPoint(vector,playerPoint,lowerAngle);
        }
        thetaB = normalizeAngle(atan2(pointB.y-playerPoint.y,pointB.x-playerPoint.x));
        SDL_SetRenderDrawColor(renderWindow,230,173,230,255);
    }else if(inAngleRange(thetaB,lowerAngle,upperAngle) && !inAngleRange(thetaA,lowerAngle,upperAngle)){
        if(isIntersectingSeg(vector,playerPosition,lowerAngle) && !isIntersectingSeg(vector,playerPosition,upperAngle)){
            pointA = intersectingPoint(vector,playerPoint,lowerAngle);
        }else{
            pointA = intersectingPoint(vector,playerPoint,upperAngle);
        }
        thetaA = normalizeAngle(atan2(pointA.y-playerPoint.y,pointA.x-playerPoint.x));
        SDL_SetRenderDrawColor(renderWindow,255,0,0,255);
    }
    /*
    SDL_RenderDrawLine(renderWindow,playerPoint.x+2,playerPoint.y+2,
                            pointA.x,pointA.y);
                     
    SDL_RenderDrawLine(renderWindow,playerPoint.x+2,playerPoint.y+2,
                            pointB.x,pointB.y);
    */
    float aDist = pytha(pointA.x-playerPoint.x,pointA.y-playerPoint.y);
    float bDist = pytha(pointB.x-playerPoint.x,pointB.y-playerPoint.y);
    
    
    pointA = intersectingPoint(screenVector,playerPoint,pointA);
    pointB = intersectingPoint(screenVector,playerPoint,pointB); 
    
    SDL_RenderDrawLine(renderWindow,playerPoint.x,playerPoint.y,
                            pointA.x,pointA.y);
                     
    SDL_RenderDrawLine(renderWindow,playerPoint.x,playerPoint.y,
                            pointB.x,pointB.y);
    
    float ca = normalizeAngle(angle-lowerAngle);
    float cb = normalizeAngle(angle-upperAngle);
    //float a = pytha(vector->p1.x-playerPoint.x,vector->p1.y-playerPoint.y);
    float aScreenDist = pytha(pointA.x-playerPoint.x,pointA.y-playerPoint.y);
    //float c = pytha(vector->p2.x-playerPoint.x,vector->p2.y-playerPoint.y);
    float bScreenDist = pytha(pointB.x-playerPoint.x,pointB.y-playerPoint.y);
    float aPointScale = screenSizeY*(aScreenDist/aDist);
    float bPointScale = screenSizeY*(bScreenDist/bDist);

    if(aPointScale < 50){
        aPointScale = 50;
    }
    if(bPointScale < 50){
        bPointScale = 50;
    }
    
    float screenPointA = pytha(pointA.x-screenVector->p1.x,pointA.y-screenVector->p1.y);
    float screenPointB = pytha(pointB.x-screenVector->p1.x,pointB.y-screenVector->p1.y)-1;      
    drawClipWall(screenPointA,screenPointB,aPointScale,bPointScale);
}

void FOVRenderer::drawClipWall(float a, float b, float c, float d){
    float lowerX = getMinF(a,b)+eps;
    float upperX = getMaxF(a,b)-eps;
    if(coveringVectors.empty()){
        Vector* vector = new Vector;
        vector->p1.x = lowerX;
        vector->p2.x = upperX;
        coveringVectors.push_back(vector);
        SDL_RenderDrawLine(renderWindow,vector->p1.x,screenSizeY/2-c,
                            vector->p1.x,screenSizeY/2+c);
                            
        SDL_RenderDrawLine(renderWindow,vector->p2.x,screenSizeY/2-d,
                            vector->p2.x,screenSizeY/2+d);
    }else{
        for(Vector* v:coveringVectors){
            if(!inRange(lowerX,v->p1.x,v->p2.x) && !inRange(upperX,v->p1.x,v->p2.x)){
                // if its outside all the current vector check the next one
            }else if(inRange(lowerX,v->p1.x,v->p2.x) && inRange(upperX,v->p1.x,v->p2.x)){
                return;
            }else if(inRange(upperX,v->p1.x,v->p2.x) && !inRange(lowerX,v->p1.x,v->p2.x)){
                // replace values
                v->p1.x = lowerX;
                SDL_SetRenderDrawColor(renderWindow,0,0,255,255);
                SDL_RenderDrawLine(renderWindow,v->p1.x,screenSizeY/2-c,
                            v->p1.x,screenSizeY/2+c);
                
                return;
            }else if(inRange(lowerX,v->p1.x,v->p2.x) && !inRange(upperX,v->p1.x,v->p2.x)){
                v->p2.x = upperX;
                SDL_SetRenderDrawColor(renderWindow,230,173,230,255);
                
                SDL_RenderDrawLine(renderWindow,v->p2.x,screenSizeY/2-d,
                            v->p2.x,screenSizeY/2+d);
                return;
            }
        }
        SDL_SetRenderDrawColor(renderWindow,255,0,0,255);
        Vector* vector = new Vector;
        vector->p1.x = lowerX;
        vector->p2.x = upperX;
        coveringVectors.push_back(vector);
        SDL_RenderDrawLine(renderWindow,vector->p1.x,screenSizeY/2-c,
                            vector->p1.x,screenSizeY/2+c);
                            
        SDL_RenderDrawLine(renderWindow,vector->p2.x,screenSizeY/2-d,
                            vector->p2.x,screenSizeY/2+d);
    }
}