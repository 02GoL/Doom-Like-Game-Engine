#include"Maploader.h"

void Maploader::loadData(string mapDataPath){
    ifstream myFile(mapDataPath);
    string str;
    bool loadingSectorData = false;
    int sectorIndex;
    int i = 1;
    if(myFile.is_open()){
        while(getline(myFile,str)){
            if(str == "[SECTOR]"){
                loadingSectorData = true;
            }else if(loadingSectorData){
                Sector* sector = new Sector();
                stringstream ss(str);
                string metaData;

                ss >> metaData;
                sectorIndex = stoi(metaData);
                sector->sectorIndex = sectorIndex;
                ss >> metaData;
                sector->floorHeight = stoi(metaData);
                ss >> metaData;
                sector->ceilHeight = stoi(metaData);

                sectors.push_back(sector);
                loadingSectorData = false;
            }else if(str != ""){
                Vector* v1 = new Vector();
                stringstream ss(str);
                string metaData;
                
                ss >> metaData;
                v1->p1.x = stoi(metaData);
                ss >> metaData;
                v1->p1.y = stoi(metaData);

                ss >> metaData;    
                v1->p2.x = stoi(metaData);
                ss >> metaData;
                v1->p2.y = stoi(metaData);

                ss >> metaData;
                v1->facingDir = stoi(metaData);
                
                v1->setMidPoint();
                v1->setNormal();

                v1->sectorIndex = sectorIndex;
                v1->vectorIndex = i++;
                
                ss >> metaData;
                v1->portalingSector = stoi(metaData);

                vectors.push_back(v1);
            }
        }
        cout << "Loaded map data...\n";
    }else{
        cout << "Unable to open file...\n";
    }
    myFile.close();
}