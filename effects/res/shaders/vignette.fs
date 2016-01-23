#version 330

in vec2 uv;
uniform sampler2D tex;
out vec4 frag;

void main()
{
    vec4 col = texture(tex, uv);
    vec2 vc = (uv * 2.0) - 1.0;
    frag = mix(col,vec4(.0), abs(vc.x)*abs(vc.y));
}