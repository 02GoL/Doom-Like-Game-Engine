#include"Util.h"

class Maploader{
    private:
        vector<Sector*> sectors;
        vector<Vector*> vectors;
    public:
        void loadData(string dataPath);
        vector<Sector*> getSectorData(){return sectors;}
        vector<Vector*> getVectorData(){return vectors;}

};
