#include "lab_m1/homework1/homework1.h"

#include <vector>
#include <iostream>

#include "lab_m1/homework1/transform2D.h"
#include "lab_m1/homework1/object2D.h"

using namespace std;
using namespace m1;

#define SQUARE_GRID_SIZE 3
#define SQUARE_GRID_SPACE 20
#define SQUARE_SIZE 100

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Homework1::Homework1()
{
}

Homework1::~Homework1()
{
}

void Homework1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = SQUARE_SIZE;

    // Compute coordinates of a square's center
    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

    Mesh *square1 = object2D::CreateSquare("red_square", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);
    square1 = object2D::CreateSquare("green_square", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square1);

    Mesh *turret = object2D::CreateRhomb("green_turret", glm::vec3(0, 0, 0), squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(turret);
}

void Homework1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Homework1::Update(float deltaTimeSeconds)
{
    { // Game scene
        // Kill zone for friendly team
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(SQUARE_GRID_SPACE, SQUARE_GRID_SPACE);
        modelMatrix *= transform2D::Scale(0.5f, 3.4f);
        RenderMesh2D(meshes["red_square"], shaders["VertexColor"], modelMatrix);

        // Squares for friendly team
        for (int i = 0; i < SQUARE_GRID_SIZE; i++)
        {
            for (int j = 1; j <= SQUARE_GRID_SIZE; j++)
            {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate((SQUARE_SIZE + SQUARE_GRID_SPACE) * j - 30, (SQUARE_SIZE + SQUARE_GRID_SPACE) * i + SQUARE_GRID_SPACE);
                RenderMesh2D(meshes["green_square"], shaders["VertexColor"], modelMatrix);
            }
        }
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(600, 600);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);
    RenderMesh2D(meshes["green_turret"], shaders["VertexColor"], modelMatrix);
}

void Homework1::FrameEnd()
{
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Homework1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W))
    {
        scaleX += deltaTime * 10;
        scaleY += deltaTime * 10;
    }
    else if (window->KeyHold(GLFW_KEY_S)){
        scaleX -= deltaTime * 10;
        scaleY -= deltaTime * 10;
    }
}

void Homework1::OnKeyPress(int key, int mods)
{
    // Add key press event
}

void Homework1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Homework1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}

void Homework1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}

void Homework1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Homework1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Homework1::OnWindowResize(int width, int height)
{
}