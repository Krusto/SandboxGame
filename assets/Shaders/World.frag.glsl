#version 450 core

layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec3 aoColor;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 cameraPos;
layout(location = 3) in float visibility;
layout(location = 4) flat in float textureIndex;

layout(binding = 0) uniform sampler2DArray blockTextures;

void main()
{
    vec3 textureColor = texture(blockTextures, vec3(texCoord, textureIndex)).xyz;

    vec4 outputColor = vec4(aoColor * textureColor, 1.0);

    FragColor = mix(vec4(1, 1, 1, 1.0), outputColor, visibility);
}