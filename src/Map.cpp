#include"map.h"

Point getItersection(Vector* v1, Vector* v2){
    Point p;

    int day = v1->p2.y - v1->p1.y;
    int dax = v1->p1.x - v1->p2.x;
    int da = day*v1->p1.x + dax*v1->p1.y;

    int dby = v2->p2.y - v2->p1.y;
    int dbx = v2->p1.x - v2->p2.x;
    int db = dby*v2->p1.x + dbx*v2->p1.y;

    double det = double(day*dbx - dax*dby);

    int x = double(dbx*da - dax*db)/det;
    int y = double(day*db - dby*da)/det;
    p.x = x;
    p.y = y;
    return p;
}

void setMidPoint(Vector* vector){
    vector->midPoint.x = (vector->p1.x+vector->p2.x)/2;
    vector->midPoint.y = (vector->p1.y+vector->p2.y)/2;
}

void setNormal(Vector* vector){
    int a = vector->p2.x - vector->p1.x;
    int b = vector->p2.y - vector->p1.y;
    int uVect = sqrt(pow(a,2) + pow(b,2));
    if(vector->facingDir == 1){ // 1 == inward 0 == outward
        vector->normal.x = vector->midPoint.x + (a*cos(M_PI/2) - b*sin(M_PI/2)) * 10 / uVect;
        vector->normal.y = vector->midPoint.y + (a*sin(M_PI/2) + b*cos(M_PI/2)) * 10 / uVect;
    }else{
        vector->normal.x = vector->midPoint.x - (a*cos(M_PI/2) - b*sin(M_PI/2)) * 10 / uVect;
        vector->normal.y = vector->midPoint.y - (a*sin(M_PI/2) + b*cos(M_PI/2)) * 10 / uVect;
    }
}

void Map::loadData(string mapDataPath){
    ifstream myFile(mapDataPath);
    string str;
    int i = 1;
    if(myFile.is_open()){
        while(getline(myFile,str)){
            if(str != ""){
                Vector* vector = new Vector;
                stringstream ss(str);
                string num;
                
                ss >> num;
                vector->p1.x = stoi(num);
                ss >> num;
                vector->p1.y = stoi(num);
                ss >> num;
                vector->p2.x = stoi(num);
                ss >> num;
                vector->p2.y = stoi(num);
                ss >> num;
                vector->facingDir = stoi(num);
                vector->index = i++;
                
                setMidPoint(vector);
                setNormal(vector);
            
                vectors.push_back(vector);
            }
        }
        cout << "Loaded map data...\n";
    }else{
        cout << "Unable to open file...\n";
    }
    myFile.close();
}
