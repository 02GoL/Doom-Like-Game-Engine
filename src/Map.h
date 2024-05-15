#include"Util.h"

class Map{
    private:
        vector<Vector*> vectors;
    public:
        void loadData(string dataPath);
        vector<Vector*> getMapVectors(){return vectors;};
};
