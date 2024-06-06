#include"ScreenRenderer.h"

ScreenRenderer::ScreenRenderer(SDL_Renderer* renderWindow, Player* player, float screenSizeX, float screenSizeY, vector<Sector*> sectorData){
    this->renderWindow = renderWindow;
    this->screenSizeX = screenSizeX;
    this->screenSizeY = screenSizeY;
    this->player = player;
    this->fov = player->getFov();
    this->screenDist = screenSizeX/(2*tan(fov));
    this->sectorData = sectorData;
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
    //SDL_RenderDrawLineF(renderWindow,x,y,x+(400*cos(angle+fov)),y+(400*sin(angle+fov))); //Upper line
    //SDL_RenderDrawLineF(renderWindow,x,y,x+(400*cos(angle-fov)),y+(400*sin(angle-fov))); // lower line
}

void ScreenRenderer::renderFOV(queue<Vector*> renderQueue){
    queue<Vector*> vectorQueue = renderQueue;
    occupiedAngle.clear();
    while(!vectorQueue.empty()){
        if(inFOV(vectorQueue.front())){
            //cout << "Rendering: " << vectorQueue.front()->index << " ";
            //SDL_SetRenderDrawColor(renderWindow,210,4,45,255);
            renderWalll(vectorQueue.front());
        }
        vectorQueue.pop();
    }
}

bool ScreenRenderer::inFOV(Vector* vector){
    Point position = player->getPosition();
    float angle = player->getAngle();
    float lowerAngle = normalizeAngle(angle-fov);
    float upperAngle = normalizeAngle(angle+fov);

    float p1Theta = normalizeAngle(atan2(vector->p1.y-position.y,vector->p1.x-position.x)); // angle for the point p1
    float p2Theta = normalizeAngle(atan2(vector->p2.y-position.y,vector->p2.x-position.x)); // angle for the point p2
    
    if(inAngleRange(p1Theta,lowerAngle,upperAngle) || inAngleRange(p2Theta,lowerAngle,upperAngle)){
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
    
    Vector* screenVector = new Vector(position,position);
    screenVector->p1.x += hyp*cos(lowerAngle);
    screenVector->p1.y += hyp*sin(lowerAngle);
    screenVector->p2.x += hyp*cos(upperAngle);
    screenVector->p2.y += hyp*sin(upperAngle);
    
    // gets the intersect of both end points to a vector and angle relative of the player
    float thetaA = normalizeAngle(atan2(vector->p1.y-position.y,vector->p1.x-position.x));
    float thetaB = normalizeAngle(atan2(vector->p2.y-position.y,vector->p2.x-position.x));
    
    // from the inital 2 points, the following will determine where the points end up on the
    // players fov projected on the screen
    if(!inAngleRange(thetaA,lowerAngle,upperAngle) && !inAngleRange(thetaB,lowerAngle,upperAngle)){
        thetaA = lowerAngle;
        thetaB = upperAngle;
    }else if(inAngleRange(thetaA,lowerAngle,upperAngle) && !inAngleRange(thetaB,lowerAngle,upperAngle)){
        if(isIntersectingSeg(vector,position,lowerAngle)){
            thetaB = lowerAngle;
        }else if(isIntersectingSeg(vector,position,upperAngle)){
            thetaB = upperAngle;
        }else{
            thetaB = thetaA;
        }
    }else if(inAngleRange(thetaB,lowerAngle,upperAngle) && !inAngleRange(thetaA,lowerAngle,upperAngle)){
        if(isIntersectingSeg(vector,position,lowerAngle)){
            thetaA = lowerAngle;
        }else if(isIntersectingSeg(vector,position,upperAngle)){
            thetaA = upperAngle;
        }else{
            thetaA = thetaB;
        }
    }
    if(occupiedAngle.empty() || !(occupiedAngle.front()->lowerX <= 0 && occupiedAngle.back()->upperX >= screenSizeX)){
        clipWall(getMinF(thetaA,thetaB),getMaxF(thetaA,thetaB),vector,screenVector);
    }
}

void ScreenRenderer::clipWall(float lowerTheta, float upperTheta, Vector* vector, Vector* screenVector){
    if(!occupiedAngle.empty()){
        for(NumberRange* range:occupiedAngle){
            if(!inAngleRange(lowerTheta,range->lowerX,range->upperX) && !inAngleRange(upperTheta,range->lowerX,range->upperX)){
                if((getAngleDiff(range->lowerX,lowerTheta) < 0 && getAngleDiff(range->upperX,upperTheta) > 0) ||
                    (getAngleDiff(range->lowerX,lowerTheta) > 0 && getAngleDiff(range->upperX,upperTheta) < 0)){
                    clipWall(lowerTheta,range->lowerX,vector,screenVector);
                    clipWall(range->upperX,upperTheta,vector,screenVector);
                    return;
                }
            }else if(inAngleRange(lowerTheta,range->lowerX,range->upperX) && inAngleRange(upperTheta,range->lowerX,range->upperX)){
                return;
            }else if(inAngleRange(upperTheta,range->lowerX,range->upperX) && !inAngleRange(lowerTheta,range->lowerX,range->upperX)){
                if(getAbsAngleDiff(lowerTheta,range->lowerX) < getAbsAngleDiff(lowerTheta,range->upperX)){
                    upperTheta = range->lowerX;
                }else{
                    upperTheta = range->upperX;
                }
            }else if(inAngleRange(lowerTheta,range->lowerX,range->upperX) && !inAngleRange(upperTheta,range->lowerX,range->upperX)){
                if(getAbsAngleDiff(upperTheta,range->lowerX) < getAbsAngleDiff(upperTheta,range->upperX)){
                    lowerTheta = range->lowerX;
                }else{
                    lowerTheta = range->upperX;
                }
            }
        }
    }

    Point position = player->getPosition();
    Point pointA = intersectingPoint(vector,position,lowerTheta);
    Point pointB = intersectingPoint(vector,position,upperTheta);

    Point screenPointA = intersectingPoint(screenVector,position,lowerTheta);
    Point screenPointB = intersectingPoint(screenVector,position,upperTheta); 
    
    float screenXA = pytha(screenPointA.x-screenVector->p1.x,screenPointA.y-screenVector->p1.y);
    float screenXB = pytha(screenPointB.x-screenVector->p1.x,screenPointB.y-screenVector->p1.y); 
    
    float y0Scaler = (pytha(screenPointA.x-position.x,screenPointA.y-position.y)/
                            pytha(pointA.x-position.x,pointA.y-position.y));
    float y1Scaler = (pytha(screenPointB.x-position.x,screenPointB.y-position.y)/
                            pytha(pointB.x-position.x,pointB.y-position.y));

    float x0 = getMinF(screenXA,screenXB);
    float x1 = getMaxF(screenXA,screenXB)-1;

    float floorHeight = sectorData.at(vector->sectorIndex)->floorHeight;
    float ceilingHeight = sectorData.at(vector->sectorIndex)->ceilingHeight;

    SDL_SetRenderDrawColor(renderWindow,255,0,0,255);
    if(vector->portalingSector != -1){
        float floorTemp = sectorData.at(vector->portalingSector)->floorHeight;
        float ceilTemp = sectorData.at(vector->portalingSector)->ceilingHeight;
        SDL_SetRenderDrawColor(renderWindow,155,69,133,255);
        if(x0 == screenXA){
            drawWall(x0,x1,y0Scaler,y1Scaler,floorHeight,-floorTemp);
            SDL_SetRenderDrawColor(renderWindow,155,69,133,255);
            drawWall(x0,x1,y0Scaler,y1Scaler,-ceilTemp,ceilingHeight);
        }else{
            drawWall(x0,x1,y1Scaler,y0Scaler,floorHeight,-floorTemp);
            SDL_SetRenderDrawColor(renderWindow,155,69,133,255);
            drawWall(x0,x1,y1Scaler,y0Scaler,-ceilTemp,ceilingHeight);
        }
    }else{
        if(x0 == screenXA){
        drawWall(x0,x1,y0Scaler,y1Scaler,floorHeight,ceilingHeight);
        }else{
            drawWall(x0,x1,y1Scaler,y0Scaler,floorHeight,ceilingHeight);
        }
        occupiedAngle.push_back(new NumberRange(lowerTheta,upperTheta));
    }

}

// with the use of linear interpolation, draws a filled wall from given values
void ScreenRenderer::drawWall(float x0, float x1, float y0, float y1, float floor, float ceiling){
    float yScaler;
    float yf;
    float yc;
    for(int x = x0; x < x1; x++){
        yScaler = y0+(y1-y0)/(x1-x0)*(x-x0);
        if(yScaler > getMaxF(y0,y1)){
            yScaler = getMaxF(y0,y1);
        }
        if(yScaler < getMinF(y0,y1)){
            yScaler = getMinF(y0,y1);
        }
        yf = screenSizeY/2-yScaler*ceiling; // ceiling height
        yc = screenSizeY/2+yScaler*floor; // floor height
        SDL_RenderDrawLine(renderWindow,x,yf,
                            x,yc);
    }
    
    SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
    SDL_RenderDrawLine(renderWindow,x0,screenSizeY/2-y0*ceiling,
                        x0,screenSizeY/2+y0*floor);
    SDL_RenderDrawLine(renderWindow,x1,screenSizeY/2-y1*ceiling,
                        x1,screenSizeY/2+y1*floor);
}

void ScreenRenderer::drawWall(float x0, float x1, float y0, float y1, float floor, float ceiling, float minY, float maxY){
    float yScaler;
    float yf;
    float yc;
    for(int x = x0; x < x1; x++){
        yScaler = y0+(y1-y0)/(x1-x0)*(x-x0);
        if(yScaler > maxY){
            yScaler = maxY;
        }
        if(yScaler < minY){
            yScaler = minY;
        }
        yf = screenSizeY/2-yScaler*ceiling; // ceiling height
        yc = screenSizeY/2+yScaler*floor; // floor height
        SDL_RenderDrawLine(renderWindow,x,yf,
                            x,yc);
    }
    
    SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
    SDL_RenderDrawLine(renderWindow,x0,screenSizeY/2-y0*ceiling,
                        x0,screenSizeY/2+y0*floor);
    SDL_RenderDrawLine(renderWindow,x1,screenSizeY/2-y1*ceiling,
                        x1,screenSizeY/2+y1*floor);
}

