#include"Maploader.h"

vector<Vector*> Maploader::loadData(string mapDataPath){
    ifstream myFile(mapDataPath);
    string str;
    int i = 1;
    if(myFile.is_open()){
        while(getline(myFile,str)){
            if(str != ""){
                Vector* v1 = new Vector();
                stringstream ss(str);
                string num;
                
                ss >> num;
                v1->p1.x = stoi(num);
                ss >> num;
                v1->p1.y = stoi(num);
                ss >> num;
                v1->p2.x = stoi(num);
                ss >> num;
                v1->p2.y = stoi(num);
                ss >> num;
                v1->facingDir = stoi(num);
                v1->index = i++;
                
                v1->setMidPoint();
                v1->setNormal();
            
                vectors.push_back(v1);
            }
        }
        cout << "Loaded map data...\n";
    }else{
        cout << "Unable to open file...\n";
    }
    myFile.close();
    return vectors;
}