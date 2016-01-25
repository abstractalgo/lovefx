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
    // gl_Position = projMat
    // 			* viewMat
    // 			* modelMat
    // 			* vec4(iPos*0.00003,1.);

    vec3 wPos = (modelMat * vec4(iPos*0.00003,1.)).xyz;
vec3 cPos = (viewMat * vec4(wPos, 1.0)).xyz;
gl_Position = projMat * vec4(cPos, 1.0);

    uv = iUV;
    nor = iNor;
}