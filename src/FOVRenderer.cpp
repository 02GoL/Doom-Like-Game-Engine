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
    facingPoint = getIntersection(vector,playerPosition,facingPoint);

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
    Point pointA = getIntersection(screenVector,playerPoint,vector->p1); 
    Point pointB = getIntersection(screenVector,playerPoint,vector->p2); 
    float thetaA = normalizeAngle(atan2(vector->p1.y-playerPoint.y,vector->p1.x-playerPoint.x));
    float thetaB = normalizeAngle(atan2(vector->p2.y-playerPoint.y,vector->p2.x-playerPoint.x));
    
    if(!inAngleRange(thetaA,lowerAngle,upperAngle) && !inAngleRange(thetaB,lowerAngle,upperAngle)){
        // if both points are outside the fov reset both points to be the sides of the fov
        pointA = setPoint(vector,playerPoint,lowerAngle);
        pointB = setPoint(vector,playerPoint,upperAngle);
        thetaA = normalizeAngle(atan2(pointA.y-playerPoint.y,pointA.x-playerPoint.x));
        thetaB = normalizeAngle(atan2(pointB.y-playerPoint.y,pointB.x-playerPoint.x));
        SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
    }else if(inAngleRange(thetaA,lowerAngle,upperAngle) && !inAngleRange(thetaB,lowerAngle,upperAngle)){
        /*
        if the angle of pointA relative the the player is in the fov and pointB isn't
        recalculate pointB to the either the lower or upper bound of the fov that is 
        */
        if(getAngleDiff(thetaB,lowerAngle) < getAngleDiff(thetaB,upperAngle)){
            pointB = setPoint(vector,playerPoint,lowerAngle);
        }else{
            pointB = setPoint(vector,playerPoint,upperAngle);
        }
        thetaB = normalizeAngle(atan2(pointB.y-playerPoint.y,pointB.x-playerPoint.x));
        SDL_SetRenderDrawColor(renderWindow,230,173,230,255);
    }else if(inAngleRange(thetaB,lowerAngle,upperAngle) && !inAngleRange(thetaA,lowerAngle,upperAngle)){
        /*
        if the angle of pointB relative the the player is in the fov and pointA isn't
        recalculate pointA to the either the lower or upper bound of the fov
        */
        if(getAngleDiff(thetaA,lowerAngle) < getAngleDiff(thetaB,upperAngle)){
            pointA = setPoint(vector,playerPoint,lowerAngle);
        }else{
            pointA = setPoint(vector,playerPoint,upperAngle);
        }
        thetaA = normalizeAngle(atan2(pointA.y-playerPoint.y,pointA.x-playerPoint.x));
        SDL_SetRenderDrawColor(renderWindow,255,0,0,255);
    }
    /*
    after finding the point of interest in the fov get the intersecting points 
    from the player to the found point with the screen projected by the player
    */

    float a = pytha(vector->p1.x-playerPoint.x,vector->p1.y-playerPoint.y)*cos(thetaA);
    float b = pytha(pointA.x-playerPoint.x,pointA.y-playerPoint.y)*cos(thetaA);
    float c = pytha(vector->p2.x-playerPoint.x,vector->p2.y-playerPoint.y)*cos(thetaB);
    float d = pytha(pointB.x-playerPoint.x,pointB.y-playerPoint.y)*cos(thetaB);
    float lowerLineH = 100*screenSizeY/a;
    float upperLineH = 100*screenSizeY/c;
    
    if(lowerLineH < 550){
        lowerLineH = 550;
    }
    if(upperLineH < 550){
        upperLineH = 550;
    }
    
    /*
    SDL_RenderDrawLine(renderWindow,playerPoint.x+2,playerPoint.y+2,
                            pointA.x,pointA.y);
                     
    SDL_RenderDrawLine(renderWindow,playerPoint.x+2,playerPoint.y+2,
                            pointB.x,pointB.y);
    */
    pointA = getIntersection(screenVector,playerPoint,pointA);
    pointB = getIntersection(screenVector,playerPoint,pointB); 
    

    float lowerScreenPoint = pytha(pointA.x-screenVector->p1.x,pointA.y-screenVector->p1.y);
    float upperScreenPoint = pytha(pointB.x-screenVector->p1.x,pointB.y-screenVector->p1.y)-1;      
    drawClipWall(lowerScreenPoint,upperScreenPoint,lowerLineH,upperLineH);
}

Point FOVRenderer::setPoint(Vector* vector, Point playerPosition, float angle){
    Point p = playerPosition;
    p.x += cos(angle);
    p.y += sin(angle);
    p = getIntersection(vector,playerPosition,p);
    return p;
}

void FOVRenderer::drawClipWall(float a, float b, float c, float d){
    float lowerX = getMinF(a,b)+eps;
    float upperX = getMaxF(a,b)-eps;
    if(coveringVectors.empty()){
        Vector* vector = new Vector;
        vector->p1.x = lowerX;
        vector->p2.x = upperX;
        coveringVectors.push_back(vector);
        SDL_RenderDrawLine(renderWindow,vector->p1.x,0,
                            vector->p1.x,c);
                            
        SDL_RenderDrawLine(renderWindow,vector->p2.x,0,
                            vector->p2.x,d);
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
                SDL_RenderDrawLine(renderWindow,v->p1.x,0,
                            v->p1.x,c);
                
                return;
            }else if(inRange(lowerX,v->p1.x,v->p2.x) && !inRange(upperX,v->p1.x,v->p2.x)){
                v->p2.x = upperX;
                SDL_SetRenderDrawColor(renderWindow,230,173,230,255);
                
                SDL_RenderDrawLine(renderWindow,v->p2.x,0,
                            v->p2.x,d);
                return;
            }
        }
        SDL_SetRenderDrawColor(renderWindow,255,0,0,255);
        Vector* vector = new Vector;
        vector->p1.x = lowerX;
        vector->p2.x = upperX;
        coveringVectors.push_back(vector);
        SDL_RenderDrawLine(renderWindow,vector->p1.x,0,
                            vector->p1.x,c);
                            
        SDL_RenderDrawLine(renderWindow,vector->p2.x,0,
                            vector->p2.x,d);
    }
}