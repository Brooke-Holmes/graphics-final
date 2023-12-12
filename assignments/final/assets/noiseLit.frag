
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
	//if(fs_in.Dist >= 0.5f){color = 1.0f;}
	//else{color = 0.0f;}
	FragColor = vec4(0.0f,color,0.0f,1.0f) /* * texture(_Texture,fs_in.UV)*/;

}