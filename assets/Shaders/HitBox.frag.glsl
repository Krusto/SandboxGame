#version 450 core

layout(location = 0) out vec4 FragColor;

layout(location = 5) uniform float delta;

void main()
{
    const float speed = 20;
    // float factor = sin(delta * speed);
    // if (factor > 0) { FragColor = vec4(0.2, 0.2, 0.2, 0.5); }
    // else { FragColor = vec4(0.1, 0.1, 0.1, 0.01); }
    float factor = (sin(delta * speed) + 1) / 2;
    vec4 outColor = factor * vec4(1.0);
    outColor.rgb = clamp(outColor.rgb, vec3(0.2), vec3(1));
    outColor.a = clamp(outColor.a, 0.01, 0.2);
    FragColor = vec4(outColor);
}