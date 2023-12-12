
#version 450
out vec4 FragColor;

in Surface2{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
	float Dist;
}fs_in;

uniform sampler2D _Texture;

void main(){
	
	float color = fs_in.Dist;
	
	FragColor = vec4(0.0f,sin(color),0.0f,1.0f) * texture(_Texture,fs_in.UV);

}