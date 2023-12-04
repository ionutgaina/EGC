#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 object_color;
uniform int health;

// Output value to fragment shader
out vec3 color;


void main()
{   
    int health_rate = 3 -health;
    color = object_color - vec3(0.2, 0.2, 0.2) * health_rate;
    
    vec3 position = vec3(v_position.x - sin(health_rate) * 2, v_position.y + sin(health_rate) * 4, v_position.z);

    gl_Position = Projection * View * Model * vec4(position, 1.0);
}
