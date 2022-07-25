#version 440 core

out vec4 FragColor;

in vec2 TexCoords;

uniform vec2 resolution;
uniform int ssaaLevel;
uniform sampler2D renderTexture;

void main()
{
	float tx = 1.0 / resolution.x;
	float ty = 1.0 / resolution.y;
	vec2 texelSize = vec2(tx, ty);

	vec3 result = vec3(0.0, 0.0, 0.0);
	int sampleCount = ssaaLevel / 2;

	for (int x = -sampleCount; x <= sampleCount; ++x)
	{
		for (int y = -sampleCount; y <= sampleCount; ++y)
		{
			result += texture(renderTexture, TexCoords + vec2(x, y) * texelSize).rgb;
		}
	}

	float divider = pow(ssaaLevel + 1, 2);

	FragColor = vec4(result / divider, 1.0);

	// FragColor = texture(render, TexCoords);
}