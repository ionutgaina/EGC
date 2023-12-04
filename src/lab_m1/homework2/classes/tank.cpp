#include "../utils.hpp"
#include "utils.hpp"

using namespace std;

#define STOP 0
#define MOVE_FORWARD 1
#define MOVE_BACKWARD 2
#define ROTATE_LEFT 3
#define ROTATE_RIGHT 4
#define ROTATE_TURRET_LEFT 5
#define ROTATE_TURRET_RIGHT 6

class Tank : public MyGameObject
{
public:
    float rotation_body = 0;
    float rotation_turret = 0;
    float rotation_cannon = 0;
    float cooldown = 0;
    float ai_cooldown = 0;
    float ai_time = 0;
    int ai_state = 0;

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

    bool MoveForward(float speed, vector<MyGameObject *> objects)
    {
        float aux_x = this->x;
        float aux_z = this->z;

        this->x += speed * sin(rotation_body);
        this->z += speed * cos(rotation_body);

        if (this->getCollisionObject(objects) != NULL || !this->verifyWorldLimit())
        {
            this->x = aux_x;
            this->z = aux_z;
            return false;
        }

        return true;
    }

    bool rotateBody(float speed, vector<MyGameObject *> objects)
    {
        float aux_rotation = this->rotation_body;

        this->rotation_body += speed;

        if (this->getCollisionObject(objects) != NULL || !this->verifyWorldLimit())
        {
            this->rotation_body = aux_rotation;
            return false;
        }

        return true;
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

    void ai(float speed, float currentTime, vector<MyGameObject *> objects, Tank *friendlyTank)
    {

        if (currentTime - this->ai_cooldown > this->ai_time)
        {
            this->ai_cooldown = currentTime;
            // this->ai_state = rand() % 7;
            this->ai_time = rand() % 3 + 1;
        }

        if (this->ai_state == MOVE_FORWARD && !this->MoveForward(speed, objects))
        {
            this->ai_state = rand() % 4 + 1;
            ai(speed, currentTime, objects, friendlyTank);
        }
        else

            if (this->ai_state == MOVE_BACKWARD && !this->MoveForward(-speed, objects))
        {
            this->ai_state = rand() % 4 + 1;
            ai(speed, currentTime, objects, friendlyTank);
        }
        else

            if (this->ai_state == ROTATE_LEFT)
        {
            this->ai_time -= 0.1f;
            this->rotateBody(speed / 5, objects);
        }
        else

            if (this->ai_state == ROTATE_RIGHT)
        {
            this->ai_time -= 0.1f;
            this->rotateBody(-speed / 5, objects);
        }

        if (this->verifyInRadius(friendlyTank->x, friendlyTank->z, AI_RADIUS))
        {
            float angle = this->calculateAngle(friendlyTank->x, friendlyTank->z, this->x, this->z);
            this->rotation_turret = -(angle * 0.0174532925194444f + 3.14159265358979323846f / 2);
            // cout << rotation_turret << endl;
        }
        // else

        // if (this->ai_state == ROTATE_TURRET_LEFT)
        // {
        //     this->ai_time -= 0.1f;
        //     this->rotateTurret(speed / 5);
        // }
        // else

        //     if (this->ai_state == ROTATE_TURRET_RIGHT)
        // {
        //     this->ai_time -= 0.1f;
        //     this->rotateTurret(-speed / 5);
        // }
    }
};