

class Turret
{
public:
    std::string type = "";
    float cooldown = 0;
    bool isDead = false;
    float radius = TURRET_SIZE / 0.95f;
    int x;
    int y;

    Turret() {}

    Turret(std::string type)
    {
        this->type = type;
    }

    bool isShootReady(float deltaTimeSeconds)
    {
        if (cooldown <= 0)
        {
            cooldown = TURRET_COOLDOWN;
            return true;
        }
        else
        {
            cooldown -= deltaTimeSeconds * 10;
            return false;
        }
    }

    void resetTurret() {
        this->type = "";
        this->cooldown = 0;
        this->isDead = false;
        this->radius = TURRET_SIZE / 0.95f;
        this->x = -1000;
        this->y = -1000;
    }
};