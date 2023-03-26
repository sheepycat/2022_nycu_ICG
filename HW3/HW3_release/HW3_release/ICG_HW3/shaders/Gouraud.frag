#version 330 core


in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;
in vec3 ambient;
in vec3 diffuse;
in vec3 specular;

uniform sampler2D ourTexture;

out vec4 FragColor;


void main()
{
	vec3 obj_color = vec3(texture(ourTexture, texCoord));

	//ambient
	vec3 ambient_frag = ambient * obj_color;

	//diffuse	
	vec3 diffuse_frag = diffuse * obj_color;

	FragColor = vec4((ambient_frag + diffuse_frag + specular), 1.0);
}