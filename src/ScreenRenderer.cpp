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

void ScreenRenderer::renderTopDown(queue<Vector*> vectorQueue){
    queue<Vector*> renderQueue = vectorQueue;
    while(!renderQueue.empty()){
        if(inFOV(renderQueue.front())){
            SDL_SetRenderDrawColor(renderWindow,210,4,45,255);
        }else{
            SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
        }
        // renders vector
        SDL_RenderDrawLineF(renderWindow,renderQueue.front()->p1.x,renderQueue.front()->p1.y,
                            renderQueue.front()->p2.x,renderQueue.front()->p2.y);
        // renders normal
        SDL_RenderDrawLineF(renderWindow,renderQueue.front()->midPoint.x,renderQueue.front()->midPoint.y,
                            renderQueue.front()->normal.x,renderQueue.front()->normal.y);
        renderQueue.pop();
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

void ScreenRenderer::renderFOV(queue<Vector*> vectorQueue){
    queue<Vector*> renderQueue = vectorQueue;
    renderedSection.clear();
    //cout << "Start of render\n";
    while(!renderQueue.empty()){
        if(inFOV(renderQueue.front())){
            //SDL_SetRenderDrawColor(renderWindow,210,4,45,255);
            renderWall(renderQueue.front());
        }
        renderQueue.pop();
    }
    //cout << "\n";
}

bool ScreenRenderer::inFOV(Vector* vector){
    Point playerPos = player->getPosition();
    float angle = player->getAngle();
    float lowerAngle = normalizeAngle(angle-fov);
    float upperAngle = normalizeAngle(angle+fov);

    float p1Theta = normalizeAngle(atan2(vector->p1.y-playerPos.y,vector->p1.x-playerPos.x)); // angle for the point p1
    float p2Theta = normalizeAngle(atan2(vector->p2.y-playerPos.y,vector->p2.x-playerPos.x)); // angle for the point p2
    
    if((inAngleRange(p1Theta,lowerAngle,upperAngle) || inAngleRange(p2Theta,lowerAngle,upperAngle)) && 
        !(isIntersectingSeg(vector,playerPos,vector->normal))){
        // if the angle of p1 or p2 relative to the player is in the fov, draw
        return true;
    }else if(isIntersectingSeg(vector,playerPos,angle) && !isIntersectingSeg(vector,playerPos,vector->normal)){
        // if both points of p1 and p2 are outside and there is a wall infront, draw
        return true;
    }else{
        return false;
    }
}

void ScreenRenderer::renderWall(Vector* vector){
    Point playerPos = player->getPosition();
    float angle = player->getAngle();
    float hyp = pytha(screenDist,screenSizeX/2);
    float lowerAngle = normalizeAngle(angle-fov);
    float upperAngle = normalizeAngle(angle+fov);
    
    Vector* screenVector = new Vector(playerPos,playerPos);
    screenVector->p1.x += hyp*cos(lowerAngle);
    screenVector->p1.y += hyp*sin(lowerAngle);
    screenVector->p2.x += hyp*cos(upperAngle);
    screenVector->p2.y += hyp*sin(upperAngle);
    
    // gets the intersect of both end points to a vector and angle relative of the player
    float thetaA = normalizeAngle(atan2(vector->p1.y-playerPos.y,vector->p1.x-playerPos.x));
    float thetaB = normalizeAngle(atan2(vector->p2.y-playerPos.y,vector->p2.x-playerPos.x));
    
    // from the inital 2 points, the following will determine where the points end up on the
    // players fov projected on the screen
    if(!inAngleRange(thetaA,lowerAngle,upperAngle) && !inAngleRange(thetaB,lowerAngle,upperAngle)){
        thetaA = lowerAngle;
        thetaB = upperAngle;
    }else if(inAngleRange(thetaA,lowerAngle,upperAngle) && !inAngleRange(thetaB,lowerAngle,upperAngle)){
        if(isIntersectingSeg(vector,playerPos,lowerAngle) && isIntersectingSeg(vector,playerPos,upperAngle)){
            thetaA = lowerAngle;
            thetaB = upperAngle;
        }else if(isIntersectingSeg(vector,playerPos,lowerAngle)){
            thetaB = lowerAngle;
        }else if(isIntersectingSeg(vector,playerPos,upperAngle)){
            thetaB = upperAngle;
        }else{
            thetaB = thetaA;
        }
    }else if(inAngleRange(thetaB,lowerAngle,upperAngle) && !inAngleRange(thetaA,lowerAngle,upperAngle)){
        if(isIntersectingSeg(vector,playerPos,lowerAngle) && isIntersectingSeg(vector,playerPos,upperAngle)){
            thetaA = lowerAngle;
            thetaB = upperAngle;
        }else if(isIntersectingSeg(vector,playerPos,lowerAngle)){
            thetaA = lowerAngle;
        }else if(isIntersectingSeg(vector,playerPos,upperAngle)){
            thetaA = upperAngle;
        }else{
            thetaA = thetaB;
        }
    }

    if(renderedSection.empty() || !(renderedSection.front()->t0 <= 0 && renderedSection.back()->t1 >= screenSizeX)){
        clipWall(getMinF(thetaA,thetaB),getMaxF(thetaA,thetaB),vector,screenVector);
    }
}
/*

void ScreenRenderer::clipWall(float lowerTheta, float upperTheta, Vector* vector, Vector* screenVector){
    if(!renderedAngle.empty()){
        for(NumberRange* range:renderedAngle){
            if(!range->portal){
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
    }
    if(lowerTheta == upperTheta){
        return;
    }

    Point position = player->getPosition();
    Point pointA = intersectingPoint(vector,position,lowerTheta);
    Point pointB = intersectingPoint(vector,position,upperTheta);

    Point screenPointA = intersectingPoint(screenVector,position,lowerTheta);
    Point screenPointB = intersectingPoint(screenVector,position,upperTheta); 
    
    float screenXA = pytha(screenPointA.x-screenVector->p1.x,screenPointA.y-screenVector->p1.y);
    float screenXB = pytha(screenPointB.x-screenVector->p1.x,screenPointB.y-screenVector->p1.y); 
    
    float screenYA = (pytha(screenPointA.x-position.x,screenPointA.y-position.y)/
                            pytha(pointA.x-position.x,pointA.y-position.y));
    float screenYB = (pytha(screenPointB.x-position.x,screenPointB.y-position.y)/
                            pytha(pointB.x-position.x,pointB.y-position.y));

    float floorHeight = sectorData.at(vector->sectorIndex)->floorHeight;
    float ceilingHeight = sectorData.at(vector->sectorIndex)->ceilingHeight;

    float x0 = getMinF(screenXA,screenXB);
    float x1 = getMaxF(screenXA,screenXB)-1;
    float y0;
    float y1;
    if(x0 == screenXA){
        y0 = screenYA;
        y1 = screenYB;
    }else{
        y0 = screenYB;
        y1 = screenYA;
    }

    SDL_SetRenderDrawColor(renderWindow,255,0,0,255);
    if(vector->portalingSector != -1){
        float floorTemp = sectorData.at(vector->portalingSector)->floorHeight;
        float ceilTemp = sectorData.at(vector->portalingSector)->ceilingHeight;

        SDL_SetRenderDrawColor(renderWindow,155,69,133,255);
        drawWall(x0,x1,y0,y1,floorHeight,-floorTemp);


        SDL_SetRenderDrawColor(renderWindow,155,69,133,255);
        drawWall(x0,x1,y0,y1,-ceilTemp,ceilingHeight);


        renderedAngle.push_back(new NumberRange(lowerTheta,upperTheta,true));
    }else{
        drawWall(x0,x1,y0,y1,floorHeight,ceilingHeight);


        renderedAngle.push_back(new NumberRange(lowerTheta,upperTheta,false));
    }
    SDL_SetRenderDrawColor(renderWindow,66,12,200,255);
    drawWall(x0,x1,y0,y1,screenSizeY/2*floorHeight,-floorHeight);

    SDL_SetRenderDrawColor(renderWindow,66,12,200,255);
    drawWall(x0,x1,y0,y1,-ceilingHeight,screenSizeY/2*ceilingHeight);

}
*/

void ScreenRenderer::clipWall(float lowerTheta, float upperTheta, Vector* vector, Vector* screenVector){
    RenderedSection* temp = new RenderedSection();
    bool inPortal = false;
    bool isPortal = false;
    if(!renderedSection.empty()){
        for(RenderedSection* range:renderedSection){
            if(!inAngleRange(lowerTheta,range->t0,range->t1) && !inAngleRange(upperTheta,range->t0,range->t1)){
                if((getAngleDiff(range->t0,lowerTheta) < 0 && getAngleDiff(range->t1,upperTheta) > 0) ||
                    (getAngleDiff(range->t0,lowerTheta) > 0 && getAngleDiff(range->t1,upperTheta) < 0)){
                    clipWall(lowerTheta,range->t0,vector,screenVector);
                    clipWall(range->t1,upperTheta,vector,screenVector);
                    return;
                }
            }else if(inAngleRange(lowerTheta,range->t0,range->t1) && inAngleRange(upperTheta,range->t0,range->t1)){
                if(range->portal){ // currently we are double rendering both front and back sides of a portal
                    temp = range;
                    inPortal = true;
                }else{
                    return;
                }
            }else if(inAngleRange(upperTheta,range->t0,range->t1) && !inAngleRange(lowerTheta,range->t0,range->t1)){
                if(getAbsAngleDiff(lowerTheta,range->t0) < getAbsAngleDiff(lowerTheta,range->t1)){
                    if(range->portal){
                        clipWall(range->t0,upperTheta,vector,screenVector);
                    }
                    upperTheta = range->t0;
                }else{
                    if(range->portal){
                        clipWall(range->t1,upperTheta,vector,screenVector);
                    }
                    upperTheta = range->t1;
                }
            }else if(inAngleRange(lowerTheta,range->t0,range->t1) && !inAngleRange(upperTheta,range->t0,range->t1)){
                if(getAbsAngleDiff(upperTheta,range->t0) < getAbsAngleDiff(upperTheta,range->t1)){
                    if(range->portal){
                        clipWall(lowerTheta,range->t0,vector,screenVector);
                    }
                    lowerTheta = range->t0;
                }else{
                    if(range->portal){
                        clipWall(lowerTheta,range->t1,vector,screenVector);
                    }
                    lowerTheta = range->t1;
                }
            }
        }
    }

    Point position = player->getPosition();
    // point of where the vetors lie in the fov
    Point lowerPoint = intersectingPoint(vector,position,lowerTheta);
    Point upperPoint = intersectingPoint(vector,position,upperTheta);
    
    // point where the vector lies on the projected screen infront of the player
    Point sp1 = intersectingPoint(screenVector,position,lowerTheta);
    Point sp2 = intersectingPoint(screenVector,position,upperTheta); 
    
    float x0 = pytha(sp1.x-screenVector->p1.x,sp1.y-screenVector->p1.y);
    float x1 = pytha(sp2.x-screenVector->p1.x,sp2.y-screenVector->p1.y);

    float y0 = (pytha(sp1.x-position.x,sp1.y-position.y)/
                            pytha(lowerPoint.x-position.x,lowerPoint.y-position.y));
    float y1 = (pytha(sp2.x-position.x,sp2.y-position.y)/
                            pytha(upperPoint.x-position.x,upperPoint.y-position.y));

    Point* p0;
    Point* p1;
    if(getMinF(x0,x1) == x0){
        p0 = new Point(x0,y0);
        p1 = new Point(x1,y1);
    }else{
        p0 = new Point(x1,y1);
        p1 = new Point(x0,y0);
    }

    // get the heights of the sector that the vector belongs in
    float floorY0 = sectorData.at(vector->sectorIndex)->floorHeight;
    float ceilY0 = sectorData.at(vector->sectorIndex)->ceilHeight;
    float slope = (p1->y-p0->y)/(p1->x-p0->x);

    SDL_SetRenderDrawColor(renderWindow,255,0,0,255);
    if(inPortal){
        if(vector->portalingSector != -1){
            float floorY1 = sectorData.at(vector->portalingSector)->floorHeight;
            float ceilY1 = sectorData.at(vector->portalingSector)->ceilHeight;

            SDL_SetRenderDrawColor(renderWindow,155,69,133,255);
            drawWall(p0,p1,slope,getMaxF(floorY0,floorY1),-getMinF(floorY0,floorY1),temp);

            SDL_SetRenderDrawColor(renderWindow,155,69,133,255);
            drawWall(p0,p1,slope,-getMinF(ceilY0,ceilY1),getMax(ceilY0,ceilY1),temp);

            SDL_SetRenderDrawColor(renderWindow,66,12,200,255);
            drawWall(p0,p1,slope,screenSizeY/2*floorY0,-floorY0,temp);

            SDL_SetRenderDrawColor(renderWindow,66,12,200,255);
            drawWall(p0,p1,slope,-ceilY0,screenSizeY/2*ceilY0,temp);
            
            floorY0 = getMinF(floorY0,floorY1);
            ceilY0 = getMinF(ceilY0,ceilY1);
            isPortal = true;
        }else{
            drawWall(p0,p1,slope,floorY0,ceilY0,temp);
            SDL_SetRenderDrawColor(renderWindow,66,12,200,255);
            drawWall(p0,p1,slope,screenSizeY/2*floorY0,-floorY0,temp);

            SDL_SetRenderDrawColor(renderWindow,66,12,200,255);
            drawWall(p0,p1,slope,-ceilY0,screenSizeY/2*ceilY0,temp);
        }
    }else if(vector->portalingSector != -1){
        // get the height of the portaling sector the vector portals with
        float floorY1 = sectorData.at(vector->portalingSector)->floorHeight;
        float ceilY1 = sectorData.at(vector->portalingSector)->ceilHeight;

        SDL_SetRenderDrawColor(renderWindow,155,69,133,255);
        drawWall(p0,p1,slope,getMaxF(floorY0,floorY1),-getMinF(floorY0,floorY1));

        SDL_SetRenderDrawColor(renderWindow,155,69,133,255);
        drawWall(p0,p1,slope,-getMinF(ceilY0,ceilY1),getMax(ceilY0,ceilY1));

        SDL_SetRenderDrawColor(renderWindow,66,12,200,255);
        drawWall(p0,p1,slope,screenSizeY/2*floorY0,-floorY0);

        SDL_SetRenderDrawColor(renderWindow,66,12,200,255);
        drawWall(p0,p1,slope,-ceilY0,screenSizeY/2*ceilY0);
        
        floorY0 = getMinF(floorY0,floorY1);
        ceilY0 = getMinF(ceilY0,ceilY1);
        isPortal = true;
    }else{
        drawWall(p0,p1,slope,floorY0,ceilY0);
        SDL_SetRenderDrawColor(renderWindow,66,12,200,255);
        drawWall(p0,p1,slope,screenSizeY/2*floorY0,-floorY0);

        SDL_SetRenderDrawColor(renderWindow,66,12,200,255);
        drawWall(p0,p1,slope,-ceilY0,screenSizeY/2*ceilY0);
    }
    renderedSection.push_back(
        new RenderedSection(lowerTheta,upperTheta,x0,y0,slope,floorY0,ceilY0,isPortal)
    );
}

// with the use of linear interpolation, draws a filled wall from given values
void ScreenRenderer::drawWall(Point* p0, Point* p1, float slope, float floor, float ceiling){
    for(int x = p0->x; x < p1->x; x++){
        float yScaler = p0->y+slope*(x-p0->x);
        if(yScaler > getMaxF(p0->y,p1->y)){
            yScaler = getMaxF(p0->y,p1->y);
        }
        if(yScaler < getMinF(p0->y,p1->y)){
            yScaler = getMinF(p0->y,p1->y);
        }
        float yf = screenSizeY/2-yScaler*ceiling; // ceiling height
        float yc = screenSizeY/2+yScaler*floor; // floor height
        SDL_RenderDrawLine(renderWindow,x,yf,x,yc);
    }
    /*
    SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
    SDL_RenderDrawLine(renderWindow,p0->x,screenSizeY/2-p0->y*ceiling,
                        p0->x,screenSizeY/2+p0->y*floor);
    SDL_RenderDrawLine(renderWindow,p1->x,screenSizeY/2-p1->y*ceiling,
                        p1->x,screenSizeY/2+p1->y*floor);
    */                  
}   

void ScreenRenderer::drawWall(Point* p0, Point* p1, float slope, float floor, float ceiling, RenderedSection* portalingSector){
    for(int x = p0->x; x < p1->x; x++){
        float yScaler = p0->y+slope*(x-p0->x);
        if(yScaler > getMaxF(p0->y,p1->y)){
            yScaler = getMaxF(p0->y,p1->y);
        }
        if(yScaler < getMinF(p0->y,p1->y)){
            yScaler = getMinF(p0->y,p1->y);
        }
        float yc = screenSizeY/2-yScaler*ceiling; // ceiling height
        float yf = screenSizeY/2+yScaler*floor; // floor height

        float y2Scaler = portalingSector->y0+portalingSector->slope*(x-portalingSector->x0);
        float yc2 = screenSizeY/2-y2Scaler*portalingSector->ceiling;
        float yf2 = screenSizeY/2+y2Scaler*portalingSector->floor;
        if(yc < yc2){
            yc = yc2;
        }
        if(yf > yf2){
            yf = yf2;
        }
        if(yc < yf){
            SDL_RenderDrawLine(renderWindow,x,yc,
                            x,yf);
        }
    }
    /*
    SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
    float yScaler = p0->y;
    if(yScaler > getMaxF(p0->y,p1->y)){
        yScaler = getMaxF(p0->y,p1->y);
    }
    if(yScaler < getMinF(p0->y,p1->y)){
        yScaler = getMinF(p0->y,p1->y);
    }
    float yc = screenSizeY/2-yScaler*ceiling; // ceiling height
    float yf = screenSizeY/2+yScaler*floor; // floor height

    float y2Scaler = portalingSector->y0+portalingSector->slope*(p0->x-portalingSector->x0);
    float yc2 = screenSizeY/2-y2Scaler*portalingSector->ceiling;
    float yf2 = screenSizeY/2+y2Scaler*portalingSector->floor;
    if(yc < yc2){
        yc = yc2;
    }
    if(yf > yf2){
        yf = yf2;
    }
    if(yc < yf){
        SDL_RenderDrawLine(renderWindow,p0->x,yc,
                            p0->x,yf);
    }
    SDL_RenderDrawLine(renderWindow,p0->x,screenSizeY/2-(portalingSector->y0+portalingSector->slope*(p0->x-portalingSector->x0))*portalingSector->ceiling,
                        p1->x,screenSizeY/2-(portalingSector->y0+portalingSector->slope*(p1->x-portalingSector->x0))*portalingSector->ceiling);

    yScaler = p0->y+slope*(p1->x-p0->x);
    if(yScaler > getMaxF(p0->y,p1->y)){
        yScaler = getMaxF(p0->y,p1->y);
    }
    if(yScaler < getMinF(p0->y,p1->y)){
        yScaler = getMinF(p0->y,p1->y);
    }
    yc = screenSizeY/2-yScaler*ceiling; // ceiling height
    yf = screenSizeY/2+yScaler*floor; // floor height

    y2Scaler = portalingSector->y0+portalingSector->slope*(p1->x-portalingSector->x0);
    yc2 = screenSizeY/2-y2Scaler*portalingSector->ceiling;
    yf2 = screenSizeY/2+y2Scaler*portalingSector->floor;
    if(yc < yc2){
        yc = yc2;
    }
    if(yf > yf2){
        yf = yf2;
    }
    if(yc < yf){
        SDL_RenderDrawLine(renderWindow,p1->x,yc,
                            p1->x,yf);
    }
    SDL_RenderDrawLine(renderWindow,p0->x,screenSizeY/2+(portalingSector->y0+portalingSector->slope*(p0->x-portalingSector->x0))*portalingSector->floor,
                        p1->x,screenSizeY/2+(portalingSector->y0+portalingSector->slope*(p1->x-portalingSector->x0))*portalingSector->floor);
    
    SDL_SetRenderDrawColor(renderWindow,255,255,255,255);
    SDL_RenderDrawLine(renderWindow,x0,screenSizeY/2-y0*ceiling,
                        x0,screenSizeY/2+y0*floor);
    SDL_RenderDrawLine(renderWindow,x1,screenSizeY/2-y1*ceiling,
                        x1,screenSizeY/2+y1*floor);
                        
                       */
                        
}

