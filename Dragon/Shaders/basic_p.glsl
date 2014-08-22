#version 440 core

in vec3 Color;

out vec4 FragColor;

void main() { 
	FragColor.xyz = Color;
	FragColor.w = 1.0f;
}
