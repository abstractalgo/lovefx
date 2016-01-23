#version 330

in vec2 uv;

uniform sampler2D tex;
//uniform float amount;
//uniform float angle;

out vec4 fragCol;

void main()
{
	float amount = .002;
	float angle = 10.0;
    vec2 offset = amount * vec2( cos(angle), sin(angle));
    vec4 cr     = texture(tex, uv + offset);
    vec4 cga    = texture(tex, uv);
    vec4 cb     = texture(tex, uv - offset);
    fragCol     = vec4(cr.r, cga.g, cb.b, cga.a);
}