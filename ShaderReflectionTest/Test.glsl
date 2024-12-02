#version 450 core

layout(location = 0) in vec3 aPosition;

layout(packed, binding = 0) uniform Camera
{
    mat4 projection;
    mat4 view;
    vec3 position;
};

layout(std140, binding = 1) uniform Model { mat4 model; };

void main()
{
    vec3 worldPos = vec3(model * vec4(aPosition, 1.0));

    gl_Position = projection * view * vec4(worldPos, 1.0);
}