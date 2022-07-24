#version 440 core

out vec4 FragPos;

in vec3 Normal;
in vec2 TexCoords;

struct Material
{
	sampler2D baseColor;
};

uniform Material material;

void main()
{
	FragPos = texture(material.baseColor, TexCoords);
}