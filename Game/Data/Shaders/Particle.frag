#version 140

in vec2 v_UVCoord;
flat in int v_TU;

uniform sampler2D u_Tex[5];

out vec4 COLOR;

void main()
{
    COLOR = texture2D( u_Tex[v_TU], v_UVCoord );
}

