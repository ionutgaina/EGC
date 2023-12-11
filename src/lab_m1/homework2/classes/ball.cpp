#include "../utils.hpp"
#include "utils.hpp"

using namespace std;

class Ball : public MyGameObject
{
public:
    float y = 3.2f;

    float rotation_OX = 0;
    float rotation_OZ = 0;
    float speed = 20.0f;
    float startTime = 0;

    Ball(float x, float z, float rotation_OX, float rotation_OZ, float startTime)
    {
        this->x = x + 9.f * sin(rotation_OZ);
        this->z = z + 9.f * cos(rotation_OZ);
        this->rotation_OX = rotation_OX;
        this->rotation_OZ = rotation_OZ;
        this->startTime = startTime;
        this->radius = 1;
    }

    Ball()
    {
    }

    void Move(float deltaTimeSeconds)
    {

        float x_speed = speed * sin(rotation_OZ);
        float z_speed = speed * cos(rotation_OZ);

        this->x += x_speed * deltaTimeSeconds;
        this->z += z_speed * deltaTimeSeconds;
    }

    bool isValid(float time)
    {   
        if (time - startTime <= 5) {
            return true;
        }
        return false;
    }



    void debug()
    {
        cout << "x: " << this->x << endl;
        cout << "y: " << this->y << endl;
        cout << "z: " << this->z << endl;
    }
};