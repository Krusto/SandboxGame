#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in uint aCompressedData;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec2 texCoord;
layout(location = 2) out vec3 cameraPos;
layout(location = 3) out vec3 vertPosition;
layout(location = 4) out vec3 normal;
layout(location = 5) out float visibility;
layout(location = 6) flat out float textureIndex;

struct Camera {
    mat4 projection;
    mat4 view;
    vec3 position;
};

uniform Camera camera;
uniform mat4 model;
uniform vec3 offset;

layout(std430, binding = 3) buffer blockBuffer { uint blocks[]; };

vec2 getTexCoord(uint axis, uint vertexID, uint width, uint height)
{
    vec2 coords;
    if (axis == 0 || axis == 1)
    {
        if (vertexID == 0) { coords = vec2(0, 1); }
        if (vertexID == 1) { coords = vec2(1, 1); }
        if (vertexID == 2) { coords = vec2(1, 0); }
        if (vertexID == 3) { coords = vec2(0, 0); }
        coords.x *= height;
        coords.y *= width;
    }
    if (axis == 2 || axis == 3)
    {
        if (vertexID == 0) { coords = vec2(0, 0); }
        if (vertexID == 1) { coords = vec2(0, 1); }
        if (vertexID == 2) { coords = vec2(1, 1); }
        if (vertexID == 3) { coords = vec2(1, 0); }
        coords.x *= width;
        coords.y *= height;
    }
    if (axis == 4 || axis == 5)
    {
        if (vertexID == 0) { coords = vec2(0, 0); }
        if (vertexID == 1) { coords = vec2(1, 0); }
        if (vertexID == 2) { coords = vec2(1, 1); }
        if (vertexID == 3) { coords = vec2(0, 1); }
        coords.x *= width;
        coords.y *= height;
    }
    return coords;
}

uint getVertexID() { return gl_VertexID % 4; }

vec3 getBlockPosition(uint axis, uint width, uint height)
{
    vec3 blockPosition = vec3(0, 0, 0);
    uint vert = gl_VertexID;
    uint vertexID = getVertexID();

    if (axis == 0 || axis == 1)
    {
        if (vertexID == 3) { blockPosition = vec3(aPosition.x + width, aPosition.z, aPosition.y + height); }
        else if (vertexID == 2) { blockPosition = vec3(aPosition.x + width, aPosition.z, aPosition.y); }
        else if (vertexID == 1) { blockPosition = vec3(aPosition.x, aPosition.z, aPosition.y); }
        else { blockPosition = vec3(aPosition.x, aPosition.z, aPosition.y + height); }
    }
    else if (axis == 2 || axis == 3)
    {
        if (vertexID == 0) { blockPosition = vec3(aPosition.z, aPosition.y + height, aPosition.x + width); }
        else if (vertexID == 1) { blockPosition = vec3(aPosition.z, aPosition.y, aPosition.x + width); }
        else if (vertexID == 2) { blockPosition = vec3(aPosition.z, aPosition.y, aPosition.x); }
        else { blockPosition = vec3(aPosition.z, aPosition.y + height, aPosition.x); }
    }

    else if (axis == 4 || axis == 5)
    {
        if (vertexID == 3) { blockPosition = vec3(aPosition.x + width, aPosition.y + height, aPosition.z); }
        else if (vertexID == 2) { blockPosition = vec3(aPosition.x, aPosition.y + height, aPosition.z); }
        else if (vertexID == 1) { blockPosition = vec3(aPosition.x, aPosition.y, aPosition.z); }
        else { blockPosition = vec3(aPosition.x + width, aPosition.y, aPosition.z); }
    }
    return blockPosition;
}

uint getBlock()
{
    uint quadIndex = gl_VertexID / 4;
    uint row = quadIndex / 4;
    uint byteIndex = quadIndex % 4;

    return (blocks[row] >> (byteIndex * 8)) & 0xF + 4;
}

vec3 getAO(uint axis)
{

    if (axis == 2 || axis == 3) { return vec3(0.7, 0.7, 0.7); }
    else if (axis == 4 || axis == 5) { return vec3(0.6, 0.6, 0.6); }
    return vec3(1, 1, 1);
}

uint getTilingFactorX() { return (aCompressedData >> 8 & 0xFF); }

uint getTilingFactorY() { return (aCompressedData >> 16 & 0xFF); }

uint getAxis() { return aCompressedData & 0xFF; }

vec3 getNormal(uint axis)
{
    if (axis == 0) { return vec3(0, -1, 0); }
    else if (axis == 1) { return vec3(0, 1, 0); }
    else if (axis == 2) { return vec3(1, 0, 0); }
    else if (axis == 3) { return vec3(-1, 0, 0); }
    else if (axis == 4) { return vec3(0, 0, 1); }
    else if (axis == 5) { return vec3(0, 0, -1); }
    return vec3(0, 0, 0);
}

void main()
{
    uint width = getTilingFactorX();
    uint height = getTilingFactorY();

    uint axis = getAxis();

    vec3 blockPosition = getBlockPosition(axis, width, height);
    vec4 worldPosition = model * vec4(blockPosition + offset, 1.0);

    vec4 posRelToCamera = camera.view * worldPosition;

    gl_Position = camera.projection * posRelToCamera;

    outColor = getAO(axis);

    texCoord = getTexCoord(axis, getVertexID(), width, height);
    // texCoord.z = getBlock();
    textureIndex = float(getBlock());
    cameraPos = camera.position;

    float density = 0.002;
    float gradient = 5;
    visibility = exp(-pow(density * length(posRelToCamera.xyz), gradient));
    // visibility = 1;

    normal = mat3(transpose(inverse(model))) * vec3(0, -1, 0);
    vertPosition = worldPosition.xyz;
}
