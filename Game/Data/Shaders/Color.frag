#version 140

out vec4 glFragColor;

in vec4 v_Color;


void main()
{
    glFragColor = v_Color;
}
