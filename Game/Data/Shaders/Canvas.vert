#version 120

attribute vec2 a_Position;
attribute vec4 a_Color;
attribute vec2 a_UVCoord;

uniform vec2 u_ProjectionScale;
uniform vec2 u_CameraTranslation;

varying vec4 v_Color;
varying vec2 v_UVCoord;

void main()
{
    vec2 finalPos = a_Position;
	finalPos += u_CameraTranslation;
    finalPos *= u_ProjectionScale;
	

    gl_Position = vec4( finalPos, 0, 1 );

    v_Color = a_Color;

	v_UVCoord = a_UVCoord;
}
