#version 440 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
	mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.TexCoord = aTexCoords;
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

	vec3 T = normalize(vec3(model * vec4(aTangent, 1.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 1.0)));
	vec3 B = cross(T, N);

	vs_out.TBN = mat3(T, B, N);
}