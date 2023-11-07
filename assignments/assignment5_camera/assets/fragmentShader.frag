#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Create a gradient effect using UV coordinates
    vec3 color1 = vec3(1.0, 0.0, 0.0); // Red
    vec3 color2 = vec3(0.0, 0.0, 1.0); // Blue

    // Interpolate between the two colors based on the y-coordinate of the UV
    vec3 gradientColor = mix(color1, color2, TexCoord.y);

    FragColor = vec4(gradientColor, 1.0);
}