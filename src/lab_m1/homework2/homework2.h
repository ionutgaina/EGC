#pragma once

#include "components/simple_scene.h"
#include "lab_m1/homework2/camera.h"
#include "lab_m1/homework2/classes/house.cpp"
#include "lab_m1/homework2/classes/tank.cpp"
#include "lab_m1/homework2/classes/ball.cpp"
#include "utils.hpp"
#include "classes/utils.hpp"

namespace hw2
{
   class Homework2 : public gfxc::SimpleScene
   {
   public:
      Homework2();
      ~Homework2();

      void Init() override;

   private:
      void FrameStart() override;
      void Update(float deltaTimeSeconds) override;
      void FrameEnd() override;

      void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));

      void OnInputUpdate(float deltaTime, int mods) override;
      void OnKeyPress(int key, int mods) override;
      void OnKeyRelease(int key, int mods) override;
      void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
      void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
      void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
      void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
      void OnWindowResize(int width, int height) override;
      void RenderTank(Tank *tank);
      void RenderBall(Ball *ball);
      glm::mat4 SpawnModelMatrix(glm::vec3 position, float rotation = 0);

   protected:
      Camera *camera;
      glm::mat4 projectionMatrix;
      bool renderCameraTarget;

      // TODO(student): If you need any other class variables, define them here.
      bool projectionType;

      GLfloat fov;

      float cameraSpeed = 10.0f;

      glm::vec3 lightPosition;
      unsigned int materialShininess;
      float materialKd;
      float materialKs;

      vector<Ball> bullets;
      Tank* friendlyTank;

      vector<Tank*> enemyTanks;

      Ball testBal;

      float timePassed = 0;

      int houseCount = rand() % 5 + 10;
      vector<House*> houses;
      float auxDeltaX = 0;
   };
} // namespace m1
