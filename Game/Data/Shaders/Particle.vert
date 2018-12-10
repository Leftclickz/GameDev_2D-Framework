#version 140

in vec2 a_Position;
in vec2 a_UVCoord;

uniform vec2 u_ProjectionScale;
uniform vec2 u_CameraTranslation;

uniform vec2 u_ObjectScale[100];
uniform float u_ObjectAngleRadians[100];
uniform vec2 u_ObjectPosition[100];
uniform vec2 u_UVOffset[100];
uniform vec2 u_UVScale[100];
uniform vec2 u_ObjectAnchor[100];
uniform int u_TU[100];

out vec2 v_UVCoord;
flat out int v_TU;

void main()
{
    vec2 finalPos = a_Position;

	finalPos *= u_ObjectScale[gl_InstanceID];
    finalPos = vec2( finalPos.x*cos(u_ObjectAngleRadians[gl_InstanceID]) - finalPos.y*sin(u_ObjectAngleRadians[gl_InstanceID]),
                     finalPos.x*sin(u_ObjectAngleRadians[gl_InstanceID]) + finalPos.y*cos(u_ObjectAngleRadians[gl_InstanceID]) );
    finalPos += u_ObjectPosition[gl_InstanceID] + u_ObjectAnchor[gl_InstanceID];


	finalPos += u_CameraTranslation;
    finalPos *= u_ProjectionScale;
	

    gl_Position = vec4( finalPos, 0, 1 );

	vec2 UV_Final = a_UVCoord * u_UVScale[gl_InstanceID];
	UV_Final += u_UVOffset[gl_InstanceID];

	v_UVCoord = UV_Final;
	v_TU = u_TU[gl_InstanceID];
}
