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
    vec3 rotation;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    float shininess;
};

layout(location = 3) uniform Light light;

layout(location = 0) in VertexData vertDataIn;
layout(location = 12) flat in float textureIndex;
layout(location = 13) in float fogVisibility;

layout(binding = 0) uniform sampler2DArray blockTextures;
layout(binding = 1) uniform samplerCube skybox;


mat3x3 ACESInputMat = mat3x3(0.59719, 0.35458, 0.04823, 0.07600, 0.90834, 0.01566, 0.02840, 0.13383, 0.83777);

// ODT_SAT => XYZ => D60_2_D65 => sRGB
mat3x3 ACESOutputMat = mat3x3(1.60475, -0.53108, -0.07367, -0.10208, 1.10813, -0.00605, -0.00327, -0.07276, 1.07602);

vec3 RRTAndODTFit(vec3 v)
{
    vec3 a = v * (v + 0.0245786f) - 0.000090537f;
    vec3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

vec3 ACESFitted(vec3 color)
{
    color = transpose(ACESInputMat) * color;
    // Apply RRT and ODT
    color = RRTAndODTFit(color);
    color = transpose(ACESOutputMat) * color;
    color = clamp(color, 0, 1);
    return color;
}

vec3 ACESFilmSimple(vec3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0, 1);
}

//


// https://advances.realtimerendering.com/s2021/jpatry_advances2021/index.html
// https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.40.9608&rep=rep1&type=pdf
vec4 rgb_to_lmsr(vec3 c)
{
    const mat4x3 m = mat4x3(0.31670331, 0.70299344, 0.08120592, 0.10129085, 0.72118661, 0.12041039, 0.01451538,
                            0.05643031, 0.53416779, 0.01724063, 0.60147464, 0.40056206);
    return c * m;
}

vec3 lms_to_rgb(vec3 c)
{
    const mat3 m = mat3(4.57829597, -4.48749114, 0.31554848, -0.63342362, 2.03236026, -0.36183302, -0.05749394,
                        -0.09275939, 1.90172089);
    return c * m;
}

//you can change the look! 0 is default, 1 is golden, 2 is punchy (that looks the best)
#define AGX_LOOK 2

vec3 AGX(vec3 col)
{
    col = mat3(.842, .0423, .0424, .0784, .878, .0784, .0792, .0792, .879) * col;
    // Log2 space encoding
    col = clamp((log2(col) + 12.47393) / 16.5, vec3(0), vec3(1));
    // Apply sigmoid function approximation
    col = .5 + .5 * sin(((-3.11 * col + 6.42) * col - .378) * col - 1.44);
// AgX look (optional)
#if AGX_LOOK == 1
    // Golden
    col = mix(vec3(dot(col, vec3(.216, .7152, .0722))), col * vec3(1., .9, .5), .8);
#elif AGX_LOOK == 2
    // Punchy
    col = mix(vec3(dot(col, vec3(.216, .7152, .0722))), pow(col, vec3(1.35)), 1.4);
#endif

    return col;
}

void main()
{
    float dist = length(vertDataIn.viewPos - vertDataIn.worldPos);
    //add attenuation
    float constant = 0.3f;
    float linear = 0.1f;
    float quadratic = 0.02f;
    float attenuation = 10.0 / (constant + linear * dist + quadratic * (dist * dist));

    // if (attenuation < 0.01) { discard; }

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.ambient;

    vec3 norm = normalize(vertDataIn.vertNormal);
    //POINT LIGHT
    // vec3 lightDir = normalize(light.pos - vertDataIn.worldPos);
    //DIRECTIONAL LIGHT
    vec3 lightDir = normalize(radians(light.rotation));
    // vec3 lightDir = normalize(1.0);
    lightDir = clamp(lightDir, 0.1f, 1.0);
    // lightDir.y = clamp(radians(light.rotation.y), 0.3, 1.0);
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
    // vec4 fogColor = vec4(vec3(0.7), 1);
    // outputColor = mix(fogColor, outputColor, fogVisibility);

    // Apply gamma correction
    float gamma = 2.0;
    float exposure = 1;

    outputColor.rgb = pow(outputColor.rgb, vec3(gamma));
    outputColor.rgb = AGX(outputColor.rgb * exposure);//use the tonemaper last step and than,

    outputColor = vec4(pow(outputColor.rgb, vec3(1.0 / gamma)), outputColor.a);

    FragColor = outputColor;
}
