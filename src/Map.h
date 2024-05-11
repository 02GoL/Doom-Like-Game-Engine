#include"Util.h"

class Map{
    private:
        vector<Vector*> vectors;
        int cellSize;
    public:
        void loadData(string dataPath);
        vector<Vector*> getMapVectors(){return vectors;};
};
