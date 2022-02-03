#version 330 core

uniform vec4 vertColor;
out vec4 frag_color;

void main()
{
    frag_color = vertColor;
};