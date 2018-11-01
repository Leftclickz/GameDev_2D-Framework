#version 120

attribute vec2 a_Position;

void main()
{
    vec2 objectpos = a_Position;

    gl_Position = vec4( objectpos, 0, 1 );
}
