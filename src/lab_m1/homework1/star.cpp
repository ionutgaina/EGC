
class Star
{
public:
    int x;
    int y;
    bool isCollected = false;
    float radius = STAR_SIZE / 1.47f;
    int timeAppear;
    float scale = 1;

    Star(int timeAppear)
    {
        this->timeAppear = timeAppear;
        this->x = rand() % 1000 + 100;
        this->y = rand() % 500 + 100;
    }

    bool isStarCollected(int mouseX, int mouseY, int resolution_y)
    {
        if (this->isCollected)
        {
            return false;
        }

        if (mouseX >= this->x - this->radius && mouseX <= this->x + this->radius &&
            mouseY >= resolution_y - this->y - this->radius && mouseY <= resolution_y - this->y + this->radius)
        {
            this->isCollected = true;
            return true;
        }
        return false;
    }

    bool renderStar(int timePassed, float deltaTime)
    {
        if (this->scale <= 0)
        {
            return false;
        }

        if (this->isCollected || timeAppear + 400 < timePassed)
        {
            this->scale -= deltaTime * 5;
        }

        return true;
    }
};