#version 440 core

in vec3 Color;
in vec2 Texcoord; 

layout(binding=0) uniform sampler2D diffuse1;

void main() { 
	gl_FragColor = texture2D(diffuse1, Texcoord);
}
