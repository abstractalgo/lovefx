#version 330

in vec3 iPos;
in vec2 iUV;
in vec3 iNor;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 nor;
out vec2 uv;

void main()
{
    gl_Position = projMat
    			* viewMat
    			* modelMat
    			* vec4(iPos*0.0003,1.);

    uv = iUV;
    nor = normalize(iNor);
}