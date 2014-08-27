#version 440 core

in vec3 Color;
in vec2 Texcoord; 
out vec4 FragColor;

layout(binding=0) uniform sampler2D diffuse1;

void main() {
    vec2 z;
	int iter = 10;
	vec2 c = vec2(1000, 1000);
    z.x = 3.0 * (Texcoord.x - 0.5);
    z.y = 2.0 * (Texcoord.y - 0.5);

    int i;
    for(i=0; i<iter; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }

    FragColor = texture2D(diffuse1, vec2(1, (i == iter ? 0.0 : float(i)) / 100.0));
}
