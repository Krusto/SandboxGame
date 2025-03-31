#version 450 core

layout(location = 0) in vec3 aPosition;

struct Camera {
    mat4 projection;
    mat4 view;
    vec3 position;
};

layout(location = 0) uniform Camera camera;
layout(location = 3) uniform mat4 model;

void main()
{
    vec3 worldPos = vec3(model * vec4(aPosition, 1.0));

    gl_Position = camera.projection * camera.view * vec4(worldPos, 1.0);
}