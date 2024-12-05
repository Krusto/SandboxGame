#version 450 core

struct Camera {
    mat4 projection;
    mat4 view;
    vec3 position;
};

layout(location = 0) uniform Camera camera;
layout(location = 3) uniform mat4 model;
layout(location = 4) uniform uint axis;

vec3 getBlockPosition(uint axis, uint width, uint height)
{
    vec3 blockPosition = vec3(0, 0, 0);
    uint vertexID = gl_VertexID % 4;
    vec3 position = vec3(0, 0, 0);

    if (axis == 1 || axis == 3 || axis == 5) { position.z += 1; }
    if (axis == 0 || axis == 1)
    {
        if (vertexID == 3) { blockPosition = vec3(position.x + width, position.z, position.y + height); }
        else if (vertexID == 2) { blockPosition = vec3(position.x + width, position.z, position.y); }
        else if (vertexID == 1) { blockPosition = vec3(position.x, position.z, position.y); }
        else { blockPosition = vec3(position.x, position.z, position.y + height); }
    }
    else if (axis == 2 || axis == 3)
    {
        if (vertexID == 0) { blockPosition = vec3(position.z, position.y + height, position.x + width); }
        else if (vertexID == 1) { blockPosition = vec3(position.z, position.y, position.x + width); }
        else if (vertexID == 2) { blockPosition = vec3(position.z, position.y, position.x); }
        else { blockPosition = vec3(position.z, position.y + height, position.x); }
    }

    else if (axis == 4 || axis == 5)
    {
        if (vertexID == 3) { blockPosition = vec3(position.x + width, position.y + height, position.z); }
        else if (vertexID == 2) { blockPosition = vec3(position.x, position.y + height, position.z); }
        else if (vertexID == 1) { blockPosition = vec3(position.x, position.y, position.z); }
        else { blockPosition = vec3(position.x + width, position.y, position.z); }
    }
    return blockPosition;
}

void main()
{
    vec3 worldPos = vec3(model * vec4(getBlockPosition(axis, 1, 1), 1.0));

    gl_Position = camera.projection * camera.view * vec4(worldPos, 1.0);
}