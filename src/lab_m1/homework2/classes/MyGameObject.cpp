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

    bool verifyInRadius(float x, float z, float radius)
    {
        float distance = sqrt(pow(this->x - x, 2) + pow(this->z - z, 2));
        cout << distance << " " << radius <<  " " << this->radius << endl;
        return distance < radius + this->radius;
    }

    bool verifyInSquare(float x, float z, float lengthSquare)
    {
        float distanceX = abs(this->x - x);
        float distanceZ = abs(this->z - z);

        if (distanceX > (lengthSquare + this->radius) || distanceZ > (lengthSquare + this->radius))
        {
            return false;
        }

        if (distanceX <= lengthSquare || distanceZ <= lengthSquare)
        {
            return true;
        }

        float cornerDistance = pow(distanceX - lengthSquare, 2) + pow(distanceZ - lengthSquare, 2);

        return cornerDistance <= pow(this->radius, 2);
    }

    void generateXandZ(vector<MyGameObject> &objects) {
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
                if (object.verifyInRadius(this->x, this->z, this->radius))
                {   
                    ok = 1;
                    break;
                }
                ok = 0;
            }
        }
    }

    MyGameObject* getCollisionObject(vector<MyGameObject*> objects)
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
};