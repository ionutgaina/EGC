#include "lab_m1/homework2/homework2.h"

#include <vector>
#include <string>
#include <iostream>

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
    renderCameraTarget = false;

    fov = 50.f;

    camera = new Camera();
    camera->Set(glm::vec3(0, 7.5f, 10.0f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

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
        Shader *shader = new Shader("myShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "homework2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "homework2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, .01f, 200.f);
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
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(3));
        RenderMesh(meshes["plane"], shaders["myShader"], modelMatrix, glm::vec3(0.21f, 0.37f, 0.23f));
    }

    {
        RenderTank();
    }
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix, glm::vec3(0, 0, 0));
    }
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
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {

        if (window->KeyHold(GLFW_KEY_W))
        {
            // Translate the camera forward
            camera->TranslateForward(deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_A))
        {
            // Translate the camera to the left
            camera->TranslateRight(-deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_S))
        {
            // Translate the camera backwards
            camera->TranslateForward(-deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_D))
        {
            // Translate the camera to the right
            camera->TranslateRight(deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_Q))
        {
            // Translate the camera down
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_E))
        {
            // Translate the camera up
            camera->TranslateUpward(deltaTime * cameraSpeed);
        }
    }
}

void Homework2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
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
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0)
        {
            camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
            camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
        }
    }
}

void Homework2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
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

void Homework2::RenderTank()
{
    float x_spawn_rotate = 270.f;
    float z_spawn_rotate = 180.f;

    glm::vec3 tank_position = glm::vec3(0, 0, 0);

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, tank_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(x_spawn_rotate), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(z_spawn_rotate), glm::vec3(0, 0, 1));
    RenderMesh(meshes["tank_body"], shaders["myShader"], modelMatrix, glm::vec3(0.27f, 0.71f, 0.28f));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, tank_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(x_spawn_rotate), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(z_spawn_rotate), glm::vec3(0, 0, 1));
    RenderMesh(meshes["tank_turret"], shaders["myShader"], modelMatrix, glm::vec3(0.72f, 0.67f, 0.74f));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, tank_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(x_spawn_rotate), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(z_spawn_rotate), glm::vec3(0, 0, 1));
    RenderMesh(meshes["tank_cannon"], shaders["myShader"], modelMatrix, glm::vec3(0.07f, 0.41f, 0.21f));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, tank_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(x_spawn_rotate), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(z_spawn_rotate), glm::vec3(0, 0, 1));
    RenderMesh(meshes["tank_rails"], shaders["myShader"], modelMatrix, glm::vec3(0.17f, 0.18f, 0.19f));
}