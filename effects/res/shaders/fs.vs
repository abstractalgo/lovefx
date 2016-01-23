#version 330

in vec2 iPos;
out vec2 uv;

void main()
{
    gl_Position = vec4(iPos, 0.5, 1.0);
    uv = (iPos + vec2(1.0)) * 0.5;
}