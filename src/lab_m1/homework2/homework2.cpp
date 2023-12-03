#include "lab_m1/homework2/homework2.h"

using namespace std;
using namespace hw2;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Homework2::Homework2()
{
}

Homework2::~Homework2()
{
}

void Homework2::Init()
{
    window->DisablePointer();
    renderCameraTarget = false;

    fov = 50.f;

    camera = new Camera();
    camera->Set(glm::vec3(0, 7.5f, -12.0f), glm::vec3(0, 5, -2), glm::vec3(0, 1, 0));

    friendlyTank = new Tank(0, 0);

    {
        Mesh *mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // load tank obj
    {
        Mesh *mesh = new Mesh("tank_rails");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "hw2tank"), "rails.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("tank_body");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "hw2tank"), "body.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("tank_turret");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "hw2tank"), "turret.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("tank_cannon");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "hw2tank"), "cannon.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("house");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader *shader = new Shader("TankShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "homework2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "homework2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader *shader = new Shader("simpleShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "homework2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "homework2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, .01f, 200.f);

    // Load enemy tanks
    {
        vector<MyGameObject *> collisionObjects;
        collisionObjects.push_back(friendlyTank);

        for (int i = 0; i < ENEMY_TANKS; i++)
        {
            Tank *tank = new Tank(collisionObjects);
            enemyTanks.push_back(tank);
            collisionObjects.push_back(tank);
            cout << "Tank " << i << " at " << tank->x << " " << tank->z << endl;
        }
    }
    // Load houses
    {
        vector<MyGameObject *> collisionObjects;
        collisionObjects.push_back(friendlyTank);
        for (int i = 0; i < enemyTanks.size(); i++)
        {
            collisionObjects.push_back(enemyTanks[i]);
        }

        for (int i = 0; i < houseCount; i++)
        {
            House *house = new House(collisionObjects);
            houses.push_back(house);
            cout << "House " << i << " at " << house->x << " " << house->z << endl;
        }
    }
}

void Homework2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Homework2::Update(float deltaTimeSeconds)
{
    deltaTime = deltaTimeSeconds;
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(100));
        RenderMesh(meshes["plane"], shaders["simpleShader"], modelMatrix, glm::vec3(0.21f, 0.37f, 0.23f));
    }

    { // render tanks
        RenderTank(friendlyTank);

        for (Tank *tank : enemyTanks)
        {
            RenderEnemyTank(tank);
        }
    }

    { // render bullets
        for (int i = 0; i < bullets.size(); i++)
        {
            vector<MyGameObject *> collisionObjects;

            collisionObjects.push_back(friendlyTank);

            for (int j = 0; j < enemyTanks.size(); j++)
            {
                collisionObjects.push_back(enemyTanks[j]);
            }

            for (int j = 0; j < houses.size(); j++)
            {
                collisionObjects.push_back(houses[j]);
            }

            MyGameObject *hitObject = bullets[i]->getCollisionObject(collisionObjects);

            if (hitObject != NULL && hitObject->health > 0)
            {
                hitObject->health -= 1;
                cout << "Hit tank" << hitObject->health << endl;
            }

            if (bullets[i]->isValid(timePassed) && hitObject == NULL)
            {
                bullets[i]->Move(deltaTimeSeconds);
                RenderBall(bullets[i]);
            }
            else
            {
                bullets.erase(bullets.begin() + i);
                i--;
            }
        }
    }

    { // render Houses
        for (House *house : houses)
        {
            glm::vec3 house_position = glm::vec3(house->x, house->y, house->z);

            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, house_position);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(house->scale));
            RenderMesh(meshes["house"], shaders["simpleShader"], modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
        }
    }

    // // DEBUG PURPOSE
    {
        RenderBall(&testBal);
    }

    timePassed += deltaTimeSeconds * 100;
}

void Homework2::FrameEnd()
{
}

void Homework2::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    glUniform3f(glGetUniformLocation(shader->program, "object_color"), color.x, color.y, color.z);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Homework2::OnInputUpdate(float deltaTime, int mods)
{
    vector<MyGameObject *> collisionObjects;

    for (int i = 0; i < ENEMY_TANKS; i++)
    {
        collisionObjects.push_back(enemyTanks[i]);
    }

    for (int i = 0; i < houseCount; i++)
    {
        collisionObjects.push_back(houses[i]);
    }

    if (window->KeyHold(GLFW_KEY_W))
    {
        // Translate the camera forward
        if (friendlyTank->MoveForward(deltaTime * cameraSpeed, collisionObjects))
            camera->MoveForward(deltaTime * cameraSpeed, friendlyTank->rotation_body);
    }
    else if (window->KeyHold(GLFW_KEY_S))
    {
        // Translate the camera backwards
        if (friendlyTank->MoveForward(-deltaTime * cameraSpeed, collisionObjects))
            camera->MoveForward(-deltaTime * cameraSpeed, friendlyTank->rotation_body);
    }

    if (window->KeyHold(GLFW_KEY_A))
    {
        // Translate the camera to the left
        if (friendlyTank->rotateBody(deltaTime, collisionObjects))
            camera->RotateThirdPerson_OY(deltaTime);
    }
    else if (window->KeyHold(GLFW_KEY_D))
    {
        // Translate the camera to the right
        if (friendlyTank->rotateBody(-deltaTime, collisionObjects))
            camera->RotateThirdPerson_OY(-deltaTime);
    }

    // // DEBUG PURPOSE
    if (window->KeyHold(GLFW_KEY_UP))
    {
        testBal.z += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_DOWN))
    {
        testBal.z -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_LEFT))
    {
        testBal.x += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_RIGHT))
    {
        testBal.x -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_KP_ADD))
    {
        testBal.y += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_KP_SUBTRACT))
    {
        testBal.y -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_B))
    {
        testBal.debug();
        cout << glfwGetTime() << endl;
    }
}

void Homework2::OnKeyPress(int key, int mods)
{
    // Add key press event
}

void Homework2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Homework2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOY = 0.001f;

        camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
        // camera->RotateThirdPerson_OX(auxDeltaY);
    }
    else
    {
        float sensitivyOY = 0.001f;
        friendlyTank->rotateTurret(sensitivyOY * -deltaX);
    }
}

void Homework2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
    {
        float x = friendlyTank->x;
        float z = friendlyTank->z;
        float rotation_OX = friendlyTank->rotation_cannon;
        float rotation_OZ = friendlyTank->rotation_turret + friendlyTank->rotation_body;

        if (friendlyTank->canShoot(glfwGetTime()))
        {
            Ball *bullet = new Ball(x, z, rotation_OX, rotation_OZ, timePassed);

            bullets.push_back(bullet);
        }
    }
}

void Homework2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Homework2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Homework2::OnWindowResize(int width, int height)
{
}

glm::mat4 Homework2::SpawnModelMatrix(glm::vec3 position, float rotation)
{
    float x_spawn_rotate = 270.f;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(x_spawn_rotate), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 0, 1));

    return modelMatrix;
}

void Homework2::RenderTank(Tank *tank)
{
    glm::vec3 tank_position = glm::vec3(tank->x, tank->y, tank->z);
    float tank_rotation = tank->rotation_body;
    float tank_turret_rotation = tank->rotation_turret;

    glm::mat4 modelMatrix = SpawnModelMatrix(tank_position, tank_rotation);
    RenderMesh(meshes["tank_body"], shaders["TankShader"], modelMatrix, glm::vec3(0.27f, 0.71f, 0.28f));

    modelMatrix = SpawnModelMatrix(tank_position, tank_rotation);
    modelMatrix = glm::rotate(modelMatrix, tank_turret_rotation, glm::vec3(0, 0, 1));
    RenderMesh(meshes["tank_turret"], shaders["TankShader"], modelMatrix, glm::vec3(0.07f, 0.41f, 0.21f));

    modelMatrix = SpawnModelMatrix(tank_position, tank_rotation);
    modelMatrix = glm::rotate(modelMatrix, tank_turret_rotation, glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, tank->rotation_cannon, glm::vec3(1, 0, 0));
    RenderMesh(meshes["tank_cannon"], shaders["TankShader"], modelMatrix, glm::vec3(0.72f, 0.67f, 0.74f));

    modelMatrix = SpawnModelMatrix(tank_position, tank_rotation);
    RenderMesh(meshes["tank_rails"], shaders["TankShader"], modelMatrix, glm::vec3(0.72f, 0.67f, 0.74f));
}

void Homework2::RenderEnemyTank(Tank *tank)
{
    vector<MyGameObject *> collisionObjects;

    collisionObjects.push_back(friendlyTank);

    for (int i = 0; i < enemyTanks.size(); i++)
    {
        if (enemyTanks[i] != tank)
        {
            collisionObjects.push_back(enemyTanks[i]);
        }
    }

    for (int i = 0; i < houseCount; i++)
    {
        collisionObjects.push_back(houses[i]);
    }

    tank->ai(deltaTime * cameraSpeed, glfwGetTime(), collisionObjects, friendlyTank);

    glm::vec3 tank_position = glm::vec3(tank->x, tank->y, tank->z);
    float tank_rotation = tank->rotation_body;
    float tank_turret_rotation = tank->rotation_turret;

    glm::mat4 modelMatrix = SpawnModelMatrix(tank_position, tank_rotation);
    RenderMesh(meshes["tank_body"], shaders["TankShader"], modelMatrix, glm::vec3(1, 0, 0));

    modelMatrix = SpawnModelMatrix(tank_position, tank_rotation);
    modelMatrix = glm::rotate(modelMatrix, tank_turret_rotation, glm::vec3(0, 0, 1));
    RenderMesh(meshes["tank_turret"], shaders["TankShader"], modelMatrix, glm::vec3(0, 0, 1));

    modelMatrix = SpawnModelMatrix(tank_position, tank_rotation);
    modelMatrix = glm::rotate(modelMatrix, tank_turret_rotation, glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, tank->rotation_cannon, glm::vec3(1, 0, 0));
    RenderMesh(meshes["tank_cannon"], shaders["TankShader"], modelMatrix, glm::vec3(0, 1, 0));

    modelMatrix = SpawnModelMatrix(tank_position, tank_rotation);
    RenderMesh(meshes["tank_rails"], shaders["TankShader"], modelMatrix, glm::vec3(0, 1, 0));
}

void Homework2::RenderBall(Ball *ball)
{
    glm::vec3 ball_position = glm::vec3(ball->x, ball->y, ball->z);
    float ball_rotation = 0;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, ball_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    modelMatrix = glm::rotate(modelMatrix, ball_rotation, glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, ball->rotation_OX, glm::vec3(1, 0, 0));
    RenderMesh(meshes["sphere"], shaders["simpleShader"], modelMatrix, glm::vec3(1, 1, 1));
}
