#version 450 core
layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec2 texCoords;

layout(binding = 0) uniform sampler2D plane;

void main()
{
    float depthValue = texture(plane, texCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
    // FragColor = vec4(1.0);
}
