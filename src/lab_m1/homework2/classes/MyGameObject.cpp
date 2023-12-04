#include "../utils.hpp"

using namespace std;
class MyGameObject
{
public:
    float x = 0;
    float y = 0;
    float z = 0;
    float scale = 1;
    float radius = 0;
    int health = 0;

    virtual bool verifyInRadius(float x, float z, float radius)
    {
        float distance = sqrt(pow(this->x - x, 2) + pow(this->z - z, 2));
        return distance < radius + this->radius;
    }

    void generateXandZ(vector<MyGameObject *> objects)
    {
        random_device rd;
        mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(-TERRAIN_SIZE, TERRAIN_SIZE);

        int ok = 1;

        if (objects.size() == 0)
        {
            this->x = distribution(gen);
            this->z = distribution(gen);
        }

        while (ok)
        {
            this->x = distribution(gen);
            this->z = distribution(gen);

            for (auto object : objects)
            {
                if (this->verifyInRadius(object->x, object->z, object->radius))
                {
                    ok = 1;
                    break;
                }
                ok = 0;
            }
        }
    }

    MyGameObject *getCollisionObject(vector<MyGameObject *> objects)
    {
        for (auto object : objects)
        {
            if (object->verifyInRadius(this->x, this->z, this->radius))
            {
                return object;
            }
        }
        return NULL;
    }

    float calculateAngle(float x1, float y1, float x2, float y2)
    {
        return atan2(y2 - y1, x2 - x1) / M_PI * 180;
    }

    bool verifyWorldLimit()
    {   
        float WORLD_LIMIT = TERRAIN_SIZE + TERRAIN_SIZE/2;
        if (this->x > WORLD_LIMIT || this->x < -WORLD_LIMIT || this->z > WORLD_LIMIT || this->z < -WORLD_LIMIT)
        {
            return false;
        }

        return true;
    }
};