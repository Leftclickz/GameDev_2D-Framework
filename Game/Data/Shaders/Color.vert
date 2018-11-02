#version 120

attribute vec2 a_Position;
attribute vec2 a_UVCoord;

varying vec2 v_UVCoord;

void main()
{
	v_UVCoord = a_UVCoord;

    gl_Position = vec4( a_Position, 0, 1 );
}
