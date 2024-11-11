#version 450 core
layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec3 texCoords;

layout(binding = 0) uniform samplerCube skybox;

void main()
{

    vec3 coord = vec3(texCoords.x, texCoords.y, texCoords.z);
    FragColor = texture(skybox, coord);
    // FragColor = vec4(1, 0, 0, 1);
    // FragColor = vec4(coord, 1);
}
 