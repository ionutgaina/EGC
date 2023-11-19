
class Bullet
{
public:
    int x;
    int y;
    std::string type;
    bool isDead = true;
    float radius = STAR_SIZE / 1.47f;

    Bullet() {}

    Bullet(int x, int y, std::string type)
    {
        this->x = x;
        this->y = y;
        this->type = type;
        this->isDead = false;
    }

    bool isBulletDead(int resolution_x, float deltaTimeSeconds)
    {
        if (this->isDead || this->x >= resolution_x )
        {
            this->isDead = true;
            return true;
        }
        this->x += deltaTimeSeconds * 250;
        return false;
    }

    std::string getBulletType()
    {
        return this->type;
    }
};