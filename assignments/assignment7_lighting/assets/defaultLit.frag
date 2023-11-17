#version 450
out vec4 FragColor;

in Surface {
    vec2 UV; // Per-fragment interpolated UV
    vec3 WorldPosition; // Per-fragment interpolated world position
    vec3 WorldNormal; // Per-fragment interpolated world normal
} fs_in;

uniform sampler2D _Texture;

struct Light {
    vec3 position;
    vec3 color;
};
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];

// Exposed material properties
uniform float ambientK;
uniform float diffuseK;
uniform float specularK;
uniform float shininess;

void main() {
    vec3 normal = normalize(fs_in.WorldNormal);
    vec3 viewDir = normalize(fs_in.WorldPosition - vec3(0, 0, 0)); // Assuming the camera is at the origin

    vec3 finalColor = vec3(0.0);

    for (int i = 0; i < MAX_LIGHTS; ++i) {
        vec3 lightDir = normalize(_Lights[i].position - fs_in.WorldPosition);
        vec3 halfwayDir = normalize(lightDir + viewDir);

        float diffuseIntensity = max(dot(normal, lightDir), 0.0);
        float specularIntensity = pow(max(dot(normal, halfwayDir), 0.0), shininess);

        vec3 diffuseColor = diffuseK * _Lights[i].color * diffuseIntensity;
        vec3 specularColor = specularK * _Lights[i].color * specularIntensity;

        finalColor += ambientK + diffuseColor + specularColor;
    }

    vec3 textureColor = texture(_Texture, fs_in.UV).rgb;
    finalColor *= textureColor; // You may want to adjust this based on your needs

    FragColor = vec4(finalColor, 1.0);
}
