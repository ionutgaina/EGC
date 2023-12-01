#include "../utils.hpp"
#include "utils.hpp"

using namespace std;

class House : public MyGameObject
{
public:
    House(vector<MyGameObject> &objects)
    {
        this->scale = rand() % 30 + 10;
        this->y = 0.5f * scale;
        this->radius = 0.5f * scale;

        generateXandZ(objects);
    }

    void generateXandZ(vector<MyGameObject> &objects)
    {
        random_device rd;
        mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(-TERRAIN_SIZE, TERRAIN_SIZE);

        int ok = 1;

        if (objects.size() == 0)
        {
            this->x = distribution(gen);
            this->z = distribution(gen);
        }

        while (ok)
        {
            this->x = distribution(gen);
            this->z = distribution(gen);

            for (auto object : objects)
            {
                if (object.verifyInSquare(this->x, this->z, this->radius))
                {
                    ok = 1;
                    break;
                }
                ok = 0;
            }
        }
    }
};