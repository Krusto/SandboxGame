#version 450 core
layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec3 texCoords;

layout(binding = 0) uniform samplerCube skybox;

const float lowerLimit = -0.5;
const float upperLimit = 1.4;

void main()
{
    vec3 fogColor = vec3(0.7);
    vec3 coord = vec3(texCoords.x, texCoords.y, texCoords.z);
    vec4 finalColor = texture(skybox, coord);

    float factor = (coord.y - lowerLimit) / (upperLimit - lowerLimit);
    factor = clamp(factor, 0.0, 1.0);

    FragColor = mix(vec4(fogColor, 1.0), finalColor, factor);
}
