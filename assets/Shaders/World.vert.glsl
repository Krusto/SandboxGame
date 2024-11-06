#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in int aAxis;
layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec3 outColor;

struct Camera {
    mat4 projection;
    mat4 view;
    vec3 position;
};

uniform Camera camera;
uniform mat4 model;
uniform vec3 offset;

void main()
{
    mat4 MVP = camera.projection * camera.view * model;

    gl_Position = MVP * vec4(aPosition + offset, 1.0);

    vec3 color = vec3(1, 1, 1);

    if (aAxis == 2 || aAxis == 3) { color = vec3(0.7, 0.7, 0.7); }
    else if (aAxis == 4 || aAxis == 5) { color = vec3(0.6, 0.6, 0.6); }

    outColor = vec3(0, 1, 0) * color;
}