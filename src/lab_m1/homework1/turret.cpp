

class Turret
{
public:
    std::string type = "";
    float cooldown = 0;
    bool isDead = false;

    Turret() {}

    Turret(std::string type)
    {
        this->type = type;
    }

    bool isShootReady(float deltaTimeSeconds)
    {
        if (cooldown <= 0)
        {
            cooldown = 500.0f;
            return true;
        }
        else
        {
            cooldown -= deltaTimeSeconds * 100;
            return false;
        }
    }
};