#version 440 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
	mat3 TBN;
} fs_in;

struct Material
{
	sampler2D baseColor;
	sampler2D normal;
	sampler2D arm;
};

uniform Material material;

uniform	sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

float bias;

float ShadowCalculation(vec4 fragPosLightSpace)
{	
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;   // Normalize coords
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	if (currentDepth > 1.0)
		return 0.0;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	//float bias = 0.005;
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9;

	return shadow;
}

void main()
{
	// normal = texture(material.normal, fs_in.TexCoord).rgb;
	// normal = normalize(normal * 2.0 - 1.0);

	vec3 color = texture(material.baseColor, fs_in.TexCoord).rgb;
	vec3 normal = texture(material.normal, fs_in.TexCoord).rgb;
	normal = normalize(fs_in.TBN * (normal * 2.0 - 1.0));
	//normal = -normal;
	vec3 lightColor = vec3(1.0);

	vec3 ambient = 0.1 * lightColor;

	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor * vec3(texture(material.arm, fs_in.TexCoord).g);

	bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.05);

	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0);

	//FragColor = vec4(vec3(texture(shadowMap, ((fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w) * 0.5 + 0.5).xy).r), 1.0); 
	//FragColor = vec4(vec3(texture(shadowMap, ((fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w)).xy).r), 1.0); 
	//FragColor = vec4(vec3(texture(shadowMap, fs_in.TexCoord).r), 1.0);

	//FragColor = vec4(vec3(texture(material.arm, fs_in.TexCoord)), 1.0);
	//FragColor = vec4(vec3(texture(material.baseColor, fs_in.TexCoord)), 1.0);

	// if (texture(material.baseColor, fs_in.TexCoord).rgb == texture(shadowMap, fs_in.TexCoord).rgb) FragColor = vec4(1.0);
	// else FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
