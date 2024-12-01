#version 450 core
layout(location = 0) in vec3 aPos;

layout(location = 0) out vec3 texCoords;

struct Camera {
    mat4 projection;
    mat4 view;
    vec3 position;
};

uniform Camera camera;

void main()
{
    vec4 pos = camera.projection * camera.view * vec4(aPos, 1.0);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    texCoords = aPos;
}
