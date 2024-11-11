#version 450 core

layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec3 aoColor;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 cameraPos;
layout(location = 3) in vec3 vertPosition;
layout(location = 4) in vec3 normal;
layout(location = 5) in float visibility;
layout(location = 6) flat in float textureIndex;

layout(binding = 0) uniform sampler2DArray blockTextures;
layout(binding = 1) uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(vertPosition - cameraPos);

    vec3 R = reflect(I, normalize(normal));
    float diff = max(dot(normalize(normal), I), 0.0);
    vec3 outL = diff * R;

    // vec4 fogColor = vec4(1, 1, 1, 1);
    vec4 fogColor = vec4(texture(skybox, R).rgb, 1.0);
    vec3 textureColor = texture(blockTextures, vec3(texCoord, textureIndex)).xyz;

    vec4 outputColor = vec4(aoColor * textureColor, 1.0);

    FragColor = mix(fogColor, outputColor, visibility);
}
