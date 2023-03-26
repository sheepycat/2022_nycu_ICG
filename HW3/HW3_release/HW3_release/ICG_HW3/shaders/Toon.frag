#version 330 core

in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;

uniform sampler2D ourTexture;



out vec4 FragColor;

void main()
{
	vec3 obj_color = vec3(texture(ourTexture, texCoord));
	vec4 color;
	vec3 lightPos = vec3(10, 10, 10);
	vec3 n_normal = normalize(normal);
	vec3 light = (normalize(vec4(lightPos, 1.0) - worldPos)).xyz;
	float diffuse = max(dot(light, n_normal), 0.0);
	
	
	if(diffuse > 0.95) color = vec4(1.0, 1.0, 1.0, 1.0);
	else if(diffuse > 0.5) color = vec4(0.7, 0.7, 0.7, 1.0);
	else if(diffuse > 0.25) color = vec4(0.3, 0.3, 0.3, 1.0);
	else color = vec4(0.1, 0.1, 0.1, 1.0);

	FragColor = color * vec4(obj_color,1.0);
	
}