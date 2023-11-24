#version 330

// Input
// TODO(student): Get values from vertex shader
in vec4 out_color;
in vec2 out_texture;
in vec3 out_normal;


// Output
out vec4 color;
// out vec2 texture;
// out vec3 normal;

uniform float Time;


void main()
{
    // TODO(student): Write pixel out color
    color = normalize(out_color + Time * 10);
    // normal = out_normal;
}
