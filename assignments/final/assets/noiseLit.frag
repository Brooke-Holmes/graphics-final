//copy of autumn's assingment 7, expanded to include the noise function
#version 450
out vec4 FragColor;

struct Light
{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 3
uniform Light _Light[MAX_LIGHTS];


uniform vec3 cameraPos;
uniform vec3 ambientColor;

uniform bool BP;

uniform int numLights;

struct Material
{
	float ambientK; // ambient coefficient (0-1)
	float diffuseK; //Diffuse Coefficient (0-1)
	float specularK; //Specular coefficient (0-1)
	float shininess;//shininess
};
uniform Material _Material;


in Surface2{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
	float Dist;
}fs_in;


uniform sampler2D _Texture;

void main(){
	vec3 results[MAX_LIGHTS];
	vec3 resultColor;
	vec3 ambient = ambientColor * _Material.ambientK;
	for(int i = 0; i<numLights; i++){
	vec3 lightVector = normalize(vec3(_Light[i].position - fs_in.WorldPosition));
	vec3 cameraVector = normalize(vec3(cameraPos - fs_in.WorldPosition));
	vec3 normal = normalize(fs_in.WorldNormal);
	//add lighting calculations
	
	float diff = max(dot(normal, lightVector), 0);
	vec3 r;
	float spec;
	if(dot(lightVector, normal) >= 0.0){
	if(BP){ 
		r = normalize(lightVector + cameraVector);
		spec = pow(max(dot(normal, r), 0.0f), _Material.shininess);
	}
	else{ 
		r = reflect(-1*lightVector, normal);
		spec = pow(max(dot(cameraVector, r), 0.0f), _Material.shininess);	
	}
	}

	vec3 diffuse = _Light[i].color * diff * _Material.diffuseK;
	vec3 specular = _Light[i].color * spec * _Material.specularK;
	
	results[i] = diffuse + specular;
	resultColor += results[i];
	}
	resultColor += ambient;

	if(fs_in.Dist > -1.0f){
		float color = 1 - ((fs_in.Dist/50.f) * cos(fs_in.Dist));//50.0f is a magic number, but its the plane_width/2

		if(color <= 0.9f){
			FragColor = vec4(resultColor,1.0) * (texture(_Texture,fs_in.UV) + vec4(color,color,color,1.0f));
		}
		else{ FragColor = vec4(resultColor,1.0) *texture(_Texture,fs_in.UV); }
	}
	else { FragColor = vec4(resultColor,1.0) *texture(_Texture,fs_in.UV); }
}