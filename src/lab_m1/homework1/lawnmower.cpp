
class Lawnmower
{
public:
    int x;
    int y;
    int row;
    bool isActivated = false;
    bool isDead = false;
    Lawnmower(int x, int y, int row){
        this->x = x;
        this->y = y;
        this->row = row;
    }

    void Update(float deltaTimeSeconds){
        if(isActivated){
            x += deltaTimeSeconds * 100;
        }
    }
};