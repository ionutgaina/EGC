#include "lab_m1/homework1/homework1.h"

using namespace std;
using namespace m1;

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
    square1 = object2D::CreateSquare("border_square", corner, squareSide, glm::vec3(1, 1, 1));
    AddMeshToList(square1);
    square1 = object2D::CreateSquare("life_square", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    // start Init turrets
    int turretSize = 50;

    Mesh *turret = object2D::CreateTurret("blue_turret", glm::vec3(0, 0, 0), TURRET_SIZE, glm::vec3(0, 0, 1));
    AddMeshToList(turret);

    turret = object2D::CreateTurret("orange_turret", glm::vec3(0, 0, 0), TURRET_SIZE, glm::vec3(1.0f, 0.5f, 0));
    AddMeshToList(turret);

    turret = object2D::CreateTurret("yellow_turret", glm::vec3(0, 0, 0), TURRET_SIZE, glm::vec3(1, 1, 0));
    AddMeshToList(turret);

    turret = object2D::CreateTurret("purple_turret", glm::vec3(0, 0, 0), TURRET_SIZE, glm::vec3(0.5f, 0, 1));
    AddMeshToList(turret);

    // end Init turrets

    // start Init stars
    // grey star
    Mesh *star = object2D::CreateStar("star", glm::vec3(0, 0, 0), STAR_SIZE, glm::vec3(0.5f, 0.5f, 0.5f));
    AddMeshToList(star);

    // start hexagon
    Mesh *hex;

    hex = object2D::CreateHexagon("inner_hex", glm::vec3(0, 0, 11), HEXAGON_SIZE, glm::vec3(0.5f, 1.0f, 0));
    AddMeshToList(hex);
    hex = object2D::CreateHexagon("blue_hex", glm::vec3(0, 0, 10), HEXAGON_SIZE, glm::vec3(0, 0, 1));
    AddMeshToList(hex);
    hex = object2D::CreateHexagon("orange_hex", glm::vec3(0, 0, 10), HEXAGON_SIZE, glm::vec3(1.0f, 0.5f, 0));
    AddMeshToList(hex);
    hex = object2D::CreateHexagon("yellow_hex", glm::vec3(0, 0, 10), HEXAGON_SIZE, glm::vec3(1, 1, 0));
    AddMeshToList(hex);
    hex = object2D::CreateHexagon("purple_hex", glm::vec3(0, 0, 10), HEXAGON_SIZE, glm::vec3(0.5f, 0, 1));
    AddMeshToList(hex);
    // end hexagon

    // start Init enemies
    for (int i = 0; i < ENEMY_SIZE; i++)
    {
        enemy[i] = Enemy();
    }
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

        { // Turrets for buy menu
            std::string turretNames[] = {"blue_turret", "orange_turret", "yellow_turret", "purple_turret"};
            int turretPrices[] = {1, 2, 2, 3};

            for (int i = 1; i <= 4; i++)
            {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate((SQUARE_SIZE + SQUARE_GRID_SPACE) * i + SQUARE_GRID_SPACE, 650);
                RenderMesh2D(meshes[turretNames[i - 1]], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(((SQUARE_SIZE + SQUARE_GRID_SPACE)) * i - cx + SQUARE_GRID_SPACE, 650 - cy);
                RenderMesh2D(meshes["border_square"], shaders["VertexColor"], modelMatrix);

                for (int j = 0; j < turretPrices[i - 1]; j++)
                {
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate((SQUARE_SIZE + SQUARE_GRID_SPACE) * i - STAR_SIZE / 2 + (j * (STAR_SIZE + 10)), 650 - SQUARE_SIZE / 2 - STAR_SIZE);
                    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
                }
            }
        }

        { // Life bar and currency
            for (int i = 6; i < 6 + life; i++)
            {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(((SQUARE_SIZE + SQUARE_GRID_SPACE)) * i - (cx * 0.75f) + SQUARE_GRID_SPACE, 650 - (cy * 0.75f));
                modelMatrix *= transform2D::Scale(0.75f, 0.75f);
                RenderMesh2D(meshes["life_square"], shaders["VertexColor"], modelMatrix);
            }

            for (int i = 0; i < min(currency, 15); i++)
            {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate((SQUARE_SIZE + SQUARE_GRID_SPACE) * 6 - STAR_SIZE / 2 + (i * (STAR_SIZE + 10)), 650 - SQUARE_SIZE / 2 - STAR_SIZE);
                RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
            }
        }

        {
            for (int i = 0; i < ENEMY_SIZE && life > 0; i++)
            {
                if (timePassed >= enemy[i].timeAppear && enemy[i].renderEnemy(deltaTimeSeconds))
                {
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(enemy[i].translateX, enemy[i].translateY);
                    modelMatrix *= transform2D::Scale(enemy[i].scale, enemy[i].scale);
                    RenderMesh2D(meshes[enemy[i].type_mesh], shaders["VertexColor"], modelMatrix);
                    modelMatrix *= transform2D::Scale(enemy[i].scale * .75f, enemy[i].scale * .75f);
                    RenderMesh2D(meshes["inner_hex"], shaders["VertexColor"], modelMatrix);
                }
                
                if (enemy[i].translateX <= 50 && !enemy[i].isDead) {
                    life--;
                    enemy[i].isDead = true;
                }
            }
        }
    }

    // TODO win if timePassed > 1000
    // TODO lose if life <= 0

    timePassed += std::floor(deltaTimeSeconds * 100);
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