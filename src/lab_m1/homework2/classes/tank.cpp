#include "../utils.h"

using namespace std;

class Tank
{
public:
    int health = 3;
    float x = 0;
    float y = 0;
    float z = 0;
    float rotation_body = 0;
    float rotation_turret = 0;
    float rotation_cannon = 0;
    

    Tank(int x, int z)
    {
        this->x = x;
        this->z = z;
    }

    void MoveForward(float speed)
    {
        z += speed * cos(rotation_body);
        x += speed * sin(rotation_body);
    }

    void rotateBody(float speed)
    {
        rotation_body += speed;
    }

    void rotateTurret(float speed)
    {
        rotation_turret += speed;
    }

    void rotateCannon(float speed)
    {   
        float max_rotation = 0.101f;
        float min_rotation = -0.06f;

        if (rotation_cannon + speed > max_rotation)
            rotation_cannon = max_rotation;
        else if (rotation_cannon + speed < min_rotation)
            rotation_cannon = min_rotation;
        else
            rotation_cannon += speed;
    }
};