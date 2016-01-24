#version 330

in vec3 iPos;
//in vec2 iUV;
//out vec2 uv;

void main()
{
    gl_Position = vec4(iPos, 1.0);
    //uv = iUV;
}