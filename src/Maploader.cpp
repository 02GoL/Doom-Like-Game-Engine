#include"Maploader.h"

vector<Vector*> Maploader::loadData(string mapDataPath){
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
    return vectors;
}