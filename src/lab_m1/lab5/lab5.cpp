#include "lab_m1/lab5/lab5.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Lab5::Lab5()
{
}

Lab5::~Lab5()
{
}

void Lab5::Init()
{
    renderCameraTarget = false;
    projectionType = true;

    right = 10.f;
    left = .01f;
    bottom = .01f;
    top = 10.f;
    fov = 40.f;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh *mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
}

void Lab5::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab5::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));

        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    }

    // TODO(student): Draw more objects with different model matrices.
    // Attention! The `RenderMesh()` function overrides the usual
    // `RenderMesh()` that we've been using up until now. This new
    // function uses the view matrix from the camera that you just
    // implemented, and the local projection matrix.

    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(3.5f, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(30.f), glm::vec3(1.f, 1.f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, .5f, .5f));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(5.f, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(180.f), glm::vec3(1.f, 0, 0.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.2f, 1.5f, 1.f));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }
}

void Lab5::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Lab5::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Lab5::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    // move the camera only if MOUSE_RIGHT button is pressed
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

        if (window->KeyHold(GLFW_KEY_F))
        {
            fov += deltaTime * cameraSpeed;

            if (projectionType)
            {
                projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
            }
        }
        if (window->KeyHold(GLFW_KEY_G))
        {
            fov -= deltaTime * cameraSpeed;

            if (projectionType)
            {
                projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
            }
        }

        if (window->KeyHold(GLFW_KEY_UP) && !projectionType)
        {
            bottom -= deltaTime;
            top += deltaTime;
            projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        }
        if (window->KeyHold(GLFW_KEY_DOWN) && !projectionType)
        {
            bottom += deltaTime;
            top -= deltaTime;

            projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        }
        if (window->KeyHold(GLFW_KEY_LEFT) && !projectionType)
        {
            left -= deltaTime;
            right += deltaTime;
            projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        }
        if (window->KeyHold(GLFW_KEY_RIGHT) && !projectionType)
        {
            left += deltaTime;
            right -= deltaTime;
            projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        }
    }

    // TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.
}

void Lab5::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections
    if (key == GLFW_KEY_O)
    {
        projectionType = false;
        projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
    }
    if (key == GLFW_KEY_P)
    {
        projectionType = true;
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
    }
}

void Lab5::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Lab5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0)
        {
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
            camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
        }
    }
}

void Lab5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}

void Lab5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Lab5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Lab5::OnWindowResize(int width, int height)
{
}
