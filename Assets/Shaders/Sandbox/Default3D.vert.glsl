#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

struct Material {
    float shininess;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
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

struct Camera{
    mat4 projection;
    mat4 view;
    mat4 model;
    vec3 position;
};

uniform Material material;
uniform Light light;
uniform Camera camera;

layout(location = 0) out Material outMaterial;
layout(location = 4) out Light outLight;
layout(location = 9) out VertexData outVertexData;

void main()
{
    
    outVertexData.worldPos = vec3(camera.model *vec4(aPosition,1.0));
    outVertexData.vertNormal = mat3(transpose(inverse(camera.model))) * aNormal;
    outVertexData.texCoord = aTexCoord;
    outVertexData.viewPos = camera.position;
    outLight = light;
    outMaterial = material;

    gl_Position = camera.projection * camera.view * vec4(outVertexData.worldPos,1.0);
}