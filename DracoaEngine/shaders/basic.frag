#version 330 core

in vec2 TexCoordinate;
out vec4 frag_color;

uniform sampler2D myTexture1;

void main()
{
    frag_color = texture(myTexture1, TexCoordinate);
};