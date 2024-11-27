#version 450 core

layout(location = 0) out vec4 FragColor;

struct VertexData {
    vec3 vertNormal;
    vec2 texCoord;
    vec3 worldPos;
    vec3 viewPos;
    vec3 aoColor;
    vec4 lightPosWorldPos;
};

struct Light {
    vec3 position;
    // vec3 rotation;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    float shininess;
};

layout(location = 0) in VertexData vertDataIn;
layout(location = 6) in Light light;
layout(location = 13) flat in float textureIndex;
layout(location = 14) in float fogVisibility;

layout(binding = 0) uniform sampler2DArray blockTextures;
layout(binding = 1) uniform samplerCube skybox;
layout(binding = 2) uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace, float dotLightNormal)
{
    return 0.0;
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    if (projCoords.z > 1.0) { projCoords.z = 1.0; }
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float bias = max(0.05 * (1.0 - dotLightNormal), 0.005);
    // return (closestDepth + bias) < projCoords.z ? 0.0 : 1.0;
}

void main()
{

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.ambient;

    vec3 norm = normalize(vertDataIn.vertNormal);
    vec3 lightDir = normalize(light.position - vertDataIn.worldPos);
    // vec3 lightDir = normalize(-light.rotation);
    vec3 viewDir = normalize(vertDataIn.viewPos - vertDataIn.worldPos);

    vec3 reflectDir = reflect(-lightDir, norm);

    vec3 textureColor = texture(blockTextures, vec3(vertDataIn.texCoord, textureIndex)).xyz;
    vec4 outputColor;
    if (light.intensity != 0)
    {
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

        //add ambient occlussion and diffuse
        outputColor = vec4(vertDataIn.aoColor * diffuse, 1.0);

        float shadow = ShadowCalculation(vertDataIn.lightPosWorldPos, dotLightNormal);

        //add attenuation
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        float dist = length(light.position - vertDataIn.worldPos);
        float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        outputColor = vec4(light.intensity * (ambient + specular + diffuse), 1.0);
    }
    else
    {
        float shadow = ShadowCalculation(vertDataIn.lightPosWorldPos, 1.0);

        outputColor = vec4((1.0 - shadow) * (textureColor), 1.0);
    }

    // FragColor = vec4(outputColor);
    vec4 fogColor = vec4(texture(skybox, reflectDir).rgb, 1.0);
    outputColor = mix(fogColor, outputColor, fogVisibility);

    // Apply gamma correction
    float gamma = 1.4;
    outputColor = vec4(pow(outputColor.rgb, vec3(1.0 / gamma)), outputColor.a);

    FragColor = outputColor;
}
