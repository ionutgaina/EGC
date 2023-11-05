
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
        this->x += deltaTimeSeconds * 200;
        return false;
    }

    std::string getBulletType()
    {

        if (this->type == "blue_turret")
        {
            return "blue_star";
        }
        else if (this->type == "orange_turret")
        {
            return "orange_star";
        }
        else if (this->type == "yellow_turret")
        {
            return "yellow_star";
        }
        else if (this->type == "purple_turret")
        {
            return "purple_star";
        }
        else
        {
            std::cout << "Error: Invalid turret type " << this->type << endl;
            return "";
        }
    }
};