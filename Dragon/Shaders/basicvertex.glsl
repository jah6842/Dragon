#version 440 core

in vec2 position; 
in vec3 color;
in vec2 texcoord; 

out vec3 Color; 
out vec2 Texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() { 
	Texcoord = texcoord;
	Color = color; 
	//gl_Position = vec4(position, 0.0, 1.0); 
	gl_Position = proj * view * model * vec4(position, 0.0, 1.0);
}
