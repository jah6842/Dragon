#version 440 core

in vec3 Color;
in vec2 Texcoord; 

layout(binding=0) uniform sampler2D diffuse1;
layout(binding=1) uniform sampler2D diffuse2;

uniform float time;

void main() { 
	gl_FragColor = mix(texture2D(diffuse1, Texcoord), texture2D(diffuse2, Texcoord), (sin(time) + 1) / 2);
	gl_FragColor = vec4(Color, 1.0) * gl_FragColor;
}
