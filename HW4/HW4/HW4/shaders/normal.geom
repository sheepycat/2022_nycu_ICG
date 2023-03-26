#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 normal;
	vec2 texCoord;
} gs_in[];

uniform mat4 P;

out vec3 color;

const float MAGNITUDE = 0.05;

void GenerateLine(int index)
{
	color = vec3(0.0);
	color[index] = 1.0;
	gl_Position = P * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = P * (gl_in[index].gl_Position + 
						vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}