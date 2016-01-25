#version 330

in vec3 iPos;
in vec2 iUV;
in vec3 iNor;

uniform mat4 mvp;

out vec3 nor;
out vec2 uv;

void main()
{
    gl_Position = mvp * vec4(iPos*0.0005, 1.0);
    uv = iUV;
    nor = iNor;
}