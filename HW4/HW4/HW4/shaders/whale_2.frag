#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec4 worldPos;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	//vec3 obj_color = vec3(texture(ourTexture, texCoord));
	vec4 color;
	vec3 lightPos = vec3(10, 10, 10);
	vec3 n_normal = normalize(normal);
	vec3 light = (normalize(vec4(lightPos, 1.0) - worldPos)).xyz;
	float diffuse = max(dot(light, n_normal), 0.0);
	
	
	if(diffuse > 0.9) color = vec4(1.0, 0.5, 0.5, 1.0);
	else if(diffuse > 0.70) color = vec4(0.8, 0.8, 0.5, 1.0);
	else if(diffuse > 0.55) color = vec4(1.0, 1.0, 0.5, 1.0);
	else if(diffuse > 0.45) color = vec4(0.5, 1.0, 0.5, 1.0);
	else if(diffuse > 0.15) color = vec4(0.5, 0.5, 1.0, 1.0);
	else if(diffuse > 0.05) color = vec4(0.8, 0.5, 1.0, 1.0);
	else color = vec4(1.0, 0.5, 1.0, 1.0);

	FragColor = color;

	//FragColor = texture(ourTexture, texCoord);
}