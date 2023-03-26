#version 330 core


in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;
in vec3 view_pos;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	vec3 obj_color = vec3(texture(ourTexture, texCoord));
	vec4 color;
	vec3 lightPos = vec3(5, 10,	10);
	vec3 n_normal = normalize(normal);
	vec3 light = (normalize(vec4(lightPos, 1.0) - worldPos)).xyz;
	float diffuse = max(dot(light, n_normal), 0.0);
	float facing = dot(normalize(view_pos), n_normal);
	if(facing<=0.3){
		FragColor = (1-facing*1.5 ) * vec4(0.0,0.0,1.0,1.0);
	}
	else{
		FragColor = vec4(0.0,0.0,0.0,0.0) ;
	}

	
}