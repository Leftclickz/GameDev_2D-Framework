#version 120

varying vec4 v_Color;
varying vec2 v_UVCoord;

uniform sampler2D u_Tex;

void main()
{
    gl_FragColor = texture2D( u_Tex, v_UVCoord );
}

