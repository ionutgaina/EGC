#pragma once

#include "lab_m1/homework1/utils.hpp"
#include "lab_m1/homework1/transform2D.h"
#include "lab_m1/homework1/object2D.h"
#include "components/simple_scene.h"
#include "lab_m1/homework1/enemy.cpp"

namespace m1
{
   class Homework1 : public gfxc::SimpleScene
   {
   public:
      Homework1();
      ~Homework1();

      void Init() override;

   private:
      void FrameStart() override;
      void Update(float deltaTimeSeconds) override;
      void FrameEnd() override;

      void OnInputUpdate(float deltaTime, int mods) override;
      void OnKeyPress(int key, int mods) override;
      void OnKeyRelease(int key, int mods) override;
      void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
      void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
      void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
      void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
      void OnWindowResize(int width, int height) override;

   protected:
      float cx, cy;
      glm::mat3 modelMatrix;
      float translateX, translateY;
      float scaleX, scaleY;
      float angularStep;

      bool dummy;
      float rotation = 0;

      int life = 3;
      int currency = 10;
      long timePassed = 0;
      Enemy enemy[ENEMY_SIZE];

      std::string turretNames[4] = {"blue_turret", "orange_turret", "yellow_turret", "purple_turret"};
      int turretPrices[4] = {1, 2, 2, 3};

      bool turretSelected = false;
      int turretSelectedIndex = -1;
      int turretX = -1000;
      int turretY = -1000;

      glm::ivec2 resolution;

      std::string turretPlaced[SQUARE_GRID_SIZE][SQUARE_GRID_SIZE] = {{"", "", ""}, {"", "", ""}, {"", "", ""}};
   };
} // namespace m1