#version 330

in vec2 uv;
uniform sampler2D tex;
out vec4 frag;

void main()
{
    frag = texture(tex, uv);
}