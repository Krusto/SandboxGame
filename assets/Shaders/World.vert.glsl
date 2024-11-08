#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in int aAxis;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in uint aCompressedData;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 texCoord;
layout(location = 2) out vec3 cameraPos;
layout(location = 3) out float visibility;

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
    vec4 worldPosition = model * vec4(aPosition + offset, 1.0);

    vec4 posRelToCamera = camera.view * worldPosition;

    gl_Position = camera.projection * posRelToCamera;

    vec3 color = vec3(1, 1, 1);

    if (aAxis == 2 || aAxis == 3) { color = vec3(0.7, 0.7, 0.7); }
    else if (aAxis == 4 || aAxis == 5) { color = vec3(0.6, 0.6, 0.6); }

    outColor = color;

    texCoord.x = aTexCoord.x * (aCompressedData & 0xFF);
    texCoord.y = aTexCoord.y * ((aCompressedData >> 8) & 0xFF);
    texCoord.z = (aCompressedData >> 16) & 0xFF + 2;

    cameraPos = camera.position;

    float density = 0.015;
    float gradient = 2.0;
    visibility = exp(-pow(density * length(posRelToCamera.xyz), gradient));
}