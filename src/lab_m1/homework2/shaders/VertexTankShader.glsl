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
uniform int HEALTH_CONSTANT;

// Output value to fragment shader
out vec3 color;

// Uniforms for light properties
uniform vec3 eye_position;

void main() {

    int material_shininess = 50;
    float material_kd = 0.5;
    float material_ks = 0.5;

    vec3 light_position = vec3(0, 5, 0);

	// Compute world space vectors
    vec3 world_position = (Model * vec4(v_position, 1)).xyz;
    vec3 world_normal = normalize(mat3(Model) * normalize(v_normal));

    vec3 V = normalize(eye_position - world_position);
    vec3 L = normalize(light_position - world_position);
    vec3 H = normalize(L + V);

	// Define ambient light component
    float ambient_light = 1;

	// Compute diffuse light component
    float diffuse_light = material_kd * max(dot(world_normal, L), 0);

	// Compute specular light component
    float specular_light = 0;

    if(diffuse_light > 0) {
        specular_light = material_ks * pow(max(dot(world_normal, L), 0), material_shininess);
    }

	// Compute light
    float d = distance(light_position, v_position);
    float attenuation_factor = 1 / (0.5 + 0.2 * d + 0.1 * d * d);
    float light = ambient_light + attenuation_factor * (diffuse_light + specular_light);

    float health_rate = 0.5/HEALTH_CONSTANT * (HEALTH_CONSTANT - health);

    color = object_color * light - health_rate;

    vec3 position = vec3(v_position.x - sin(health_rate) * 5, v_position.y + sin(health_rate) * 3, v_position.z - sin(health_rate));

    gl_Position = Projection * View * (Model - sin(health_rate) *0.00001f) * vec4(position, 1.0);
}
