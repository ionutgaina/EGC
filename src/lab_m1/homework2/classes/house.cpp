#include "../utils.hpp"
#include "utils.hpp"

using namespace std;

class House : public MyGameObject
{
public:
    House(vector<MyGameObject> &objects)
    {
        this->scale = rand() % 30 + 10;
        this->y = 0.5f * scale;
        this->radius = 0.5f * scale;

        generateXandZ(objects);
    }
};