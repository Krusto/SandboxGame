#version 450 core
layout(location = 0) in vec3 aPos;

struct Camera {
    mat4 projection;
    mat4 view;
    vec3 position;
};

uniform Camera camera;
uniform mat4 model;
uniform uint axis;
uniform vec3 pos;

uint getVertexID() { return gl_VertexID % 4; }

vec3 getBlockPosition(uint axiss, uint width, uint height)
{
    vec3 blockPosition = vec3(0, 0, 0);
    uint vert = gl_VertexID;
    uint vertexID = getVertexID();


    if (axis == 0 || axis == 1)
    {
        if (vertexID == 3) { blockPosition = vec3(aPos.x + width, aPos.z, aPos.y + height); }
        else if (vertexID == 2) { blockPosition = vec3(aPos.x + width, aPos.z, aPos.y); }
        else if (vertexID == 1) { blockPosition = vec3(aPos.x, aPos.z, aPos.y); }
        else { blockPosition = vec3(aPos.x, aPos.z, aPos.y + height); }
    }
    else if (axis == 2 || axis == 3)
    {
        if (vertexID == 0) { blockPosition = vec3(aPos.z, aPos.y + height, aPos.x + width); }
        else if (vertexID == 1) { blockPosition = vec3(aPos.z, aPos.y, aPos.x + width); }
        else if (vertexID == 2) { blockPosition = vec3(aPos.z, aPos.y, aPos.x); }
        else { blockPosition = vec3(aPos.z, aPos.y + height, aPos.x); }
    }

    else if (axis == 4 || axis == 5)
    {
        if (vertexID == 3) { blockPosition = vec3(aPos.x + width, aPos.y + height, aPos.z); }
        else if (vertexID == 2) { blockPosition = vec3(aPos.x, aPos.y + height, aPos.z); }
        else if (vertexID == 1) { blockPosition = vec3(aPos.x, aPos.y, aPos.z); }
        else { blockPosition = vec3(aPos.x + width, aPos.y, aPos.z); }
    }
    return blockPosition;
}

void main()
{
    vec4 sos = camera.projection * camera.view * model * vec4(getBlockPosition(axis, 1, 1), 1.0);
    gl_Position = sos;
}
