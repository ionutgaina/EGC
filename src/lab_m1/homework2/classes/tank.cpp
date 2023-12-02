#include "../utils.hpp"
#include "utils.hpp"

using namespace std;

class Tank : public MyGameObject
{
public:
    float rotation_body = 0;
    float rotation_turret = 0;
    float rotation_cannon = 0;
    float cooldown = 0;

    Tank(int x, int z)
    {
        this->x = x;
        this->z = z;
        this->health = 3;
        this->radius = TANK_RADIUS;
    }

    Tank(vector<MyGameObject *> objects)
    {
        this->generateXandZ(objects);
        this->radius = TANK_RADIUS;
        this->health = 3;
    }

    bool canShoot(float currentTime)
    {
        if (currentTime - cooldown <= 1)
        {
            return false;
        }

        cooldown = currentTime;
        return true;
    }

    void MoveForward(float speed)
    {
        this->z += speed * cos(rotation_body);
        this->x += speed * sin(rotation_body);
    }

    void rotateBody(float speed)
    {
        this->rotation_body += speed;
    }

    void rotateTurret(float speed)
    {
        this->rotation_turret += speed;
    }

    void rotateCannon(float speed)
    {
        float max_rotation = 0.101f;
        float min_rotation = -0.06f;

        if (this->rotation_cannon + speed > max_rotation)
            this->rotation_cannon = max_rotation;
        else if (this->rotation_cannon + speed < min_rotation)
            this->rotation_cannon = min_rotation;
        else
            this->rotation_cannon += speed;
    }
};