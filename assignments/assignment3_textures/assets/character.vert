#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;

uniform float _Time;

void main() {
	float displacement = 0.5*cos(_Time);
	gl_Position = vec4((vPos.x/2.5), (vPos.y/2-1+(displacement)), (vPos.z), 1.0);
	UV = vUV;
}

