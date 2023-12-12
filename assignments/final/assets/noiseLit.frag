
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
	
	float color = sin(fs_in.Dist);
	if(color >= 0.9f){
	FragColor = vec4(color,color,color,1.0f);
	}
	else
	{
	FragColor = texture(_Texture,fs_in.UV);
	}
}