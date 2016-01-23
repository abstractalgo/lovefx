#version 330

in vec2 iPos;

uniform mat4 mMat;

out vec2 uv;

void main()
{
    gl_Position = mMat* vec4(iPos/3.0, 0.5, 1.0);
    uv = (iPos + vec2(1.0)) * 0.5;
}