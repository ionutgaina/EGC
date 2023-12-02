#include "../utils.hpp"
#include "utils.hpp"

using namespace std;

class House : public MyGameObject
{
public:
    House(vector<MyGameObject*> objects)
    {
        this->scale = rand() % 30 + 10;
        this->y = 0.5f * scale;
        radius = 0.5f * scale;

        this->generateXandZ(objects);
    }

    bool verifyInRadius(float x, float z, float radius) override
    {
        float distanceX = abs(this->x - x);
        float distanceZ = abs(this->z - z);

        if (distanceX > (this->radius + radius) || distanceZ > (this->radius + radius))
        {
            return false;
        }

        if (distanceX <= this->radius || distanceZ <= this->radius)
        {
            return true;
        }

        float cornerDistance = pow(distanceX - this->radius, 2) + pow(distanceZ - this->radius, 2);

        return cornerDistance <= pow(radius, 2);
    }
};