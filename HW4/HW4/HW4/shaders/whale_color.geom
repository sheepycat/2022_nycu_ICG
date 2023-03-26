#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 normal;
	vec2 texCoord;
} gs_in[];

uniform mat4 P;

out vec3 color;

const float MAGNITUDE = 15;

void GenerateLine(int index)
{
	
	gl_Position = P * gl_in[index].gl_Position;
	color = vec3(0.55+gl_Position.x, 0.5+gl_Position.x/gl_Position.z, 1.2/gl_Position.z);
	if(gl_Position.y > 1.3 && gl_Position.x < 0.3 && gl_Position.x > -0.3 ){
		EmitVertex();
		gl_Position = P * (gl_in[index].gl_Position + 
							vec4(gs_in[index].normal, 0.0) * MAGNITUDE );
		color = vec3(0.75+gl_Position.x, 0.7+gl_Position.x/gl_Position.z, 1.5/gl_Position.z);
		//color = vec3(1.0, 1.0, 1.0);
		EmitVertex();
		EndPrimitive();
	}
	
	
}

void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}