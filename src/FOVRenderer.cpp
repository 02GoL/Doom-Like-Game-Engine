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

    facingPoint.x += cos(angle);
    facingPoint.y += sin(angle);
    facingPoint = getIntersection(vector,playerPosition,facingPoint);

    float theta = normalizeAngle(atan2(facingPoint.y-y,facingPoint.x-x));
    float alpha = normalizeAngle(atan2(vector->p1.y-y,vector->p1.x-x));
    float beta = normalizeAngle(atan2(vector->p2.y-y,vector->p2.x-x));

    float lowerAngle = normalizeAngle(angle-fov/2);
    float upperAngle = normalizeAngle(angle+fov/2);
    
    if(inTargetAngle(alpha,lowerAngle,upperAngle,fov) || inTargetAngle(beta,lowerAngle,upperAngle,fov)){
        return true;
    }else if(onLine(vector,facingPoint) && inTargetAngle(theta,lowerAngle,upperAngle,fov)){
        return true;
    }else{
        return false;
    }
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
    
    Point lowerPoint = getIntersection(screenVector,playerPoint,vector->p1); 
    Point upperPoint = getIntersection(screenVector,playerPoint,vector->p2); 
    float alpha = normalizeAngle(atan2(vector->p1.y-playerPoint.y,vector->p1.x-playerPoint.x));
    float beta = normalizeAngle(atan2(vector->p2.y-playerPoint.y,vector->p2.x-playerPoint.x));
    
    if(!inTargetAngle(alpha,lowerAngle,upperAngle,fov) && !inTargetAngle(beta,lowerAngle,upperAngle,fov)){
        lowerPoint = playerPoint;
        lowerPoint.x += cos(lowerAngle);
        lowerPoint.y += sin(lowerAngle);
        lowerPoint = getIntersection(vector,playerPoint,lowerPoint);
        upperPoint = playerPoint;
        upperPoint.x += cos(upperAngle);
        upperPoint.y += sin(upperAngle);
        upperPoint = getIntersection(vector,playerPoint,upperPoint);
        SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
    }else if(inTargetAngle(alpha,lowerAngle,upperAngle,fov) && !inTargetAngle(beta,lowerAngle,upperAngle,fov)){
        upperPoint = playerPoint;
        upperPoint.x += cos(upperAngle);
        upperPoint.y += sin(upperAngle);
        upperPoint = getIntersection(vector,playerPoint,upperPoint);
        beta = normalizeAngle(atan2(upperPoint.y-playerPoint.y,upperPoint.x-playerPoint.x));
        if(!onLine(vector,upperPoint) || !inTargetAngle(beta,lowerAngle,upperAngle,fov+eps)){
            upperPoint = playerPoint;
            upperPoint.x += cos(lowerAngle);
            upperPoint.y += sin(lowerAngle);
            upperPoint = getIntersection(vector,playerPoint,upperPoint);
        }
        SDL_SetRenderDrawColor(renderWindow,230,173,230,255);
    }else if(inTargetAngle(beta,lowerAngle,upperAngle,fov) && !inTargetAngle(alpha,lowerAngle,upperAngle,fov)){
        lowerPoint = playerPoint;
        lowerPoint.x += cos(lowerAngle);
        lowerPoint.y += sin(lowerAngle);
        lowerPoint = getIntersection(vector,playerPoint,lowerPoint);
        alpha = normalizeAngle(atan2(lowerPoint.y-playerPoint.y,lowerPoint.x-playerPoint.x));
        if(!onLine(vector,lowerPoint) || !inTargetAngle(alpha,lowerAngle,upperAngle,fov+eps)){
            lowerPoint = playerPoint;
            lowerPoint.x += cos(upperAngle);
            lowerPoint.y += sin(upperAngle);
            lowerPoint = getIntersection(vector,playerPoint,lowerPoint);
        }
        SDL_SetRenderDrawColor(renderWindow,255,0,0,255);
    }
    
    lowerPoint = getIntersection(screenVector,playerPoint,lowerPoint);
    upperPoint = getIntersection(screenVector,playerPoint,upperPoint);  
    
    /*
    SDL_RenderDrawLine(renderWindow,playerPoint.x+2,playerPoint.y+2,
                            lowerPoint.x,lowerPoint.y);
                     
    SDL_RenderDrawLine(renderWindow,playerPoint.x+2,playerPoint.y+2,
                            upperPoint.x,upperPoint.y);
    */
    float lowerScreenPoint = pytha(lowerPoint.x-screenVector->p1.x,lowerPoint.y-screenVector->p1.y);
    float upperScreenPoint = pytha(upperPoint.x-screenVector->p1.x,upperPoint.y-screenVector->p1.y)-1;      
    drawClipWall(lowerScreenPoint,upperScreenPoint);
}

void FOVRenderer::drawClipWall(float a, float b){
    float lowerX = getMinF(a,b);
    float upperX = getMaxF(a,b);
    if(coveringVectors.empty()){
        Vector* vector = new Vector;
        vector->p1.x = lowerX;
        vector->p2.x = upperX;
        coveringVectors.push_back(vector);
        SDL_RenderDrawLine(renderWindow,vector->p1.x,0,
                            vector->p1.x,screenSizeY);
                            
        SDL_RenderDrawLine(renderWindow,vector->p2.x,0,
                            vector->p2.x,screenSizeY);
    }else{
        for(Vector* v:coveringVectors){
            if((lowerX < v->p1.x && upperX < v->p1.x) || (lowerX > v->p2.x && upperX > v->p2.x)){
                // if its outside all the current vector check the next one
            }else if(lowerX >= v->p1.x-eps && upperX <= v->p2.x+eps){
                return;
            }else if(lowerX < v->p1.x && upperX <= v->p2.x){
                // replace values
                v->p1.x = lowerX;
                SDL_SetRenderDrawColor(renderWindow,0,0,255,255);
                SDL_RenderDrawLine(renderWindow,v->p1.x,0,
                            v->p1.x,screenSizeY);
                
                return;
            }else if(lowerX >= v->p1.x && upperX > v->p2.x){
                v->p2.x = upperX;
                SDL_SetRenderDrawColor(renderWindow,230,173,230,255);
                
                SDL_RenderDrawLine(renderWindow,v->p2.x,0,
                            v->p2.x,screenSizeY);
                return;
            }
        }
        SDL_SetRenderDrawColor(renderWindow,255,0,0,255);
        Vector* vector = new Vector;
        vector->p1.x = lowerX;
        vector->p2.x = upperX;
        coveringVectors.push_back(vector);
        SDL_RenderDrawLine(renderWindow,vector->p1.x,0,
                            vector->p1.x,screenSizeY);
                            
        SDL_RenderDrawLine(renderWindow,vector->p2.x,0,
                            vector->p2.x,screenSizeY);
    }
}