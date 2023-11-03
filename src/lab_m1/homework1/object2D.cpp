#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh *object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, color),
            VertexFormat(corner + glm::vec3(length, 0, 0), color),
            VertexFormat(corner + glm::vec3(length, length, 0), color),
            VertexFormat(corner + glm::vec3(0, length, 0), color)};

    Mesh *square = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};

    if (!fill)
    {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else
    {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh *object2D::CreateRhomb(
    const std::string &name,
    glm::vec3 middlePoint,
    float length,
    glm::vec3 color)
{
    glm::vec3 middle = middlePoint;

    std::vector<VertexFormat> vertices =
        {
            VertexFormat(middlePoint, color), // C
            VertexFormat(middlePoint + glm::vec3(-length/2, length, 0), color), // A
            VertexFormat(middlePoint + glm::vec3(-length, 0, 0), color), // B
            VertexFormat(middlePoint + glm::vec3(-length/2, -length, 0), color), // D
            VertexFormat(middlePoint + glm::vec3(-length/2, length/3, 0), color), // I
            VertexFormat(middlePoint + glm::vec3(-length/2, -length/3, 0), color), // H
            VertexFormat(middlePoint + glm::vec3(length, -length/3, 0), color), // G
            VertexFormat(middlePoint + glm::vec3(length, length/3, 0), color), // F
        };

    Mesh *turret = new Mesh(name);
    std::vector<unsigned int> indices = 
    {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
    };

    turret->InitFromData(vertices, indices);
    return turret;
}
