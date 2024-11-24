#version 450 core

layout(location = 0) out vec4 FragColor;

struct VertexData {
    vec3 vertNormal;
    vec2 texCoord;
    vec3 worldPos;
    vec3 viewPos;
    vec3 aoColor;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    float shininess;
};

layout(location = 0) in VertexData vertDataIn;
layout(location = 5) in Light light;
layout(location = 12) flat in float textureIndex;
layout(location = 13) in float fogVisibility;

layout(binding = 0) uniform sampler2DArray blockTextures;
layout(binding = 1) uniform samplerCube skybox;

void main()
{
    vec3 ambient = light.ambient;

    vec3 norm = normalize(vertDataIn.vertNormal);
    vec3 lightDir = normalize(light.position - vertDataIn.worldPos);
    vec3 viewDir = normalize(vertDataIn.viewPos - vertDataIn.worldPos);

    vec3 reflectDir = reflect(-lightDir, norm);


    vec3 textureColor = texture(blockTextures, vec3(vertDataIn.texCoord, textureIndex)).xyz;
    vec4 outputColor;
    if (light.intensity != 0)
    {
        //diffuse color exponent
        float diff = max(dot(norm, lightDir), 0.0);
        //specular color exponent
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess);
        // diffuse color
        vec3 diffuse = light.diffuse * (diff * textureColor);

        // specular color
        vec3 specular = light.specular * spec;

        //add ambient occlussion and diffuse
        outputColor = vec4(vertDataIn.aoColor * diffuse, 1.0);
        outputColor = vec4(light.intensity * (specular + outputColor.rgb + ambient), 1.0);
    }
    else { outputColor = vec4(vertDataIn.aoColor * textureColor, 1.0); }
    //add fog
    vec4 fogColor = vec4(texture(skybox, reflectDir).rgb, 1.0);
    outputColor = mix(fogColor, outputColor, fogVisibility);
    FragColor = outputColor;
}
