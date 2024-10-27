#version 450 core

layout(location = 0) out vec4 FragColor;

struct Material {
    float shininess;
    vec3 ambient;
    vec3 specular;
}; 
  
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};

struct VertexData{
    vec3 vertNormal;
    vec2 texCoord;
    vec3 worldPos;
    vec3 viewPos;
};

layout(location = 0) in Material material;
layout(location = 4) in Light light;
layout(location = 9) in VertexData vertexData;

layout(binding = 0) uniform sampler2D diffuseTexture;
void main()
{

    vec3 ambient  =  light.ambient * material.ambient;

  	vec3 norm = normalize(vertexData.vertNormal);
    vec3 lightDir = normalize(light.position - vertexData.worldPos);
    vec3 viewDir = normalize(vertexData.viewPos - vertexData.worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
   
    //diffuse color exponent
    float diff = max(dot(norm, lightDir), 0.0);
    //specular color exponent
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // diffuse color
    vec3 diffuse  = light.diffuse * (diff * texture(diffuseTexture,vertexData.texCoord).rgb);  
    
    // specular color
    vec3 specular =  light.specular * (spec * material.specular);  
        
    vec3 outColor = light.intensity * (specular + diffuse + ambient);

    FragColor = vec4(outColor,1.0);
}