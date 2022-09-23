#version 450 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D shadowMap;
uniform float far_plane;
uniform float near_plane;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{
	float depthValue = texture(shadowMap, texCoord).r;
	FragColor = vec4(vec3(depthValue), 1.0);
}