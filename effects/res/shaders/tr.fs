#version 330

in vec2 uv;
out vec4 frag;

void main()
{
    frag = vec4(uv, 0.5, 1.0);
}