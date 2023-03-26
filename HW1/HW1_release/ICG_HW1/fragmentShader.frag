#version 330 core

in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;

out vec4 FragColor;

void main()
{
	vec3 lightPos = vec3(5, 10, 10);
	vec3 n_normal = normalize(normal);
	vec3 light = (normalize(vec4(lightPos, 1.0) - worldPos)).xyz;

	float diffuse = max(dot(light, n_normal), 0.0);
	FragColor = vec4(diffuse, diffuse, diffuse, 1);
}