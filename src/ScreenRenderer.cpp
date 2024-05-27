#include"ScreenRenderer.h"

ScreenRenderer::ScreenRenderer(SDL_Renderer* renderWindow, Player* player, float screenSizeX, float screenSizeY){
    this->renderWindow = renderWindow;
    this->screenSizeX = screenSizeX;
    this->screenSizeY = screenSizeY;
    this->player = player;
    this->fov = player->getFov();
    this->screenDist = screenSizeX/(2*tan(fov));
}

void ScreenRenderer::renderTopDown(queue<Vector*> renderQueue){
    queue<Vector*> vectorQueue = renderQueue;
    while(!vectorQueue.empty()){
        if(inFOV(vectorQueue.front())){
            SDL_SetRenderDrawColor(renderWindow,210,4,45,255);
        }else{
            SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
        }
        // renders vector
        SDL_RenderDrawLineF(renderWindow,vectorQueue.front()->p1.x,vectorQueue.front()->p1.y,
                            vectorQueue.front()->p2.x,vectorQueue.front()->p2.y);
        // renders normal
        SDL_RenderDrawLineF(renderWindow,vectorQueue.front()->midPoint.x,vectorQueue.front()->midPoint.y,
                            vectorQueue.front()->normal.x,vectorQueue.front()->normal.y);
        vectorQueue.pop();
    }
    renderPlayer();
}

void ScreenRenderer::renderPlayer(){
    SDL_FRect playerSprite = player->getPlayerSprite();
    float angle = player->getAngle();
    float x = player->getPosition().x;
    float y = player->getPosition().y;

    SDL_SetRenderDrawColor(renderWindow,0,255,0,255);
    SDL_RenderFillRectF(renderWindow,&playerSprite);

    SDL_RenderDrawLineF(renderWindow,x,y,x+(200*cos(angle)),y+(200*sin(angle))); // Middle line
    SDL_RenderDrawLineF(renderWindow,x,y,x+(400*cos(angle+fov)),y+(400*sin(angle+fov))); //Upper line
    SDL_RenderDrawLineF(renderWindow,x,y,x+(400*cos(angle-fov)),y+(400*sin(angle-fov))); // lower line
}

void ScreenRenderer::renderFOV(queue<Vector*> renderQueue){
    queue<Vector*> vectorQueue = renderQueue;
    coveringVectors.clear();
    while(!vectorQueue.empty()){
        if(inFOV(vectorQueue.front())){
            //cout << "Rendering: " << vectorQueue.front()->index << " ";
            SDL_SetRenderDrawColor(renderWindow,210,4,45,255);
            renderWalll(vectorQueue.front());
        }else{
            SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
        }
        vectorQueue.pop();
    }
}

bool ScreenRenderer::inFOV(Vector* vector){
    Point position = player->getPosition();
    float angle = player->getAngle();
    float lowerAngle = normalizeAngle(angle-fov);
    float upperAngle = normalizeAngle(angle+fov);

    float alpha = normalizeAngle(atan2(vector->p1.y-position.y,vector->p1.x-position.x)); // angle for the point p1
    float beta = normalizeAngle(atan2(vector->p2.y-position.y,vector->p2.x-position.x)); // angle for the point p2
    
    if(inAngleRange(alpha,lowerAngle,upperAngle) || inAngleRange(beta,lowerAngle,upperAngle)){
        // if the angle of p1 or p2 relative to the player is in the fov, draw
        return true;
    }
    if(isIntersectingSeg(vector,position,angle)){
        // if both points of p1 and p2 are outside and there is a wall infront, draw
        return true;
    }
    return false;
}

void ScreenRenderer::renderWalll(Vector* vector){
    Point position = player->getPosition();
    float angle = player->getAngle();
    float hyp = pytha(screenDist,screenSizeX/2);
    float lowerAngle = normalizeAngle(angle-fov);
    float upperAngle = normalizeAngle(angle+fov);
    
    Vector* screenVector = new Vector;
    screenVector->p1 = position;
    screenVector->p1.x += hyp*cos(lowerAngle);
    screenVector->p1.y += hyp*sin(lowerAngle);
    screenVector->p2 = position;
    screenVector->p2.x += hyp*cos(upperAngle);
    screenVector->p2.y += hyp*sin(upperAngle);
    
    // gets the intersect of both end points to a vector and angle relative of the player
    Point pointA = vector->p1;
    Point pointB = vector->p2;
    float thetaA = normalizeAngle(atan2(pointA.y-position.y,pointA.x-position.x));
    float thetaB = normalizeAngle(atan2(pointB.y-position.y,pointB.x-position.x));
    
    // from the inital 2 points, the following will determine where the points end up on the
    // players fov projected on the screen
    if(!inAngleRange(thetaA,lowerAngle,upperAngle) && !inAngleRange(thetaB,lowerAngle,upperAngle)){
        // if both points are outside the fov reset both points to be the sides of the fov
        pointA = intersectingPoint(vector,position,lowerAngle);
        pointB = intersectingPoint(vector,position,upperAngle);
        thetaA = normalizeAngle(atan2(pointA.y-position.y,pointA.x-position.x));
        thetaB = normalizeAngle(atan2(pointB.y-position.y,pointB.x-position.x));
        SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
    }else if(inAngleRange(thetaA,lowerAngle,upperAngle) && !inAngleRange(thetaB,lowerAngle,upperAngle)){
        if(isIntersectingSeg(vector,position,lowerAngle) && !isIntersectingSeg(vector,position,upperAngle)){
            pointB = intersectingPoint(vector,position,lowerAngle);
        }else if(isIntersectingSeg(vector,position,upperAngle) && !isIntersectingSeg(vector,position,lowerAngle)){
            pointB = intersectingPoint(vector,position,upperAngle);
        }else{
            cout << "something went wrong with setting B\n";
        }
        //thetaB = normalizeAngle(atan2(pointB.y-position.y,pointB.x-position.x));
        SDL_SetRenderDrawColor(renderWindow,230,173,230,255);
    }else if(inAngleRange(thetaB,lowerAngle,upperAngle) && !inAngleRange(thetaA,lowerAngle,upperAngle)){
        if(isIntersectingSeg(vector,position,lowerAngle) && !isIntersectingSeg(vector,position,upperAngle)){
            pointA = intersectingPoint(vector,position,lowerAngle);
        }else if(isIntersectingSeg(vector,position,upperAngle) && !isIntersectingSeg(vector,position,lowerAngle)){
            pointA = intersectingPoint(vector,position,upperAngle);
        }else{
            cout << "something went wrong with setting A\n";
        }
        //thetaA = normalizeAngle(atan2(pointA.y-position.y,pointA.x-position.x));
        SDL_SetRenderDrawColor(renderWindow,255,0,0,255);
    }
    
    float distA = pytha(pointA.x-position.x,pointA.y-position.y);
    float distB = pytha(pointB.x-position.x,pointB.y-position.y);
    pointA = intersectingPoint(screenVector,position,pointA);
    pointB = intersectingPoint(screenVector,position,pointB); 
    float screenDistA = pytha(pointA.x-position.x,pointA.y-position.y);
    float screenDistB = pytha(pointB.x-position.x,pointB.y-position.y);
    
    float aYScaler = 20*(screenDistA/distA);
    float bYScaler = 20*(screenDistB/distB);
    
    float screenPointA = pytha(pointA.x-screenVector->p1.x,pointA.y-screenVector->p1.y);
    float screenPointB = pytha(pointB.x-screenVector->p1.x,pointB.y-screenVector->p1.y);      
    drawClipWall(screenPointA,screenPointB,aYScaler,bYScaler);
}

void ScreenRenderer::drawClipWall(float aScreenPoint, float bScreenPoint, float aYScaler, float bYScaler){
    float lowerX = getMinF(aScreenPoint,bScreenPoint);
    float upperX = getMaxF(aScreenPoint,bScreenPoint);
    float lowerY;
    float upperY;
    if(lowerX == aScreenPoint){
        lowerY = aYScaler;
        upperY = bYScaler;
    }else{
        lowerY = bYScaler;
        upperY = aYScaler;
    }

    float x = abs(upperX-lowerX);
    float y = abs(aYScaler-bYScaler);
    float theta = atan(y/x);

    if(coveringVectors.empty()){
        Vector* vector = new Vector;
        vector->p1.x = lowerX;
        vector->p2.x = upperX;
        coveringVectors.push_back(vector);
    }else{
        for(Vector* v:coveringVectors){
            if(!inRange(lowerX,v->p1.x,v->p2.x) && !inRange(upperX,v->p1.x,v->p2.x)){
                // if its outside all the current vector check the next one

                if(inRange(lowerX,0,v->p1.x) && inRange(upperX,v->p2.x,screenSizeX)){
                    drawClipWall(lowerX,v->p1.x,aYScaler,bYScaler);
                    lowerX = v->p2.x;
                }
            }else if(inRange(lowerX,v->p1.x,v->p2.x) && inRange(upperX,v->p1.x,v->p2.x)){
                return;
            }else if(inRange(upperX,v->p1.x,v->p2.x) && !inRange(lowerX,v->p1.x,v->p2.x)){
                float xDiff = abs(upperX-v->p1.x);
                float h = xDiff*tan(theta);
                lowerY += h;
                upperY += h;
                upperX = v->p1.x;
                //recalc bScaler
            }else if(inRange(lowerX,v->p1.x,v->p2.x) && !inRange(upperX,v->p1.x,v->p2.x)){
                float xDiff = abs(lowerX-v->p2.x);
                float h = xDiff*tan(theta);
                lowerY += h;
                upperY += h;
                lowerX = v->p2.x;
                //recalc aScaler
            }
        }
        SDL_SetRenderDrawColor(renderWindow,255,0,0,255);
        Vector* vector = new Vector;
        vector->p1.x = lowerX;
        vector->p2.x = upperX;
        coveringVectors.push_back(vector);                            
    }
    SDL_RenderDrawLine(renderWindow,coveringVectors.back()->p1.x,screenSizeY/2-lowerY,
                        coveringVectors.back()->p1.x,screenSizeY/2+lowerY);
    SDL_RenderDrawLine(renderWindow,coveringVectors.back()->p2.x,screenSizeY/2-upperY,
                        coveringVectors.back()->p2.x,screenSizeY/2+upperY);

    SDL_RenderDrawLine(renderWindow,coveringVectors.back()->p1.x,screenSizeY/2-lowerY,
                        coveringVectors.back()->p2.x,screenSizeY/2-upperY);
    SDL_RenderDrawLine(renderWindow,coveringVectors.back()->p1.x,screenSizeY/2+lowerY,
                        coveringVectors.back()->p2.x,screenSizeY/2+upperY);
}