#version 450 core

struct VertexData {
    vec3 vertNormal;
    vec2 texCoord;
    vec3 worldPos;
    vec3 viewPos;
    vec3 aoColor;
};

struct Camera {
    mat4 projection;
    mat4 view;
    vec3 position;
};

struct Light {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    float shininess;
};

layout(location = 0) out VertexData vertDataOut;
layout(location = 5) out Light outLight;
layout(location = 11) out vec3 outRotation;
layout(location = 12) flat out float textureIndex;
layout(location = 13) out float fogVisibility;

layout(location = 0) uniform Camera camera;
layout(location = 3) uniform Light light;
layout(location = 9) uniform vec3 rotation;
layout(location = 10) uniform mat4 model;
layout(location = 11) uniform vec3 offset;

layout(std430, binding = 3) buffer blockBuffer { uint blocks[]; };

layout(std430, binding = 4) buffer quadsBuffer { uint quads[]; };

uint getCurrentCompressedQuad() { return quads[gl_VertexID / 4]; }

uint getVertexID() { return gl_VertexID % 4; }

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
        if (vertexID == 2) { coords = vec2(0, 0); }
        if (vertexID == 3) { coords = vec2(1, 0); }
        if (vertexID == 0) { coords = vec2(1, 1); }
        if (vertexID == 1) { coords = vec2(0, 1); }
        coords.x *= width;
        coords.y *= height;
    }
    return coords;
}

vec3 getPosition()
{
    return vec3(getCurrentCompressedQuad() & 0x1F, getCurrentCompressedQuad() >> 5 & 0x1F,
                getCurrentCompressedQuad() >> 10 & 0x1F);
}

vec3 getBlockPosition(uint axis, uint width, uint height)
{
    vec3 blockPosition = vec3(0, 0, 0);
    uint vertexID = getVertexID();
    vec3 position = getPosition();

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

uint getBlock()
{
    uint quadIndex = gl_VertexID / 4;
    uint row = quadIndex / 4;
    uint byteIndex = quadIndex % 4;

    return (blocks[row] >> (byteIndex * 8)) & 0xF + 4 - 1;
}

vec3 getAO(uint axis)
{

    if (axis == 2 || axis == 3) { return vec3(0.7, 0.7, 0.7); }
    else if (axis == 4 || axis == 5) { return vec3(0.6, 0.6, 0.6); }
    return vec3(1, 1, 1);
}

uint getTilingFactorX() { return (getCurrentCompressedQuad() >> 15 & 31) + 1; }

uint getTilingFactorY() { return (getCurrentCompressedQuad() >> 20 & 31) + 1; }

uint getAxis() { return (getCurrentCompressedQuad() >> 25); }

vec3 getNormal(uint axis)
{
    if (axis == 0) { return vec3(0, -1, 0); }
    else if (axis == 1) { return vec3(0, 1, 0); }
    else if (axis == 2) { return vec3(1, 0, 0); }
    else if (axis == 3) { return vec3(1, 0, 0); }
    else if (axis == 4) { return vec3(0, 0, 1); }
    else if (axis == 5) { return vec3(0, 0, 1); }
    return vec3(0, 0, 0);
}

void main()
{
    outLight = light;
    uint width = getTilingFactorX();
    uint height = getTilingFactorY();

    uint axis = getAxis();

    vec3 blockPosition = getBlockPosition(axis, width, height);
    vec4 worldPosition = model * vec4(blockPosition + offset, 1.0);

    vec4 posRelToCamera = camera.view * worldPosition;

    gl_Position = camera.projection * posRelToCamera;

    vertDataOut.aoColor = getAO(axis);

    vertDataOut.texCoord = getTexCoord(axis, getVertexID(), width, height);

    uint currentBlock = getBlock();

    if (currentBlock == 2 && axis > 1) { textureIndex = 3; }
    else { textureIndex = float(getBlock()); }

    vertDataOut.viewPos = camera.position;

    float density = 0.004;
    float gradient = 5;
    fogVisibility = exp(-pow(density * length(posRelToCamera.xyz), gradient));

    vertDataOut.vertNormal = mat3(transpose(inverse(model))) * getNormal(axis);
    // vertDataOut.vertNormal = getNormal(axis);
    vertDataOut.worldPos = worldPosition.xyz;
    outRotation = rotation;
}