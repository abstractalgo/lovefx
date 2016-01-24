#version 330

in vec3 iPos;
in vec2 iUV;
in vec3 iNor;

out vec3 nor;
out vec2 uv;

void main()
{
    gl_Position = vec4(iPos*0.001, 1.0);
    uv = iUV;
    nor = iNor;
}