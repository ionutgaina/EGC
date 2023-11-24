using namespace std;

#pragma once

#include "lab_m1/homework1/utils.hpp"

class Turret
{
public:
    std::string type = "";
    float cooldown = 0;
    bool isDead = false;
    float scale = 1;
    float radius = TURRET_SIZE / 0.95f;
    bool readyForShot = true;
    int x;
    int y;

    Turret() {}

    Turret(std::string type)
    {
        this->type = type;
    }

    void isShootReady(float deltaTimeSeconds)
    {
        if (cooldown <= 0)
        {
            if (this->isDead)
            {
                readyForShot = false;
            }

            cooldown = TURRET_COOLDOWN;
            readyForShot = true;
        }
        else
        {
            cooldown -= deltaTimeSeconds * 10;
            readyForShot = false;
        }
    }

    bool isDeadTurret(float deltaTimeSeconds)
    {
        if (this->scale <= 0)
        {
            this->resetTurret();
            return true;
        }

        if (this->isDead)
        {
            this->scale -= deltaTimeSeconds * 5;
        }
        return false;
    }

    void resetTurret()
    {
        this->cooldown = 0;
        this->isDead = false;
        this->scale = 1;
        this->radius = TURRET_SIZE / 0.95f;
        this->x = -1000;
        this->y = -1000;
        this->type = "";
    }
};