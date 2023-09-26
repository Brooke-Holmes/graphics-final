const char* fragmentShaderSource = R"(
	#version 450
	out vec4 FragColor;
	in vec2 UV;
    uniform vec2 iResolution;
    uniform float iTime;
void main(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = fragCoord / iResolution.xy;

    // Time
    float timePeriod = 1.0; // Adjust this value to control the speed of the animation
    float sunYOffset = 0.0 + 0.8 * sin(iTime * timePeriod); // Adjust the amplitude (0.8) and offset (0.0) for the desired motion range

    // Sky
    vec3 skyColorTop = vec3(1.0 + 0.8 * sin(iTime * timePeriod), 0.0, 0.0); // Red color at the top of the sky
    vec3 skyColorBottom = vec3(0.85 + 0.8 * sin(iTime * timePeriod), 0.1 + 0.8 * sin(iTime * timePeriod), 0.25); // #ddf542 color at the bottom of the sky
    vec3 skyGradientColor = mix(skyColorBottom, skyColorTop, uv.y);

    // Sun
    vec2 sunSize = vec2(0.2, 0.35); // Size of the oval sun (width and height)
    vec3 sunColor = vec3(1.0, 1.0, 0.0); // Color of the sun
    vec2 distanceToSun = abs(uv - vec2(0.5, sunYOffset)) / sunSize;
    float blurRadius = 0.3; // Adjust this value to control the blurriness of the sun
    float alpha = 1.0 - smoothstep(1.0 - blurRadius, 2.0, length(distanceToSun));

    if ((distanceToSun.x * distanceToSun.x + distanceToSun.y * distanceToSun.y) <= 1.0) {
        // Set the color to the sun color with transparency
        fragColor = vec4(sunColor, alpha);
    } else {
        // Hills
        float hillAmplitude = 0.1;
        float hillFrequency = 10.0;
        float hillOffset = 0.0;
        float hillYOffset = hillAmplitude * sin(hillFrequency * uv.x + hillOffset);
        float yOffset = 0.2 + hillYOffset;
        vec3 hillColor = vec3(0.0, 0.5, 0.0);

        if (uv.y < yOffset) {
            // Set the color to the hill color
            fragColor = vec4(hillColor, 1.0);
        } else {
            // Set the color to the sky gradient
            fragColor = vec4(skyGradientColor, 1.0);
        }
    }
}

)";
