#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

out vec4 FragColor;

uniform vec3 _Color;

void main(){
	FragColor = vec4(_Color,1.0);
}
