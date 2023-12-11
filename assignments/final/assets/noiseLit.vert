
#version 450
//https://stackoverflow.com/questions/33493709/opengl-layout-qualifier
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;
layout(location = 3) in float vDist;

out float dist;

out Surface{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
}vs_out;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

void main(){
	dist = vDist;
	vs_out.UV = vUV;
	//calcuate vs_out.WorldPosition
	vs_out.WorldPosition = (_Model * vec4(vPos, 1.0f)).xyz;//.xyz is called swizzling
	//calculate vs_out WorldNormal
	vs_out.WorldNormal = normalize(_Model * vec4(vNormal, 0.0f)).xyz;
	//mat3(transpose(inverse(_Model))) * vNormal;
	gl_Position = _ViewProjection * _Model * vec4(vPos,1.0);
}