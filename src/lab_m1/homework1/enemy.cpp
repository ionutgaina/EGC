using namespace std;

#pragma once

#include "lab_m1/homework1/utils.hpp"

class Enemy
{
public:
    int row;
    int life = 3;
    int translateX = 1350;
    int translateY;
    float scale = 1.0f;
    std::string type_mesh;
    int timeAppear;
    bool isDead = false;
    float radius = HEXAGON_SIZE;

    Enemy()
    {
        this->type_mesh = type_mesh;

        this->row = rand() % 3;
        this->translateY = (SQUARE_SIZE + SQUARE_GRID_SPACE) * this->row + SQUARE_GRID_SPACE + (SQUARE_SIZE / 2);

        std::string enemyTypes[] = {"blue_hex", "orange_hex", "yellow_hex", "purple_hex"};

        this->type_mesh = enemyTypes[rand() % 4];
    }

    void generateTimeAppear(int time) {

        this->timeAppear = time + rand() % 500;
    }

    int renderEnemy(float deltaTime)
    {   
        if (this->scale <= 0) {
            return 0;
        }

        if (this->translateX >= 50) {
            this->translateX -= deltaTime * 100;
        } else if (this->scale > 0){
            this->scale -= deltaTime;
        };

        if (this->life <= 0) {
            this->scale -= deltaTime * 10;
            this->isDead = true;
        }

        return 1;
    }
};