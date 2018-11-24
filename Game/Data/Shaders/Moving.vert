#version 120

attribute vec2 a_Position;
attribute vec4 a_Color;
attribute vec2 a_UVCoord;

uniform vec2 u_ObjectScale;
uniform float u_ObjectAngleRadians;
uniform vec2 u_ObjectPosition;
uniform vec2 u_UVOffset;
uniform vec2 u_UVScale;
uniform vec2 u_ObjectAnchor;

uniform vec2 u_CameraTranslation;
uniform vec2 u_ProjectionScale;

varying vec4 v_Color;
varying vec2 v_UVCoord;

void main()
{
    vec2 finalPos = a_Position;

    finalPos *= u_ObjectScale;
    finalPos = vec2( finalPos.x*cos(u_ObjectAngleRadians) - finalPos.y*sin(u_ObjectAngleRadians),
                     finalPos.x*sin(u_ObjectAngleRadians) + finalPos.y*cos(u_ObjectAngleRadians) );
    finalPos += u_ObjectPosition + u_ObjectAnchor;

    finalPos += u_CameraTranslation;
    finalPos *= u_ProjectionScale;



    gl_Position = vec4( finalPos, finalPos.y * 0.00, 1 );

    v_Color = a_Color;

	vec2 UV_Final = a_UVCoord * u_UVScale;
	UV_Final += u_UVOffset;

	v_UVCoord = UV_Final;
}
