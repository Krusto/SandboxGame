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
layout(location = 11) in vec3 rotation;
layout(location = 12) flat in float textureIndex;
layout(location = 13) in float fogVisibility;

layout(binding = 0) uniform sampler2DArray blockTextures;
layout(binding = 1) uniform samplerCube skybox;

void main()
{
    float dist = length(vertDataIn.viewPos - vertDataIn.worldPos);
    //add attenuation
    float constant = 0.3f;
    float linear = 0.1f;
    float quadratic = 0.02f;
    float attenuation = 10.0 / (constant + linear * dist + quadratic * (dist * dist));

    if (attenuation < 0.01) { discard; }

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.ambient;

    vec3 norm = normalize(vertDataIn.vertNormal);
    //POINT LIGHT
    // vec3 lightDir = normalize(light.position - vertDataIn.worldPos);
    //DIRECTIONAL LIGHT
    vec3 lightDir = normalize(radians(rotation));
    lightDir = clamp(lightDir, 0.1f, 1.0);
    // rotation.y = clamp(radians(rotation.y), 0.1, 1.0);
    // rotation.z = clamp(radians(rotation.z), 0.1, 1.0);

    vec3 viewDir = normalize(vertDataIn.viewPos - vertDataIn.worldPos);

    vec3 reflectDir = reflect(-lightDir, norm);

    vec3 textureColor = texture(blockTextures, vec3(vertDataIn.texCoord, textureIndex)).xyz;

    float dotLightNormal = dot(norm, lightDir);
    //diffuse color exponent
    float diff = max(dotLightNormal, 0.0);
    //specular color exponent
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess);

    // diffuse color
    vec3 diffuse = light.diffuse * (diff * textureColor);
    // specular color
    float specStrength = 0.5;
    vec3 specular = specStrength * spec * light.specular;

    // ambient *= attenuation;
    // diffuse *= attenuation;
    // specular *= attenuation;
    vec4 outputColor;

    outputColor = vec4(light.intensity * ((ambient + specular + vertDataIn.aoColor * diffuse)), 1.0);
    // vec4 fogColor = vec4(texture(skybox, reflectDir).rgb, 1.0);
    vec4 fogColor = vec4(vec3(0.3), 1);
    outputColor = mix(fogColor, outputColor, fogVisibility);

    // Apply gamma correction
    float gamma = 1.2;
    outputColor = vec4(pow(outputColor.rgb, vec3(1.0 / gamma)), outputColor.a);

    FragColor = outputColor;
}
