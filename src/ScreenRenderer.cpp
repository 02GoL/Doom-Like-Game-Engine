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
    //SDL_RenderDrawLineF(renderWindow,x,y,x+(400*cos(angle+fov)),y+(400*sin(angle+fov))); //Upper line
    //SDL_RenderDrawLineF(renderWindow,x,y,x+(400*cos(angle-fov)),y+(400*sin(angle-fov))); // lower line
}

void ScreenRenderer::renderFOV(queue<Vector*> renderQueue){
    queue<Vector*> vectorQueue = renderQueue;
    occupiedAngle.clear();
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
        SDL_SetRenderDrawColor(renderWindow,169,200,65,255);
    }else if(inAngleRange(thetaA,lowerAngle,upperAngle) && !inAngleRange(thetaB,lowerAngle,upperAngle)){
        if(isIntersectingSeg(vector,position,lowerAngle)){
            thetaB = lowerAngle;
        }else if(isIntersectingSeg(vector,position,upperAngle)){
            thetaB = upperAngle;
        }else{
            thetaB = thetaA;
        }
        SDL_SetRenderDrawColor(renderWindow,230,173,230,255);
    }else if(inAngleRange(thetaB,lowerAngle,upperAngle) && !inAngleRange(thetaA,lowerAngle,upperAngle)){
        if(isIntersectingSeg(vector,position,lowerAngle)){
            thetaA = lowerAngle;
        }else if(isIntersectingSeg(vector,position,upperAngle)){
            thetaA = upperAngle;
        }else{
            thetaA = thetaB;
        }
        SDL_SetRenderDrawColor(renderWindow,255,0,0,255);
    }
    clipWall(getMinF(thetaA,thetaB),getMaxF(thetaA,thetaB),vector,screenVector);
}

void ScreenRenderer::clipWall(float lowerTheta, float upperTheta, Vector* vector, Vector* screenVector){
    if(occupiedAngle.empty()){
        occupiedAngle.push_back(new NumberRange(lowerTheta,upperTheta));
    }else{
        for(NumberRange* range:occupiedAngle){
            if(!inAngleRange(lowerTheta,range->lower,range->upper) && !inAngleRange(upperTheta,range->lower,range->upper)){
                if((getAngleDiff(range->lower,lowerTheta) < 0 && getAngleDiff(range->upper,upperTheta) > 0) ||
                    (getAngleDiff(range->lower,lowerTheta) > 0 && getAngleDiff(range->upper,upperTheta) < 0)){
                    clipWall(lowerTheta,range->lower,vector,screenVector);
                    clipWall(range->upper,upperTheta,vector,screenVector);
                    return;
                }
            }else if(inAngleRange(lowerTheta,range->lower,range->upper) && inAngleRange(upperTheta,range->lower,range->upper)){
                return;
            }else if(inAngleRange(upperTheta,range->lower,range->upper) && !inAngleRange(lowerTheta,range->lower,range->upper)){
                if(getAbsAngleDiff(lowerTheta,range->lower) < getAbsAngleDiff(lowerTheta,range->upper)){
                    upperTheta = range->lower;
                }else{
                    upperTheta = range->upper;
                }
            }else if(inAngleRange(lowerTheta,range->lower,range->upper) && !inAngleRange(upperTheta,range->lower,range->upper)){
                if(getAbsAngleDiff(upperTheta,range->lower) < getAbsAngleDiff(upperTheta,range->upper)){
                    lowerTheta = range->lower;
                }else{
                    lowerTheta = range->upper;
                }
            }
        }
        occupiedAngle.push_back(new NumberRange(lowerTheta,upperTheta));
    }

    Point position = player->getPosition();
    Point pointA = intersectingPoint(vector,position,lowerTheta);
    Point pointB = intersectingPoint(vector,position,upperTheta);

    Point screenPointA = intersectingPoint(screenVector,position,lowerTheta);
    Point screenPointB = intersectingPoint(screenVector,position,upperTheta); 
    
    float screenXA = pytha(screenPointA.x-screenVector->p1.x,screenPointA.y-screenVector->p1.y);
    float screenXB = pytha(screenPointB.x-screenVector->p1.x,screenPointB.y-screenVector->p1.y); 
    
    float aYScaler = 30*(pytha(screenPointA.x-position.x,screenPointA.y-position.y)/
                            pytha(pointA.x-position.x,pointA.y-position.y));
    float bYScaler = 30*(pytha(screenPointB.x-position.x,screenPointB.y-position.y)/
                            pytha(pointB.x-position.x,pointB.y-position.y));

    float lowerX = getMinF(screenXA,screenXB);
    float upperX = getMaxF(screenXA,screenXB)-1;

    if(lowerX == screenXA){
        drawWall(lowerX,upperX,aYScaler,bYScaler);
    }else{
        drawWall(lowerX,upperX,bYScaler,aYScaler);
    }
    return;
}

// with the use of linear interpolation, draws a filled wall from given values
void ScreenRenderer::drawWall(float x1, float x2, float y1, float y2){
    float y;
    for(int x = x1; x < x2; x++){
        y = y1+((y2-y1)/(x2-x1))*(x-x1);
        if(y > getMaxF(y1,y2)){
            y = getMaxF(y1,y2);
        }
        if(y < getMinF(y1,y2)){
            y = getMinF(y1,y2);
        }
        SDL_RenderDrawLine(renderWindow,x,screenSizeY/2-y,
                        x,screenSizeY/2+y);
    }
    /*
    SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
    SDL_RenderDrawLine(renderWindow,x1,screenSizeY/2-y1,
                        x1,screenSizeY/2+y1);
    SDL_RenderDrawLine(renderWindow,x2,screenSizeY/2-y2,
                        x2,screenSizeY/2+y2);
                        */
}
