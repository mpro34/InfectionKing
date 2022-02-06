#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoordinate;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Normal = mat3(transpose(inverse(model))) * normal; // Fix normal for scaling
    FragPos = vec3(model * vec4(pos, 1.0f)); // position of incoming vertex in world space, not local space

    TexCoordinate = texCoord;
    gl_Position = projection * view * model * vec4(pos, 1.0f);
};