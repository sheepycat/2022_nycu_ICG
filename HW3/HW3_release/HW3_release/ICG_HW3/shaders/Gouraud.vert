#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 camera;

out vec2 texCoord;
out vec4 worldPos;
out vec3 normal;
out vec3 ambient;
out vec3 diffuse;
out vec3 specular;

void main()
{
	gl_Position = P * V * M * vec4(aPos, 1.0);
	texCoord = aTexCoord;
	worldPos = M * vec4(aPos, 1.0);
	mat4 normal_transform = transpose(inverse(M));
	normal = normalize((normal_transform * vec4(aNormal, 0.0)).xyz);
	vec3 view_pos = camera;
	
	vec3 lightPos = vec3(10, 10, 10);
	vec3 Ka = vec3(1.0, 1.0, 1.0);
	vec3 Kd = vec3(1.0, 1.0, 1.0);
	vec3 Ks = vec3(0.7, 0.7, 0.7);
	vec3 La = vec3(0.2, 0.2, 0.2);
	vec3 Ld = vec3(0.8, 0.8, 0.8);
	vec3 Ls = vec3(0.5, 0.5, 0.5);
	float a = 10.5;

	//ambient
	vec3 ambient_tmp = La * Ka;
	ambient = ambient_tmp;

	//diffuse
	vec3 n_normal = normalize(normal);
	vec3 light = (normalize(vec4(lightPos, 1.0) - worldPos)).xyz;	
	vec3 diffuse_tmp = Ld * Kd* max(0.0, dot(light, n_normal));
	diffuse = diffuse_tmp;

	//specular
	vec3 view_dir = normalize(view_pos - vec3(worldPos));
	vec3 reflect_dir = normalize(reflect(-lightPos, normal));
	vec3 specular_tmp = Ls * Ks * pow(max(0.0, dot(view_dir, reflect_dir)), a);
	specular = specular_tmp;


	
}