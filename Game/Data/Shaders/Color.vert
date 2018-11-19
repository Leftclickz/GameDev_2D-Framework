#version 120

attribute vec2 a_Position;
attribute vec4 a_Color;

//object uniforms
uniform vec2 u_ObjectScale;
uniform float u_ObjectAngleRadians;
uniform vec2 u_ObjectPosition;
uniform vec4 u_Color;

//camera uniforms
uniform vec2 u_CameraTranslation;
uniform vec2 u_ProjectionScale;

varying vec4 v_Color;

void main()
{
	v_Color = a_Color;
	v_Color = u_Color;

    vec2 finalPos = a_Position;

    finalPos *= u_ObjectScale;
    finalPos = vec2( finalPos.x*cos(u_ObjectAngleRadians) - finalPos.y*sin(u_ObjectAngleRadians),
                     finalPos.x*sin(u_ObjectAngleRadians) + finalPos.y*cos(u_ObjectAngleRadians) );
    finalPos += u_ObjectPosition;

    finalPos += u_CameraTranslation;
    finalPos *= u_ProjectionScale;

    gl_Position = vec4( finalPos, 0, 1 );
}
