
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
	
	float color = 1 - ((fs_in.Dist/50.f) * cos(fs_in.Dist));

	if(color <= 0.9f){
	FragColor =  (texture(_Texture,fs_in.UV) + vec4(color,color,color,1.0f));
	}
	else
	{
	FragColor = texture(_Texture,fs_in.UV);
	}
}