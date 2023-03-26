#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform int key;

out vec2 texCoord;
out vec4 worldPos;
out vec3 normal;

void main()
{
	if(key==2){
		gl_Position = P * V * M * vec4(aPos.x*0.5, aPos.y*2, aPos.z, 1.0);
	}
	else if(key==3){
		gl_Position = P * V * M * vec4(aPos.x*1.5, aPos.y*0.5+1, aPos.z*0.5, 1.0);
	}
	else{
		gl_Position = P * V * M * vec4(aPos, 1.0);
	}
	
	texCoord = aTexCoord;
	worldPos = M * vec4(aPos, 1.0);
	mat4 normal_transform = transpose(inverse(M));
	normal = normalize((normal_transform * vec4(aNormal, 0.0)).xyz);;
}