#version 330 core

in vec2 texCoord;
in vec3 normal;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	FragColor = texture(ourTexture, texCoord);
}