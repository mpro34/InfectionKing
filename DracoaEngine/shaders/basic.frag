#version 330 core

in vec2 TexCoordinate;
uniform vec4 vertColor;
out vec4 frag_color;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

void main()
{
    frag_color = mix(texture(myTexture1, TexCoordinate), texture(myTexture2, TexCoordinate), 0.8);
};